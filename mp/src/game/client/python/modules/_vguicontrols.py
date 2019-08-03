from srcpy.module_generators import ClientModuleGenerator, ClientReqFile
from srcpy.matchers import calldef_withtypes, MatcherTestInheritClass

from pygccxml.declarations import matcher, matchers, pointer_t, const_t, reference_t, declarated_t, char_t, int_t, wchar_t
from pyplusplus import function_transformers as FT
from pyplusplus.module_builder import call_policies

# Convert templates for without vgui lib
ptr_convert_to_py_name_template_novguilib = '''struct %(ptr_convert_to_py_name)s : bp::to_python_converter<%(clsname)s *, ptr_%(clsname)s_to_handle>
{
    static PyObject* convert( %(clsname)s *s )
    {
        if( s ) 
        {
            PyObject *pObject = GetPyPanel( s );
            if( pObject )
                return bp::incref( pObject );
            else
                return bp::incref( bp::object( %(handlename)s(s) ).ptr() );
        }
        else
        {
            return bp::incref( Py_None );
        }
    }
};'''

convert_to_py_name_template_novguilib = '''struct %(convert_to_py_name)s : bp::to_python_converter<%(clsname)s, %(clsname)s_to_handle>
{
    static PyObject* convert( const %(clsname)s &s )
    {
        PyObject *pObject = GetPyPanel( &s );
        if( pObject )
            return bp::incref( pObject );
        else
            return bp::incref( bp::object( %(handlename)s( &s ) ).ptr() );
    }
};'''

# Converters when having the vgui lib
ptr_convert_to_py_name_template = '''struct %(ptr_convert_to_py_name)s : bp::to_python_converter<%(clsname)s *, ptr_%(clsname)s_to_handle>
{
    static PyObject* convert(%(clsname)s *s)
    {
        if( s )
        {
            if( s->GetPySelf() != NULL )
                return bp::incref( s->GetPySelf() ); 
            else
                return bp::incref( bp::object( %(handlename)s( s ) ).ptr() );
        }
        else
        {
            return bp::incref(Py_None);
        }
    }
};'''

convert_to_py_name_template = '''struct %(convert_to_py_name)s : bp::to_python_converter<%(clsname)s, %(clsname)s_to_handle>
{
    static PyObject* convert(const %(clsname)s &s)
    {
        if( s.GetPySelf() != NULL )
            return bp::incref( s.GetPySelf() );
        else
            return bp::incref( bp::object( %(handlename)s( (%(clsname)s *)&s) ).ptr() );
    }
};'''

convert_from_py_name_template = '''struct %(convert_from_py_name)s
{
    handle_to_%(clsname)s()
    {
        bp::converter::registry::insert(
            &extract_%(clsname)s,
            bp::type_id< %(clsname)s >()
            );
    }
    
    static void* extract_%(clsname)s(PyObject* op)
    {
        PyBaseVGUIHandle &h = bp::extract< PyBaseVGUIHandle & >(op);
        return h.Get();
    }
};'''

vguihandle_template = '''{ //::%(handlename)s
        typedef bp::class_< %(handlename)s, bp::bases<PyBaseVGUIHandle> > %(handlename)s_exposer_t;
        %(handlename)s_exposer_t %(handlename)s_exposer = %(handlename)s_exposer_t( "%(handlename)s", bp::init< >() );
        %(handlename)s_exposer.def( bp::init<  %(clsname)s * >(( bp::arg("val") )) );
        { //::%(handlename)s::GetAttr
            typedef bp::object ( ::%(handlename)s::*GetAttr_function_type )( const char * ) const;
            %(handlename)s_exposer.def( 
                "__getattr__"
                , GetAttr_function_type( &::%(handlename)s::GetAttr )
            );
        }
    }
'''

