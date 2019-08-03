from srcpy.module_generators import ClientModuleGenerator, ClientReqFile

from pyplusplus import function_transformers as FT
from pyplusplus.module_builder import call_policies
from pygccxml.declarations import matchers

class VGUI(ClientModuleGenerator):
    module_name = '_vgui'

    required_files = [
        ClientReqFile('$SRCDIR\game\client\python\srcpy_hud.cpp'),
        ClientReqFile('$SRCDIR\game\client\python\srcpy_hud.h'),
        ClientReqFile('$SRCDIR\game\client\python\srcpy_vgui.cpp'),
        ClientReqFile('$SRCDIR\game\client\python\srcpy_vgui.h'),
    ]

    module_symbols = ['SRCPY_MOD_VGUI']

    files = [
        'cbase.h',
        
        'vgui_controls/Controls.h',
        'vgui/Cursor.h',
        
        'vgui_controls/Panel.h',
        'view_shared.h',
        'vgui/IBorder.h',
        'vgui_controls/AnimationController.h',
        
        'vgui/IInput.h',
        'vgui/ISystem.h',
        'vgui/IScheme.h',
        'vgui/ILocalize.h',
        'iclientmode.h',
        'vgui_controls/MessageMap.h',
        'ienginevgui.h',
        'hud.h',
        'hudelement.h',
        
        'srcpy_vgui.h',
        'srcpy_hud.h',
    ]
    
    def ParseInterfaces(self, mb):
        # ISchemeManager
        mb.free_function('scheme').include()
        mb.free_function('scheme').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        
        cls = mb.class_('ISchemeManager')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        cls.member_functions( 'GetImage' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        cls.member_functions( 'GetIScheme' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        #cls.member_functions( 'GetBorder' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        
        if self.settings.branch == 'swarm':
            cls.member_functions('GetSurface').exclude()
        
        # IScheme
        cls = mb.class_('IScheme')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        cls.member_functions('GetBorder').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        if self.settings.branch == 'source2013':
            cls.member_functions('GetBorderAtIndex').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)

        if self.settings.branch == 'swarm':
            cls.class_('fontalias_t').exclude()
        
        # ILocalize
        cls = mb.class_('PyLocalize')
        cls.rename('Localize')
        cls.include()
        cls.member_function('Find').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_function('GetValueByIndex').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        if self.settings.branch == 'swarm':
            mb.variable('INVALID_STRING_INDEX').include()
        
        #mb.variable('g_pylocalize').include()
        #mb.variable('g_pylocalize').rename('localize')
        mb.add_registration_code( "bp::scope().attr( \"localize\" ) = boost::ref(g_pylocalize);" )
        
        # IPanel
        cls = mb.class_('CWrapIPanel')
        cls.include()
        cls.rename('IPanel')    
        cls.member_functions( 'GetPos' ).add_transformation( FT.output('x'), FT.output('y') )
        cls.member_functions( 'GetSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetAbsPos' ).add_transformation( FT.output('x'), FT.output('y') )
        if self.settings.branch == 'swarm':
            cls.member_functions( 'Plat' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        cls.member_functions( 'GetPanel' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        
        mb.free_function('wrapipanel').include()
        mb.free_function( 'wrapipanel' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
        mb.free_function( 'wrapipanel' ).rename('ipanel')   

        # VGUI Input
        cls = mb.class_('IInput')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        #cls.member_functions('GetCursorPos').add_transformation( FT.output('x'), FT.output('y') )
        #cls.member_functions('GetCursorPosition').add_transformation( FT.output('x'), FT.output('y') )
        #cls.member_functions('GetIMEWindow').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_function('GetIMEWindow').exclude()
        cls.member_function('SetIMEWindow').exclude()
        cls.member_function('GetIMEConversionModes').exclude()
        cls.member_function('GetIMELanguageList').exclude()
        cls.member_function('GetIMELanguageShortCode').exclude()
        cls.member_function('GetIMESentenceModes').exclude()
        
        cls.classes().exclude()
        
        mb.free_function('input').include()
        mb.free_function('input').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
        mb.free_function('input').rename('vgui_input')
        
        mb.free_function('PyInput_GetCursorPos').include()
        mb.free_function('PyInput_GetCursorPosition').include()
        
        # ISystem
        cls = mb.class_('ISystem')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        cls.member_functions('GetUserConfigFileData').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('GetRegistryInteger').add_transformation(FT.output('value'))
        
        # Exclude registry modification functions, likely not needed from Python
        cls.member_functions('SetRegistryInteger').exclude()
        cls.member_functions('SetRegistryString').exclude()
        cls.member_functions('DeleteRegistryKey').exclude()
        
        cls.member_function('SetClipboardImage').exclude()
        
        # Properly wrap getting a registery string
        mb.add_declaration_code('''static boost::python::tuple GetRegistryString_cc10e70c5f6b49d5963b27442c970b19( ::vgui::ISystem & inst, char const * key ){
    char value2[512];
    bool result = inst.GetRegistryString(key, value2, sizeof(value2));
    return bp::make_tuple( result, value2 );
}
        ''')
        cls.member_functions('GetRegistryString').exclude()
        cls.add_registration_code('''def( 
            "GetRegistryString"
            , (boost::python::tuple (*)( ::vgui::ISystem &,char const * ))( &GetRegistryString_cc10e70c5f6b49d5963b27442c970b19 )
            , ( bp::arg("inst"), bp::arg("key") ) )
        ''')
        
        vgui = mb.namespace('vgui')
        vgui.free_function('system').include()
        vgui.free_function('system').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
        vgui.free_function('system').rename('vgui_system')
        
        # IClientMode
        cls = mb.class_( 'IClientMode' )
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        
        if self.settings.branch == 'swarm':
            mb.add_declaration_code( "IClientMode *wrap_GetClientMode( void )\r\n{\r\n\treturn GetClientMode();\r\n}\r\n" )
            mb.add_registration_code( 'bp::def( "GetClientMode", wrap_GetClientMode, bp::return_value_policy<bp::reference_existing_object>() );' )
            
        if self.settings.branch == 'source2013':
            # TODO: Returns wchar_t *. Get a converter.
            cls.member_function('GetMapName').exclude()
            cls.member_function('GetServerName').exclude()
        
        cls.member_functions( 'GetViewport' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        cls.member_functions( 'GetViewportAnimationController' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        cls.member_functions( 'GetMessagePanel' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        cls.member_functions( 'AdjustEngineViewport' ).add_transformation( FT.output('x'), FT.output('y'), FT.output('width'), FT.output('height') )
        cls.member_functions( 'ActivateInGameVGuiContext' ).include()  # Not safe, but IClientMode should not be overridden.
        
        if self.settings.branch == 'swarm':
            cls.member_functions( 'GetPanelFromViewport' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        
    def ParseISurface(self, mb):
        cls = mb.class_('CWrapSurface')
        cls.include()
        cls.rename('ISurface')
        mb.free_function('wrapsurface').include()
        mb.free_function( 'wrapsurface' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
        mb.free_function( 'wrapsurface' ).rename('surface')    
        
        mb.enumeration('CursorCode').include()
        mb.enumeration('FontDrawType_t').include()
        if self.settings.branch == 'swarm':
            mb.class_('FontVertex_t').include()
        else:
            mb.class_('Vertex_t').include()
        mb.class_('IntRect').include()
        
        cls.member_functions( 'DrawGetTextPos' ).add_transformation( FT.output('x'), FT.output('y') )
        cls.member_functions( 'DrawGetTextureSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetScreenSize' ).add_transformation( FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetCharABCwide' ).add_transformation( FT.output('a'), FT.output('b'), FT.output('c') )
        cls.member_functions( 'GetWorkspaceBounds' ).add_transformation( FT.output('x'), FT.output('y'), FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetAbsoluteWindowBounds' ).add_transformation( FT.output('x'), FT.output('y'), FT.output('wide'), FT.output('tall') )
        cls.member_functions( 'GetProportionalBase' ).add_transformation( FT.output('width'), FT.output('height') )
        cls.member_functions( 'SurfaceGetCursorPos' ).add_transformation( FT.output('x'), FT.output('y') )
        
        cls.member_functions( 'CreateHTMLWindow' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
        cls.member_functions( 'DrawGetTextureMatInfoFactory' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
        cls.member_functions( 'GetIconImageForFullPath' ).call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
    
    def ParseHUD(self, mb):
        # CHud
        cls = mb.class_('CHud')
        cls.include()
        
        if self.settings.branch == 'swarm':
            mb.free_function('GetHud').include()
            mb.free_function('GetHud').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        else:
            mb.variables('gHUD').include()
        
        cls.member_functions( 'FindElement' ).call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        if self.settings.branch != 'swarm': # ASW should use HudIcons() / CHudIcons
            #cls.member_functions( 'GetIcon' ).call_policies = call_policies.return_value_policy( call_policies.manage_new_object )
            #cls.member_functions( 'AddUnsearchableHudIconToList' ).call_policies = call_policies.return_value_policy( call_policies.manage_new_object ) 
            #cls.member_functions( 'AddSearchableHudIconToList' ).call_policies = call_policies.return_value_policy( call_policies.manage_new_object )
            # The HUD only cleans up when you close the game, so internal references should't be a problem.
            cls.member_functions( 'GetIcon' ).call_policies = call_policies.return_internal_reference()
            cls.member_functions( 'AddUnsearchableHudIconToList' ).call_policies = call_policies.return_internal_reference()
            cls.member_functions( 'AddSearchableHudIconToList' ).call_policies = call_policies.return_internal_reference()
        cls.variables('m_HudList').exclude()
        
        if self.settings.branch == 'swarm':
            cls.member_functions('GetHudList').exclude()
            cls.member_functions('GetHudPanelList').exclude()
            
        cls = mb.class_('CHudElement')
        cls.include()
        cls.member_functions('GetName').exclude()
        cls.variables('m_pyInstance').exclude()
        cls.member_functions('SetActive').virtuality = 'not virtual' 
        cls.member_functions('IsActive').virtuality = 'not virtual'
        cls.member_functions('ShouldDraw').virtuality = 'not virtual' # TODO: What if we need better control in Python?
        cls.member_functions('ProcessInput').virtuality = 'not virtual' # TODO: Do we ever need this in Python?
        
        cls = mb.class_('CPyHudElementHelper')
        cls.include()
        cls.rename('CHudElementHelper')
        cls.member_functions('GetNext').exclude()

        # HudIcons
        if self.settings.branch == 'swarm':
            cls = mb.class_('CHudIcons')
            cls.include()
            # FIXME
            cls.member_functions( 'GetIcon' ).call_policies = call_policies.return_internal_reference()
            #cls.member_functions('GetIcon').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
            #cls.member_functions('GetIcon' ).call_policies = call_policies.return_value_policy( call_policies.manage_new_object ) 
            cls.member_functions('AddUnsearchableHudIconToList').exclude()
            cls.member_functions('AddSearchableHudIconToList').exclude()
            #cls.member_functions('AddUnsearchableHudIconToList').call_policies = call_policies.return_value_policy( call_policies.manage_new_object ) 
            #cls.member_functions('AddSearchableHudIconToList').call_policies = call_policies.return_value_policy( call_policies.manage_new_object ) 
            mb.free_function('HudIcons').include()
            mb.free_function('HudIcons').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object) 
            
        # CHudTexture
        cls = mb.class_('CHudTexture')
        cls.include()
        cls.variables().exclude()
        
    def ParseMisc(self, mb):
        # Include some empty classes
        #IncludeEmptyClass(mb, 'CViewSetup')    <- TODO: Needed?
        
        # Animation Controller
        mb.free_function('GetAnimationController').include()
        mb.free_function('GetAnimationController').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)

        # IVGUI/Tick signals
        mb.add_declaration_code( "void wrap_AddTickSignal( VPANEL panel, int intervalMilliseconds = 0 )\r\n{\r\n\tvgui::ivgui()->AddTickSignal(panel, intervalMilliseconds);\r\n}\r\n" )
        mb.add_declaration_code( "void wrap_RemoveTickSignal( VPANEL panel)\r\n{\r\n\tvgui::ivgui()->RemoveTickSignal(panel);\r\n}\r\n" )
        mb.add_registration_code( 'bp::def( "AddTickSignal", wrap_AddTickSignal, (bp::arg("panel"), bp::arg("intervalMilliseconds")=0 ) );' )
        mb.add_registration_code( 'bp::def( "RemoveTickSignal", wrap_RemoveTickSignal, bp::arg("panel") );' )
        
        # Gameui open?
        mb.free_function('PyIsGameUIVisible').include()
        mb.free_function('PyIsGameUIVisible').rename('IsGameUIVisible')
        mb.free_function('PyGetPanel').include()
        mb.free_function('PyGetPanel').rename('GetPanel')
        #mb.free_function('PyGetPanel').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        mb.enumeration('VGuiPanel_t').include()
        
        # Message map types
        mb.enumeration('DataType_t').include()

        # Some defines
        mb.add_registration_code( "bp::scope().attr( \"INVALID_FONT\" ) = (int)0;" )
        mb.add_registration_code( "bp::scope().attr( \"INVALID_PANEL\" ) = (int)0xffffffff;" )
        
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()  

        self.ParseInterfaces(mb)
        self.ParseISurface(mb)
        self.ParseHUD(mb)
        self.ParseMisc(mb)
        
        # Remove any protected function 
        mb.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        
        self.ApplyCommonRules(mb)
