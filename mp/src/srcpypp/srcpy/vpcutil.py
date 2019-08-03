#!/usr/bin/env python3
''' Quick and dirty VPC file parser.

    Returns a tree.
'''

from collections import defaultdict
import re
import os
import platform

valuepattern = re.compile('^(?P<key>-?\$?(\w|/|#|\+)+)(\s+(?P<condition>\[.*\])|\s+(?P<value>[^\[].*[^\]]))*$')
commentpattern = re.compile('^(?P<statement>.*)//.*')

verbose = 0

class VPCNode(defaultdict):
    ''' Helper class for storing the parsed key/values. '''
    def __init__(self, default_factory=lambda: VPCNode(), *args, **kwargs):
        super(VPCNode, self).__init__(default_factory, *args, **kwargs)
        
        self.nodevalues = []
        
    def ApplyBase(self, idx):
        if idx == -1:
            idx = len(self.nodevalues) - 1
        value = re.sub(r'^"|"$|""', '', self.nodevalues[idx])
        if idx == 0:
            return value.replace('$BASE', '')
        return value.replace('$BASE', self.ApplyBase(idx-1))
        
    @property
    def nodevalue(self):
        ''' Returns the combined value with all BASE values applied. 
            Note: in some cases it does not make sense to do this.
            In that case you should access the values list.
        '''
        return self.ApplyBase(-1)
        
def TestConditions(conditionstr, macros):
    ''' Test conditions of the format [ $COND1 || COND2 && (!$COND3 || $COND4)]
        - A macro starts with $ and is evaluated by checking it in the macros dict.
        - Can be grouped in ( .. )
        - Can be negated
        - Supports operators && and ||
    '''
    if not conditionstr:
        return True
        
    conditionstr = conditionstr.lstrip('[').rstrip(']')
    conditions = ['$'+macro for macro in macros.keys()] + ['$_TRUE']
    
    # Quick test
    if conditionstr in conditions:
        return True
        
    # Full test
    # First replace all groups
    hastest = False
    grouppattern = re.compile('(\(.*\))')
    for g in grouppattern.findall(conditionstr):
        conditionstr = conditionstr.replace(g, '$_TRUE' if TestConditions(g.lstrip('(').rstrip(')'), macros) else '$_FALSE')
        hastest = True
        
    # Replace Negation by False or True
    grouppattern = re.compile('(!\$\w+)')
    for g in grouppattern.findall(conditionstr):
        testmacro = g.lstrip('!')
        conditionstr = conditionstr.replace(g, '$_FALSE' if testmacro in conditions else '$_TRUE')
        
    # Test remainder
    operator = None
    retval = False
    grouppattern = re.compile('(\$\w+|\&\&|\|\|)')
    for g in grouppattern.findall(conditionstr):
        if g == '&&':
            operator = g
            if not retval:
                break
        elif g == '||':
            operator = g
            if retval:
                break
        elif g.startswith('$'):
            hasmacro = g in conditions
            if operator == None:
                retval = hasmacro
            elif operator == '||':
                retval |= hasmacro
            elif operator == '&&':
                retval &= hasmacro
            else:
                assert False, '%s: invalid operator %s in conditionstr "%s"' % (g, operator, conditionstr)
        else:
            assert False, 'invalid group %s in conditionstr "%s"' % (operator, conditionstr)
        
    return retval
        
def ApplyMacros(values, macros):
    if type(values) == str:
        return ApplyMacros([values], macros)[0]
        
    outvalues = []
    # Replace all values in the list
    # Keep replacing until the value does not change anymore
    for v in values:
        voriginal = v
        oldv = None
        i = 0
        while oldv != v:
            oldv = v
            for m, mrepl in macros.items():
                v = v.replace('$%s' % (m), mrepl)
                
            i += 1
            if i > 100:
                print('Unable to expand macros for %s!!!' % (voriginal))
                break
            
        outvalues.append(v)
        
    return outvalues
    
def ApplyMacrosToNodes(node, macros):
    node.nodevalues = ApplyMacros(node.nodevalues, macros)
    for keyname, childnode in node.items():
        ApplyMacrosToNodes(childnode, macros)

def NormIncludeDirectories(includedirectories):
    ''' On Mac/Linux, replace forward slash by back slashes. 

        VPC seem to contain Windows path notation, which is not accepted on Python Linux/OSX.
    '''
    if platform.system() != 'Windows':
        includedirectories = includedirectories.replace('\\', '/')
    return includedirectories
        
