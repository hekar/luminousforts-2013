import os

from .. src_module_builder import src_module_builder_t
from pyplusplus import code_creators


class ModuleGenerator(object):
    # Contains ref to settings during Parse()
    settings = None
    
    # Settings
    module_name = None
    split = False

    # Files to be included in the generated module file.
    files = []

    # Files that should be part of the generated vpc file.
    required_files = []

    # Symbols added to vpc
    module_symbols = []

    # Map some names
    dll_name = None
    path = 'specify a valid path'
    
    # Variables set by generation code
    includes = None
    symbols = []
    vpcdir = None
    srcdir = None
    mb = None
    
    def GetFiles(self):
        return self.files, []
    
    # Main method
    def Run(self):
        mb = self.CreateBuilder(*self.GetFiles())
        self.mb = mb
        self.Parse(mb)
        self.FinalOutput(mb)
        self.mb = None
        
    # Parse method. Implement this.
    def Parse(self, mb):
        assert 0, 'Must implement parse function'
        
    # Create builder
    def CreateBuilder(self, files, parseonlyfiles):
        mb = src_module_builder_t(files, self.includes, self.symbols, is_client=False)
        mb.parseonlyfiles = parseonlyfiles
        return mb
            
    def FinalOutput(self, mb):
        """ Finalizes the output after generation of the bindings.
            Writes output to file."""
        # Set pydocstring options
        mb.add_registration_code('bp::docstring_options doc_options( true, true, false );', tail=False)
    
        # Generate code
        mb.build_code_creator(module_name=self.module_name)
        self.PostCodeCreation(mb)
        
        # Add precompiled header + other general required stuff and write away
        self.AddAdditionalCode(mb)      
        if self.split:
            written_files = mb.split_module(os.path.join(self.path, self.module_name),
                                            on_unused_file_found=lambda file: print('Unused file: %s' % file))
        else:
            mb.write_module(os.path.join(os.path.abspath(self.path), self.module_name+'.cpp'))
            
    def PostCodeCreation(self, mb):
        """ Allows modifying mb.code_creator just after the code creation. """
        pass
        
    # Adds precompiled header + other default includes
    def AddAdditionalCode(self, mb):
        mb.code_creator.user_defined_directories.append(os.path.abspath('.'))
        header = code_creators.include_t('cbase.h')
        mb.code_creator.adopt_creator(header, 0)
        header = code_creators.include_t('srcpy.h')
        mb.code_creator.adopt_include(header)
        header = code_creators.include_t('tier0/memdbgon.h')
        mb.code_creator.adopt_include(header)
        
    def IncludeVarAndRename(self, varname, newvarname):
        v = self.mb.variables(varname)
        v.include()
        v.rename(newvarname)
        
    def AddProperty(self, cls, propertyname, getter, setter=''):
        cls.member_functions(getter).exclude()
        if setter: 
            cls.member_functions(setter).exclude()
        if setter:
            cls.add_property(propertyname, cls.member_function( getter ), cls.member_function( setter ))
        else:
            cls.add_property(propertyname, cls.member_function( getter ))
            
    def IncludeEmptyClass(self, mb, cls_name, no_init=True, removevirtual=True):
        c = mb.class_(cls_name)
        c.include()
        c.no_init = no_init
        if removevirtual:
            c.member_functions(allow_empty=True).virtuality = 'not virtual' 
        if c.classes(allow_empty=True):
            c.classes(allow_empty=True).exclude()
        if c.member_functions(allow_empty=True):
            c.member_functions(allow_empty=True).exclude()
        if c.variables(allow_empty=True):
            c.variables(allow_empty=True).exclude()
        if c.enumerations(allow_empty=True):
            c.enumerations(allow_empty=True).exclude()  
        return c
        
    def SetupProperty(self, cls, pyname, gettername, settername=None, excludesetget=True):
        """ Shortcut for adding a property and exluding the getter/setter functions. """
        cls = self.mb.class_(cls) if type(cls) == str else cls
        
        getter = cls.member_function(gettername)
        setter = cls.member_function(settername) if settername is not None else None
        
        if excludesetget:
            getter.exclude()
            if setter:
                setter.exclude()
        
        cls.add_property(pyname, getter, setter)