class VGUIControls(ClientModuleGenerator):
    module_name = '_vguicontrols'
    split = True

    required_files = [
        ClientReqFile('$SRCDIR\game\client\python\srcpy_hud.cpp'),
        ClientReqFile('$SRCDIR\game\client\python\srcpy_hud.h'),
        ClientReqFile('$SRCDIR\game\client\python\srcpy_vgui.cpp'),
        ClientReqFile('$SRCDIR\game\client\python\srcpy_vgui.h'),
    ]

    module_symbols = ['SRCPY_MOD_VGUI']

    files = [
        'cbase.h',
        
        'vgui_controls/Panel.h',
        'vgui_controls/AnimationController.h',
        'vgui_controls/EditablePanel.h',
        'vgui_controls/AnalogBar.h',
        'vgui_controls/Image.h',
        'vgui_controls/TextImage.h',
        'vgui_controls/ScrollBar.h',
        'vgui_controls/ScrollBarSlider.h',
        'vgui_controls/Menu.h',
        'vgui_controls/MenuButton.h',
        'vgui_controls/Frame.h',
        'vgui_controls/TextEntry.h',
        'vgui_controls/RichText.h',
        'vgui_controls/Tooltip.h',
        
        'vgui/IBorder.h',

        'vgui_bitmapimage.h',
        'vgui_avatarimage.h',
        
        'srcpy_vgui.h',
    ]
    
    panel_cls_list = [  
        'AnimationController', 
        'Panel', 
        'EditablePanel', 
        'Frame', 
        'TextEntry', 
        'ScrollBar', 
        'ScrollBarSlider',
        'RichText',
    ]

    # Almost the same as above, maybe make a shared function?   
    def AddVGUIConverter(self, mb, clsname, novguilib, containsabstract=False):
        cls = mb.class_(clsname)
        
        handlename = clsname+'HANDLE'
        
        ptr_convert_to_py_name = 'ptr_%(clsname)s_to_handle' % {'clsname' : clsname}
        convert_to_py_name = '%(clsname)s_to_handle' % {'clsname' : clsname}
        convert_from_py_name = 'handle_to_%(clsname)s' % {'clsname' : clsname}
        
        # Arguments for templates
        strargs = {
            'clsname' : clsname,
            'handlename' : handlename,
            'ptr_convert_to_py_name' : ptr_convert_to_py_name,
            'convert_to_py_name' : convert_to_py_name,
            'convert_from_py_name' : convert_from_py_name,
        }
        
        # Add GetPySelf method
        cls.add_wrapper_code(
            'virtual PyObject *GetPySelf() const { return bp::detail::wrapper_base_::get_owner(*this); }'
        )
        cls.add_wrapper_code(
            'virtual bool IsPythonManaged() { return true; }'
        )
        
        constructors = cls.constructors(name=clsname)
        constructors.body = '\tg_PythonPanelCount++;'  
            
        cls.add_wrapper_code( '~%(clsname)s_wrapper( void ) { g_PythonPanelCount--; /*::PyDeletePanel( this, this );*/ }' % strargs ) 
        
        if novguilib:
            cls.add_wrapper_code( 'void DeletePanel( void ) { ::PyDeletePanel( this, this ); }' )
        
        # Add handle typedef
        mb.add_declaration_code( 'typedef PyVGUIHandle<%(clsname)s> %(handlename)s;\r\n' % strargs )
        
        # Expose handle code
        mb.add_registration_code(vguihandle_template % {'clsname' : clsname, 'handlename' : handlename}, True)
        
        # Add to python converters
        if novguilib:
            mb.add_declaration_code(ptr_convert_to_py_name_template_novguilib % strargs)
            
            if not containsabstract:
                mb.add_declaration_code(convert_to_py_name_template_novguilib % strargs)
        else:
            mb.add_declaration_code(ptr_convert_to_py_name_template % strargs)
            
            if not containsabstract:
                mb.add_declaration_code(convert_to_py_name_template % strargs)

        # Add from python converter
        mb.add_declaration_code(convert_from_py_name_template % strargs)
        
        # Add registration code
        mb.add_registration_code( ptr_convert_to_py_name+"();" )
        if not containsabstract:
            mb.add_registration_code( convert_to_py_name+"();" )
        mb.add_registration_code( convert_from_py_name+"();" )
        
    
    def ParseImageClasses(self, mb):
        # IBorder
        cls = mb.class_('IBorder')
        cls.include()
        cls.member_functions('ApplySchemeSettings').include()    
        cls.member_functions('Paint').virtuality = 'pure virtual'
        cls.member_functions('ApplySchemeSettings').virtuality = 'pure virtual'

        # IImage
        cls = mb.class_('IImage')
        cls.include()  
        cls.member_functions( 'GetContentSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions('Paint').virtuality = 'pure virtual'
        
        # Image
        cls = mb.class_('Image')
        cls.include()
        #cls.member_functions( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        cls.no_init = True
        #cls.calldefs().virtuality = 'not virtual' 
        cls.calldefs('Image').exclude()
        cls.member_functions( 'Paint' ).exclude()
        cls.add_wrapper_code( 'virtual void Paint() {}' )    # Stub for wrapper class. Otherwise it will complain.
        cls.member_functions( 'GetSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetContentSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        
        # FIXME: Py++ is giving problems on some functions
        cls.member_functions('SetPos').virtuality = 'not virtual'
        #cls.member_functions('GetPos').virtuality = 'not virtual'
        #cls.member_functions('GetSize').virtuality = 'not virtual'
        #cls.member_functions('GetContentSize').virtuality = 'not virtual'
        cls.member_functions('SetColor').virtuality = 'not virtual'
        cls.member_functions('SetBkColor').virtuality = 'not virtual'
        cls.member_functions('GetColor').virtuality = 'not virtual'
        
        cls.member_functions('SetSize').virtuality = 'not virtual' 
        cls.member_functions('DrawSetColor').virtuality = 'not virtual' 
        cls.member_functions('DrawSetColor').virtuality = 'not virtual' 
        cls.member_functions('DrawFilledRect').virtuality = 'not virtual' 
        cls.member_functions('DrawOutlinedRect').virtuality = 'not virtual' 
        cls.member_functions('DrawLine').virtuality = 'not virtual' 
        cls.member_functions('DrawPolyLine').virtuality = 'not virtual' 
        cls.member_functions('DrawSetTextFont').virtuality = 'not virtual' 
        cls.member_functions('DrawSetTextColor').virtuality = 'not virtual' 
        cls.member_functions('DrawSetTextPos').virtuality = 'not virtual' 
        cls.member_functions('DrawPrintText').virtuality = 'not virtual' 
        cls.member_functions('DrawPrintText').virtuality = 'not virtual' 
        cls.member_functions('DrawPrintChar').virtuality = 'not virtual' 
        cls.member_functions('DrawPrintChar').virtuality = 'not virtual' 
        cls.member_functions('DrawSetTexture').virtuality = 'not virtual' 
        cls.member_functions('DrawTexturedRect').virtuality = 'not virtual' 
        
        # TextImage
        cls = mb.class_('TextImage')
        cls.include()
        cls.calldefs().virtuality = 'not virtual' 
        cls.member_functions( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        #cls.calldefs('SetText', calldef_withtypes([pointer_t(const_t(declarated_t(wchar_t())))])).exclude()
        cls.member_functions( 'GetContentSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetDrawWidth' ).add_transformation( FT.output('width') )
        cls.member_functions( 'SizeText' ).exclude()     # DECLARATION ONLY
        
        cls.member_functions('GetText').exclude()
        cls.add_wrapper_code(
            'boost::python::object GetText() {\r\n' + \
            '    char buf[1025];\r\n' + \
            '    TextImage::GetText(buf, 1025);\r\n' + \
            '    return boost::python::object(buf);\r\n' + \
            '}'
        )
        cls.add_registration_code(
            'def( \r\n' + \
            '    "GetText"\r\n' + \
            '    , (boost::python::object ( TextImage_wrapper::* )())( &TextImage_wrapper::GetText ) )'
        )
        
        # BitmapImage
        cls = mb.class_('BitmapImage')
        cls.include()
        cls.calldefs().virtuality = 'not virtual' 
        if self.settings.branch == 'source2013':
            cls.member_function('SetBitmap').exclude()
        #cls.member_functions( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        
        cls.calldefs('GetColor', calldef_withtypes([reference_t(declarated_t(int_t()))])).add_transformation(FT.output('r'), FT.output('g'), FT.output('b'), FT.output('a'))
        cls.member_functions( 'GetSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        
        # CAvatarImage
        cls = mb.class_('CAvatarImage')
        cls.include()
        cls.calldefs().virtuality = 'not virtual' 
        cls.member_functions( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        cls.rename('AvatarImage')
        cls.member_functions( 'GetSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetContentSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'InitFromRGBA' ).exclude()
        
        mb.enumeration('EAvatarSize').include()
    
    def ParsePanelHandles(self, mb):
        # Base handle for Panels
        mb.class_('PyBaseVGUIHandle').include()
        mb.class_('PyBaseVGUIHandle').member_functions().exclude()

    def ParsePanels(self, mb):
        # Panels
        cls = mb.class_('DeadPanel')
        cls.include()
        cls.member_functions('NonZero', allow_empty=True).rename('__nonzero__')
        cls.member_functions('Bool', allow_empty=True).rename('__bool__')
        
        # For each panel sub class we take some default actions
        for cls_name in self.panel_cls_list:
            cls = mb.class_(cls_name)

            # Include everything by default
            cls.include()
            cls.no_init = False
            
            # Be selective about we need to override
            cls.member_functions().virtuality = 'not virtual' 
            
            #if cls_name not in ['AnimationController', 'Frame', 'ScrollBar', 'CBaseMinimap']:
            #    cls.member_functions( matchers.access_type_matcher_t( 'protected' ) ).exclude()
            
            # By default exclude any subclass. These classes are likely controlled intern by the panel
            if cls.classes(allow_empty=True):
                cls.classes().exclude()
                
            self.AddVGUIConverter(mb, cls_name, self.novguilib, containsabstract=False)
            
            # # Add custom wrappers for functions who take keyvalues as input
            if self.novguilib:
                # No access to source code, so need to add message stuff for python here.
                cls.add_wrapper_code('virtual void OnMessage(const KeyValues *params, VPANEL fromPanel) {\r\n' +
                                     '    if( Panel_DispatchMessage( m_PyMessageMap, params, fromPanel ) )\r\n' +
                                     '        return;\r\n' +
                                     '    Panel::OnMessage(params, fromPanel);\r\n' +
                                     '}\r\n' + \
                                     '\r\n' + \
                                     'void RegMessageMethod( const char *message, boost::python::object method, int numParams=0, \r\n' + \
                                     '       const char *nameFirstParam="", int typeFirstParam=DATATYPE_VOID, \r\n' + \
                                     '       const char *nameSecondParam="", int typeSecondParam=DATATYPE_VOID ) { \r\n' + \
                                     '       py_message_entry_t entry;\r\n' + \
                                     '       entry.method = method;\r\n' + \
                                     '       entry.numParams = numParams;\r\n' + \
                                     '       entry.firstParamName = nameFirstParam;\r\n' + \
                                     '       entry.firstParamSymbol = KeyValuesSystem()->GetSymbolForString(nameFirstParam);\r\n' + \
                                     '       entry.firstParamType = typeFirstParam;\r\n' + \
                                     '       entry.secondParamName = nameSecondParam;\r\n' + \
                                     '       entry.secondParamSymbol = KeyValuesSystem()->GetSymbolForString(nameSecondParam);\r\n' + \
                                     '       entry.secondParamType = typeSecondParam;\r\n' + \
                                     '\r\n' + \
                                     '       GetPyMessageMap().Insert(message, entry);\r\n' + \
                                     '}\r\n' + \
                                     'virtual Panel *GetPanel() { return this; }\r\n'
                                     )
                cls.add_registration_code('def( "RegMessageMethod", &'+cls_name+'_wrapper::RegMessageMethod\r\n' + \
                                               ', ( bp::arg("message"), bp::arg("method"), bp::arg("numParams")=(int)(0), bp::arg("nameFirstParam")="", bp::arg("typeFirstParam")=int(::vgui::DATATYPE_VOID), bp::arg("nameSecondParam")="", bp::arg("typeSecondParam")=int(::vgui::DATATYPE_VOID) ))' )
                                               
                # Add stubs
                cls.add_wrapper_code('virtual void EnableSBuffer( bool bUseBuffer ) { PyPanel::EnableSBuffer( bUseBuffer ); }')
                cls.add_registration_code('def( "EnableSBuffer", &%(cls_name)s_wrapper::EnableSBuffer, bp::arg("bUseBuffer") )' % {'cls_name' : cls_name})
                cls.add_wrapper_code('virtual bool IsSBufferEnabled() { return PyPanel::IsSBufferEnabled(); }')
                cls.add_registration_code('def( "IsSBufferEnabled", &%(cls_name)s_wrapper::IsSBufferEnabled )' % {'cls_name' : cls_name})
                cls.add_wrapper_code('virtual void FlushSBuffer() { PyPanel::FlushSBuffer(); }')
                cls.add_registration_code('def( "FlushSBuffer", &%(cls_name)s_wrapper::FlushSBuffer )' % {'cls_name' : cls_name})
                cls.add_wrapper_code('virtual void SetFlushedByParent( bool bEnabled ) { PyPanel::SetFlushedByParent( bEnabled ); }')
                cls.add_registration_code('def( "SetFlushedByParent", &%(cls_name)s_wrapper::SetFlushedByParent, bp::arg("bEnabled") )' % {'cls_name' : cls_name})
    
        # Tweak Panels
        # Used by converters + special method added in the wrapper
        # Don't include here
        if not self.novguilib:
            mb.member_functions('GetPySelf').exclude()
            mb.member_functions('PyDestroyPanel').exclude()
            
        # Exclude message stuff. Maybe look into wrapping this in a nice way
        mb.member_functions( 'AddToMap' ).exclude()
        mb.member_functions( 'ChainToMap' ).exclude()
        mb.member_functions( 'GetMessageMap' ).exclude()
        mb.member_functions( 'AddToAnimationMap' ).exclude()
        mb.member_functions( 'ChainToAnimationMap' ).exclude()
        mb.member_functions( 'GetAnimMap' ).exclude()
        mb.member_functions( 'KB_AddToMap' ).exclude()
        mb.member_functions( 'KB_ChainToMap' ).exclude()
        mb.member_functions( 'KB_AddBoundKey' ).exclude()
        mb.member_functions( 'GetKBMap' ).exclude()
        mb.member_functions( lambda decl: 'GetVar_' in decl.name ).exclude()
        
        mb.classes( lambda decl: 'PanelMessageFunc_' in decl.name ).exclude()
        mb.classes( lambda decl: '_Register' in decl.name ).exclude()
        mb.classes( lambda decl: 'PanelAnimationVar_' in decl.name ).exclude()
        mb.variables( lambda decl: '_register' in decl.name ).exclude()
        mb.variables( lambda decl: 'm_Register' in decl.name ).exclude()
        
        # Don't need the following:
        menu = mb.class_('Menu')
        keybindindcontexthandle = mb.enumeration('KeyBindingContextHandle_t')
        excludetypes = [
            pointer_t(const_t(declarated_t(menu))),
            pointer_t(declarated_t(menu)),
            reference_t(declarated_t(menu)),
            pointer_t(declarated_t(mb.class_('IPanelAnimationPropertyConverter'))),
            declarated_t(keybindindcontexthandle),
        ]
        mb.calldefs(calldef_withtypes(excludetypes), allow_empty=True).exclude()
        
    def ParsePanel(self, mb):
        # List of functions that should be overridable
        mb.member_functions('SetVisible').virtuality = 'virtual'
        mb.member_functions('SetParent').virtuality = 'virtual'
        mb.member_functions('SetEnabled').virtuality = 'virtual'
        
        mb.member_functions('SetBgColor').virtuality = 'virtual'
        mb.member_functions('SetFgColor').virtuality = 'virtual'
        mb.member_functions('SetCursor').virtuality = 'virtual'
        
        mb.member_functions('InvalidateLayout').virtuality = 'virtual'
        
        mb.member_functions('SetBorder').virtuality = 'virtual'
        mb.member_functions('SetPaintBorderEnabled').virtuality = 'virtual'
        mb.member_functions('SetPaintBackgroundEnabled').virtuality = 'virtual'
        mb.member_functions('SetPaintEnabled').virtuality = 'virtual'
        mb.member_functions('SetPaintBackgroundType').virtuality = 'virtual'
        mb.member_functions('SetScheme').virtuality = 'virtual'
        
        mb.member_functions('ApplySchemeSettings').virtuality = 'virtual'
        mb.member_functions('OnCommand').virtuality = 'virtual'
        mb.member_functions('OnMouseCaptureLost').virtuality = 'virtual'
        mb.member_functions('OnSetFocus').virtuality = 'virtual'
        mb.member_functions('OnKillFocus').virtuality = 'virtual'
        mb.member_functions('OnDelete').virtuality = 'virtual'
        mb.member_functions('OnThink').virtuality = 'virtual'
        mb.member_functions('OnChildAdded').virtuality = 'virtual'
        mb.member_functions('OnSizeChanged').virtuality = 'virtual'
        mb.member_functions('OnTick').virtuality = 'virtual'
        
        mb.member_functions('OnCursorMoved').virtuality = 'virtual'
        mb.member_functions('OnCursorEntered').virtuality = 'virtual'
        mb.member_functions('OnCursorExited').virtuality = 'virtual'
        mb.member_functions('OnMousePressed').virtuality = 'virtual'
        mb.member_functions('OnMouseDoublePressed').virtuality = 'virtual'
        mb.member_functions('OnMouseReleased').virtuality = 'virtual'
        mb.member_functions('OnMouseWheeled').virtuality = 'virtual'
        mb.member_functions('OnMouseTriplePressed').virtuality = 'virtual'
        
        mb.member_functions('OnKeyCodePressed').virtuality = 'virtual'
        mb.member_functions('OnKeyCodeTyped').virtuality = 'virtual'
        mb.member_functions('OnKeyCodeReleased').virtuality = 'virtual'
        mb.member_functions('OnKeyFocusTicked').virtuality = 'virtual'
        mb.member_functions('OnMouseFocusTicked').virtuality = 'virtual'
        
        mb.member_functions('PaintBackground').virtuality = 'virtual'
        mb.member_functions('Paint').virtuality = 'virtual'
        #mb.member_functions('PaintBorder').virtuality = 'virtual' # TODO: Don't believe we are ever interested in painting borders in python
        mb.member_functions('PaintBuildOverlay').virtuality = 'virtual'
        mb.member_functions('PostChildPaint').virtuality = 'virtual'
        mb.member_functions('PerformLayout').virtuality = 'virtual'
        
        mb.member_functions('SetMouseInputEnabled').virtuality = 'virtual'
        mb.member_functions('SetKeyBoardInputEnabled').virtuality = 'virtual'
        
        mb.member_functions('SetDragEnabled').virtuality = 'virtual'
        
        mb.member_functions('OnRequestFocus').virtuality = 'virtual'
        mb.member_functions('OnScreenSizeChanged').virtuality = 'virtual'

        # Transformations
        mb.member_functions( 'GetPos' ).add_transformation( FT.output('x'), FT.output('y') )
        mb.class_('Panel').member_functions( 'GetSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        mb.class_('Panel').member_functions( 'GetBounds' ).add_transformation( FT.output('x'), FT.output('y'), FT.output('wide'), FT.output('tall') )
        mb.member_functions( 'GetMinimumSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        mb.member_functions( 'LocalToScreen' ).add_transformation( FT.inout('x'), FT.inout('y') )
        mb.member_functions( 'ScreenToLocal' ).add_transformation( FT.inout('x'), FT.inout('y') )
        mb.member_functions( 'ParentLocalToScreen' ).add_transformation( FT.inout('x'), FT.inout('y') )
        mb.member_functions( 'GetInset' ).add_transformation( FT.output('left'), FT.output('top'), FT.output('right'), FT.output('bottom') )
        mb.member_functions( 'GetPaintSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        mb.member_functions( 'GetClipRect' ).add_transformation( FT.output('x0'), FT.output('y0'), FT.output('x1'), FT.output('y1') )
        mb.member_functions( 'GetPinOffset' ).add_transformation( FT.output('dx'), FT.output('dy') )
        mb.member_functions( 'GetResizeOffset' ).add_transformation( FT.output('dx'), FT.output('dy') )
        mb.member_functions( 'GetCornerTextureSize' ).add_transformation( FT.output('w'), FT.output('h') )    
        
        # Exclude list
        mb.member_functions('QueryInterface').exclude()
        
        # We don't care about build mode, since we can easily reload modules in python
        # We also don't want the user to be able to call methods like Delete.
        mb.member_functions('IsBuildModeEditable').exclude()
        mb.member_functions('SetBuildModeEditable').exclude()
        mb.member_functions('IsBuildModeDeletable').exclude()
        mb.member_functions('SetBuildModeDeletable').exclude()
        mb.member_functions('IsBuildModeActive').exclude()
        #mb.member_functions('SetAutoDelete').exclude()
        #mb.member_functions('IsAutoDeleteSet').exclude()
        mb.member_functions('OnDelete').exclude()
        mb.member_functions('MarkForDeletion').exclude()
        mb.member_functions('SetBuildGroup').exclude()
        mb.member_functions('IsBuildGroupEnabled').exclude()
        mb.member_functions('CreateControlByName').exclude()
        
        mb.member_functions('LoadKeyBindings').exclude()
        mb.member_functions('SaveKeyBindingsToBuffer').exclude()
        mb.member_functions('LookupBoundKeys').exclude()
        mb.member_functions('OnKeyTyped').exclude()
        mb.member_functions('HasHotkey').exclude()

        mb.member_functions('GetDragData').exclude()
        if self.settings.branch == 'swarm':
            mb.member_functions('GetDragFailCursor').exclude()
        mb.member_functions('GetDropCursor').exclude()
        mb.member_functions('GetDropTarget').exclude()
        mb.member_functions('IsDroppable').exclude()
        mb.member_functions('OnPanelDropped').exclude()
        mb.member_functions('OnPanelEnteredDroppablePanel').exclude()
        mb.member_functions('OnPanelExitedDroppablePanel').exclude()
        mb.member_functions('OnDragFailed').exclude()
        mb.member_functions('OnDropContextHoverHide').exclude()
        mb.member_functions('OnDropContextHoverShow').exclude()
        mb.member_functions('OnDroppablePanelPaint').exclude()
        mb.member_functions('OnGetAdditionalDragPanels').exclude()
        mb.member_functions('OnDragFailed').exclude()
        
        mb.variables('m_PanelMap').exclude()
        mb.variables('m_MessageMap').exclude()
        mb.member_functions('GetPanelMap').exclude()
        if self.settings.branch == 'source2013':
            mb.member_functions('GetChildren').exclude()
       
        # Must use return_by_value. Then the converter will be used to wrap the vgui element in a safe handle
        mb.member_functions( 'GetChild' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'GetBorder' ).call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
        if self.settings.branch == 'source2007':
            mb.member_functions('GetBorderAtIndex').call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
        mb.member_functions( 'GetParent' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'FindSiblingByName' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'FindChildByName' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'HasHotkey' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'GetPanelWithKeyBindings' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'LookupBinding' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'LookupBindingByKeyCode' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'LookupDefaultKey' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'LookupMapForBinding' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'GetTooltip' ).call_policies = call_policies.return_internal_reference()  
        mb.member_functions( 'GetDragDropInfo' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'GetDragPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'GetPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'FindPanelAnimationEntry' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'FindDropTargetPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        
        if self.settings.branch == 'swarm':
            mb.member_functions( 'GetNavDown' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavDownPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavLeft' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavLeftPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavRight' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavRightPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavUp' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'GetNavUpPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'NavigateDown' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'NavigateLeft' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'NavigateRight' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'NavigateTo' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'NavigateUp' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'SetNavDown' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'SetNavLeft' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'SetNavRight' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            mb.member_functions( 'SetNavUp' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 

            # Exclude
            mb.member_functions('OnUnserialized').exclude()
            mb.member_functions('GetSizer').exclude()
            mb.member_functions('GetUnpackStructure').exclude()
            
        if self.settings.branch == 'swarm':
            # Tooltip class
            cls = mb.class_('Tooltip')
            cls.include()
        
    def ParseEditablePanel(self, mb):
        focusnavgroup = mb.class_('FocusNavGroup')
        buildgroup = mb.class_('BuildGroup')
        excludetypes = [
            pointer_t(const_t(declarated_t(focusnavgroup))),
            pointer_t(declarated_t(focusnavgroup)),
            reference_t(declarated_t(focusnavgroup)),
            pointer_t(const_t(declarated_t(buildgroup))),
            pointer_t(declarated_t(buildgroup)),
            reference_t(declarated_t(buildgroup)),
        ]
        mb.calldefs(calldef_withtypes(excludetypes), allow_empty=True).exclude()
        
        mb.member_functions( 'GetDialogVariables' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
    def ParseFrame(self, mb):
        # List of overridables
        mb.member_functions('SetTitle').virtuality = 'virtual'
        mb.member_functions('Activate').virtuality = 'virtual'
        mb.member_functions('ActivateMinimized').virtuality = 'virtual'
        mb.member_functions('Close').virtuality = 'virtual'
        mb.member_functions('CloseModal').virtuality = 'virtual'
        mb.member_functions('SetMoveable').virtuality = 'virtual'
        mb.member_functions('SetSizeable').virtuality = 'virtual'
        mb.member_functions('SetMenuButtonVisible').virtuality = 'virtual'
        mb.member_functions('SetMinimizeButtonVisible').virtuality = 'virtual'
        mb.member_functions('SetMaximizeButtonVisible').virtuality = 'virtual'
        mb.member_functions('SetMinimizeToSysTrayButtonVisible').virtuality = 'virtual'
        mb.member_functions('SetCloseButtonVisible').virtuality = 'virtual'
        mb.member_functions('FlashWindow').virtuality = 'virtual'
        mb.member_functions('FlashWindowStop').virtuality = 'virtual'
        mb.member_functions('SetTitleBarVisible').virtuality = 'virtual'
        mb.member_functions('SetClipToParent').virtuality = 'virtual'
        mb.member_functions('SetSmallCaption').virtuality = 'virtual'
        mb.member_functions('DoModal').virtuality = 'virtual'
        mb.member_functions('OnClose').virtuality = 'virtual'
        mb.member_functions('OnFinishedClose').virtuality = 'virtual'
        mb.member_functions('OnMinimize').virtuality = 'virtual'
        mb.member_functions('OnMinimizeToSysTray').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        mb.member_functions('GetDefaultScreenPosition').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        mb.member_functions('OnCloseFrameButtonPressed').virtuality = 'virtual'
        
        mb.member_functions('SetDeleteSelfOnClose').exclude()
        mb.member_functions('GetSysMenu').exclude()
        
        mb.member_functions( 'GetDefaultScreenPosition' ).add_transformation( FT.output('x'), FT.output('y'), FT.output('wide'), FT.output('tall') ) 
        mb.member_functions( 'GetClientArea' ).add_transformation( FT.output('x'), FT.output('y'), FT.output('wide'), FT.output('tall') ) 

    def ScrollBar(self, mb):
        mb.member_functions( 'GetButton' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        mb.member_functions( 'GetSlider' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
        mb.member_functions( 'GetRange' ).add_transformation( FT.output('min'), FT.output('max') ) 

        # ScrollBarSlider
        mb.member_functions( 'GetNobPos' ).add_transformation( FT.output('min'), FT.output('max') ) 
    
    def ParseAnimationController(self, mb):
        # Make empty, don't care
        #self.IncludeEmptyClass(mb, 'AnimationController')
        pass
        
    
    def ParseTextEntry(self, mb):
        # List of overridables
        mb.member_functions('SetText').virtuality = 'virtual'
        mb.member_functions('MoveCursor').virtuality = 'virtual'
        mb.member_functions('SetDisabledBgColor').virtuality = 'virtual'
        mb.member_functions('SetMultiline').virtuality = 'virtual'
        mb.member_functions('SetVerticalScrollbar').virtuality = 'virtual'

        #
        mb.member_functions('GetEditMenu').exclude()        # Exclude for now, add back later when we found out call policies.
        
        mb.member_functions( 'GetSelectedRange' ).add_transformation( FT.output('cx0'), FT.output('cx1') )   
        mb.member_functions( 'CursorToPixelSpace' ).add_transformation( FT.inout('cx'), FT.inout('cy') ) 
        mb.member_functions( 'AddAnotherLine' ).add_transformation( FT.output('cx'), FT.output('cy') ) 
        mb.member_functions( 'GetStartDrawIndex' ).add_transformation( FT.output('lineBreakIndexIndex') )
        
        charstrexcludetypes = [pointer_t(const_t(declarated_t(char_t())))]
        
        # Wrap GetText manual
        cls = mb.class_('TextEntry')
        cls.member_functions('GetText').exclude()
        cls.add_wrapper_code(
            'boost::python::object GetText() {\r\n' + \
            '    const char *buf = (const char *)malloc( (GetTextLength()+1)*sizeof(char) );\r\n' + \
            '    TextEntry::GetText((char *)buf, GetTextLength()+1);\r\n' + \
            '    boost::python::object rv(buf);\r\n' + \
            '    delete buf;\r\n' + \
            '    return rv;\r\n' + \
            '}'
        )
        
        cls.add_registration_code(
            'def( \r\n' + \
            '    "GetText"\r\n' + \
            '    , (boost::python::object ( TextEntry_wrapper::* )())( &TextEntry_wrapper::GetText ) )'
        )
        
        cls.calldefs(name='SetText', function=calldef_withtypes(charstrexcludetypes)).exclude()
        cls.calldefs(name='InsertString', function=calldef_withtypes(charstrexcludetypes)).exclude()

        # RichText
        cls = mb.class_('RichText')
        cls.calldefs(name='SetText', function=calldef_withtypes(charstrexcludetypes)).exclude()
        cls.calldefs(name='InsertString', function=calldef_withtypes(charstrexcludetypes)).exclude()
        if self.settings.branch == 'swarm':
            mb.member_functions('GetScrollBar').exclude()
        
    def TestBasePanel(self, cls):
        basepanelcls = self.basepanelcls
        recursive_bases = cls.recursive_bases
        for testcls in recursive_bases:
            if basepanelcls == testcls.related_class:
                return True
        return cls == basepanelcls
        
    def Parse(self, mb):
        self.novguilib = (self.settings.branch == 'swarm')
        
        # Exclude everything by default
        mb.decls().exclude()  

        self.ParsePanelHandles(mb)
        self.ParsePanels(mb)
        
        self.ParsePanel(mb)
        self.ParseEditablePanel(mb)
        self.ParseFrame(mb)
        self.ScrollBar(mb)
        self.ParseAnimationController(mb)
        self.ParseTextEntry(mb)
        #self.ParseMisc(mb)
        
        # Should already be included, but is for some reason not...
        mb.member_functions('SetControlEnabled').include()
        
        # Anything that returns a Panel should be returned by Value to call the right converter
        testinherit = MatcherTestInheritClass(mb.class_('Panel'))
        decls = mb.calldefs(matchers.custom_matcher_t(testinherit))
        decls.call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
        # All CBaseEntity related classes should have a custom call trait
        self.basepanelcls = mb.class_('Panel')
        def panel_call_trait(type_):
            return 'boost::python::object(*%(arg)s)'
        panelclasses = mb.classes(self.TestBasePanel)
        for panelclass in panelclasses:
            panelclass.custom_call_trait = panel_call_trait
        
        # Remove any protected function
        #mb.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        
        self.ParseImageClasses(mb)
        
        self.ApplyCommonRules(mb)