def ParseVPC(vpcpath, macros, curnode=None):
    vpcpath = NormIncludeDirectories(vpcpath)

    if verbose > 0:
        print('PARSING %s' % (vpcpath))
        
    # Create root data node
    # Format: Key : ValueList / SubNode]

    if not curnode:
        macros = dict(macros)
        macros[macros['GAMENAME'].upper()] = '' # Add the game name as a macro/symbol
        curnode = VPCNode()
        curnode.vpcpath = vpcpath
        curnode.macros = macros

    curkey = None
    nodestack = []
    
    with open(vpcpath, 'r', encoding='ascii', errors='ignore') as fp:
        multilineactive = False
        curmultiline = ''
    
        for line in fp:
            line = line.strip()
            
            # Skip comments and empty lines
            if not line or line.startswith('//'):
                continue
                
            # Strip comments from line
            match = commentpattern.match(line)
            if match:
                line = match.group('statement')
                
            # Push or pop node stack when finding a { or }
            if line == '{':
                nodestack.append(curnode)
                curnode = curnode[curkey]
                curnode.vpcpath = vpcpath
                continue
            elif line == '}':
                curnode = nodestack.pop()
                curkey = None
                continue
            # When ending with "\", it means it continues on the next line
            elif line.endswith('\\'):
                multilineactive = True
                curmultiline += line.rstrip('\\')
                continue
                
            if multilineactive:
                multilineactive = False
                line = curmultiline + line
                curmultiline = ''
                
            #assert line.startswith('$'), '%s does not start with $ (in "%s")' % (line, curnode.vpcpath)
            match = valuepattern.match(line)
            assert match, 'no match for line %s (in %s)' % (line, curnode.vpcpath)
            groups = match.groupdict('')
            
            if verbose > 3:
                print('Key: %s' % (groups['key']))
                print('Value: %s' % (groups['value']))
                print('Conditions: %s' % (groups['conditions']))
            
            # Test conditions: should either be empty or match a test condition
            conditionstr = groups['condition']
            if not TestConditions(conditionstr, macros):
                if verbose > 0:
                    print('Skipping %s (condition not met)' % (line))
                continue
                
            # Temp: skip all "Debug" configurations for now to avoid mixing symbols with Release
            # This should be done differently though
            if groups['key'] == '$Configuration' and groups['value'] and groups['value'].strip() != '"Release"':
                if verbose > 0:
                    print('Skipping configuration because not release')
                    print(groups['value'])
                continue
                    
            curkey = groups['key']
            curvalue = groups['value'].strip() # Always strip whitespace away
            curnode[curkey].nodevalues.append(curvalue)
            
            if curkey.lower() == '$include':
                # A special case is the "$Include" key. In this case we should parse the included files.
                path = re.sub(r'^"|"$|""', '', curvalue)
                path = ApplyMacros(path, macros)
                ParseVPC(path, macros, curnode)
            elif curkey.lower() == '$macro':
                # A second special case is the "$Macro" key. We could get this from the nodes, but it's more
                # easy to keep a separate dict too. The newest encountered macro will overwrite the older one.
                # Note: the existing macro might be used in the macro definition itself. In this case we should replace it here!
                macrokey, macrovalue = curvalue.split(maxsplit=1)
                macrovalue = re.sub(r'^"|"$|""', '', macrovalue)
                if macrokey in macros:
                    macrovalue = macrovalue.replace('$%s' % (macrokey), macros[macrokey])
                macros[macrokey] = macrovalue
            elif curkey.lower() == '$macrorequired':
                # MacroRequired is an assert check on the macros dict
                values = curvalue.split()
                macrokey = values[0]
                macrokey = re.sub(r'^"|"$|""', '', macrokey)
                assert len(values) > 1 or macrokey in macros, 'Macro "%s" is not defined (in %s)!' % (macrokey, curnode.vpcpath)
            
    return curnode
                
if __name__ == '__main__':
    macros = {
        'GAMENAME' : 'pysource', 
        'OUTDLLEXT' : '.dll', 
        '_DLL_EXT' : '.dll', 
        'PLATFORM' : 'WIN32',
        
        # Conditions
        'WIN32' : '',
        'VS2015' : '',
        'WINDOWS' : '',
    }
    
    path = r'C:\development\source-sdk-2013-sandern\mp\src\game\server\server_pysource.vpc'
    data = ParseVPC(path, macros=macros)
    ApplyMacrosToNodes(data, data.macros)
    print('\n\n')
    includes = data['$Configuration']['$Compiler']['$AdditionalIncludeDirectories'].nodevalue
    includes = set(filter(bool, re.findall('[^;,]*', includes)))
    for incl in includes:
        print(incl)
        
    defines = data['$Configuration']['$Compiler']['$PreprocessorDefinitions'].nodevalue
    defines = filter(bool, re.findall('[^;,]*', defines))
    for d in defines:
        print(d)