from srcpy.module_generators import SemiSharedModuleGenerator
from srcpy.matchers import calldef_withtypes

from pyplusplus.module_builder import call_policies
from pyplusplus import code_creators
from pygccxml.declarations import matchers, pointer_t, reference_t, declarated_t

class GameInterface(SemiSharedModuleGenerator):
    module_name = '_gameinterface'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_gameinterface.cpp',
        '$SRCDIR\game\shared\python\srcpy_gameinterface.h',
        '$SRCDIR\game\shared\python\srcpy_gameinterface_converters.h',
        '$SRCDIR\game\shared\python\srcpy_usermessage.cpp',
        '$SRCDIR\game\shared\python\srcpy_usermessage.h',
    ]

    module_symbols = ['SRCPY_MOD_GAMEINTERFACE']
    
    files = [
        'cbase.h',
        
        '#mathlib/vmatrix.h', 
        '#utlvector.h', 
        '#shareddefs.h',
        '#util.h',
        '#iservernetworkable.h',
        '#recipientfilter.h',
        '#srcpy_usermessage.h',
        '#mapentities.h',
        'saverestoretypes.h',
        
        '$gamerules.h',
        '$multiplay_gamerules.h',
        '$teamplay_gamerules.h',
        '$c_recipientfilter.h',
        
        'tier0/icommandline.h',
        'convar.h',
        'igameevents.h',
        'irecipientfilter.h',
        'srcpy_gameinterface.h',
        'cdll_int.h',
        '#%team.h',
        '$%c_team.h',
        '$clientsteamcontext.h',
    ]

    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude() 
        
        # POSIX compiler model_t fix ( ok to do? maybe find another fix )
        mb.add_declaration_code('#ifdef POSIX\r\n' +
                             'typedef struct model_t {};\r\n' +
                             '#endif // POSIX\r\n'
                                )
                           
        # Filesystem functions
        mb.free_function('PyGetModPath').include()
        mb.free_function('PyGetModPath').rename('GetModPath')
        mb.free_function('PyAsyncFinishAllWrites').include()
        mb.free_function('PyAsyncFinishAllWrites').rename('AsyncFinishAllWrites')
        
        # Engine functions
        mb.free_function('GetLevelName').include()    
        
        # Time
        mb.free_function('Plat_FloatTime').include()
        mb.free_function('Plat_MSTime').include()
        
        # Precache functions
        mb.free_function('PrecacheMaterial').include()
        if self.settings.branch == 'swarm':
            mb.free_function('PrecacheEffect').include()
        
        # ConVar wrapper
        cls = mb.class_('PyConVar')
        cls.include()
        cls.rename('ConVar')
        cls.member_functions().virtuality = 'not virtual'
        cls.member_function('Shutdown').exclude()
        
        mb.free_function('PyShutdownConVar').include()
        mb.free_function('PyShutdownConCommand').include()

        # Don't want to include ConVar, so add methods manually...
        # Can't this be done automatically in py++?
        cls.add_registration_code(
            '''def( 
            "AddFlags"
            , (void ( ::ConVar::* )( int ) )( &::ConVar::AddFlags )
            , ( bp::arg("flags") ) )''')
        cls.add_registration_code(
            '''def( 
            "GetBool"
            , (bool ( ::ConVar::* )(  ) const)( &::ConVar::GetBool ) )''')
        cls.add_registration_code(
            '''def( 
            "GetDefault"
            , (char const * ( ::ConVar::* )(  ) const)( &::ConVar::GetDefault ) )''')
        cls.add_registration_code(
            '''def( 
            "GetFloat"
            , (float ( ::ConVar::* )(  ) const)( &::ConVar::GetFloat ) )''')
        cls.add_registration_code(
            '''def( 
            "GetHelpText"
            , (char const * ( ::ConVar::* )(  ) const)( &::ConVar::GetHelpText ) )''')
        cls.add_registration_code(
            '''def( 
            "GetInt"
            , (int ( ::ConVar::* )(  ) const)( &::ConVar::GetInt ) )''')
        cls.add_registration_code(
            '''def( 
            "GetMax"
            , (bool ( ::ConVar::* )( float & ) const)( &::ConVar::GetMax )
            , ( bp::arg("maxVal") ) )''')
        cls.add_registration_code(
            '''def( 
            "GetMin"
            , (bool ( ::ConVar::* )( float & ) const)( &::ConVar::GetMin )
            , ( bp::arg("minVal") ) )''')
        cls.add_registration_code(
            '''def( 
            "GetName"
            , (char const * ( ::ConVar::* )(  ) const)( &::ConVar::GetName ) )''')
        cls.add_registration_code(
            '''def( 
            "GetString"
            , (char const * ( ::ConVar::* )(  ) const)( &::ConVar::GetString ) )''')
        cls.add_registration_code(
            '''def( 
            "IsCommand"
            , (bool ( ::ConVar::* )(  ) const)( &::ConVar::IsCommand ) )''')
        cls.add_registration_code(
            '''def( 
            "IsFlagSet"
            , (bool ( ::ConVar::* )( int ) const)( &::ConVar::IsFlagSet )
            , ( bp::arg("flag") ) )''')
        cls.add_registration_code(
            '''def( 
            "IsRegistered"
            , (bool ( ::ConVar::* )(  ) const)( &::ConVar::IsRegistered ) )''')
        cls.add_registration_code(
            '''def( 
            "Revert"
            , (void ( ::ConVar::* )(  ) )( &::ConVar::Revert ) )''')

        # ConVarRef
        mb.class_('ConVarRef').include()
        mb.member_functions('GetLinkedConVar').exclude()
        
        # CCommand
        cls = mb.class_('CCommand')
        cls.include()
        cls.member_functions('Tokenize').exclude()
        cls.member_functions('ArgV').exclude()
        cls.member_functions('DefaultBreakSet').exclude()
        
        cls.add_registration_code(
         '''def( 
            "__len__"
            , (int ( ::CCommand::* )(  ) const)( &::CCommand::ArgC ) )''')
        
        # PyConCommand
        cls = mb.class_('PyConCommand')
        cls.include()
        cls.rename('ConCommand')
        cls.variable('m_pyCommandCallback').exclude()         # Must be excluded, or else things get broken without errors/warnings!
        cls.member_functions('Dispatch').exclude()               # Must be excluded, or else things get broken without errors/warnings!
        cls.member_functions('AutoCompleteSuggest').exclude()
        
        # Virtuality screws up ConCommand. 
        cls.member_functions().virtuality = 'not virtual' 
        
        # Sending messages
        if self.isserver:
            mb.free_functions('PySendUserMessage').include()
            mb.free_functions('PySendUserMessage').rename('SendUserMessage')
            
        # filters
        mb.class_('IRecipientFilter').include()
        mb.class_('IRecipientFilter').member_functions().virtuality = 'not virtual' 
        if self.isserver:
            mb.class_('CRecipientFilter').include()
            mb.class_('CRecipientFilter').member_functions().virtuality = 'not virtual' 
        else:
            mb.class_('C_RecipientFilter').include()
            mb.class_('C_RecipientFilter').member_functions().virtuality = 'not virtual' 
            
            mb.class_('CLocalPlayerFilter').include()
            #mb.class_('CLocalPlayerFilter').member_functions().virtuality = 'not virtual' 
            
        # Shared filters
        mb.class_('CSingleUserRecipientFilter').include()
        mb.class_('CBroadcastRecipientFilter').include()
        mb.class_('CReliableBroadcastRecipientFilter').include()
        mb.class_('CPASFilter').include()
        mb.class_('CPASAttenuationFilter').include()
        mb.class_('CPVSFilter').include()
            
        # Gameevents
        mb.class_('PyGameEventListener').include()
        mb.class_('PyGameEventListener').rename('GameEventListener')
        mb.class_('PyGameEventListener').member_function('PyFireGameEvent').rename('FireGameEvent')
        mb.class_('PyGameEventListener').add_registration_code('def( "ListenForGameEvent", (void ( ::PyGameEventListener::* )( char const * ) )( &::PyGameEventListener::ListenForGameEvent ), bp::arg("name") ) ')
        mb.class_('PyGameEventListener').add_registration_code('def( "StopListeningForAllEvents", (void ( ::PyGameEventListener::* )() )( &::PyGameEventListener::StopListeningForAllEvents ) ) ')
        
        mb.class_('PyGameEvent').include()
        mb.class_('PyGameEvent').rename('GameEvent')
        mb.class_('PyGameEvent').member_function('Init').exclude()
        mb.class_('PyGameEvent').member_function('GetEvent').exclude()
        
        if self.isserver:
            mb.free_function('PyFireGameEvent').include()
            mb.free_function('PyFireGameEvent').rename('FireGameEvent')
        else:
            mb.free_function('PyFireGameEventClientSide').include()
            mb.free_function('PyFireGameEventClientSide').rename('FireGameEventClientSide')
                
        # Player info
        mb.class_('py_player_info_s').include()
        mb.class_('py_player_info_s').rename('PlayerInfo')
        
        # Get map header
        mb.free_function('PyGetMapHeader').include()
        mb.free_function('PyGetMapHeader').rename('GetMapHeader')
        if self.settings.branch == 'swarm':
            cls = mb.class_('BSPHeader_t')
        else:
            cls = mb.class_('dheader_t')
        cls.include()
        if self.settings.branch == 'swarm':
            cls.variable('m_nVersion').rename('version')
        cls.variable('mapRevision').rename('maprevision')
        
        mb.class_('lump_t').include()
        mb.member_functions('GetBaseMap').exclude()
        mb.variables('m_DataMap').exclude()
        
        # Content mounting
        mb.free_function('PyAddSearchPath').include()
        mb.free_function('PyAddSearchPath').rename('AddSearchPath')
        mb.free_function('PyRemoveSearchPath').include()
        mb.free_function('PyRemoveSearchPath').rename('RemoveSearchPath')
        mb.free_function('PyGetSearchPath').include()
        mb.free_function('PyGetSearchPath').rename('GetSearchPath')
        
        mb.enumeration('SearchPathAdd_t').include()
        mb.enumeration('FilesystemMountRetval_t').include()
        
        # GameSystem
        mb.class_('CBaseGameSystem').include()
        mb.class_('CBaseGameSystemPerFrame').include()
        mb.class_('CAutoGameSystem').include()
        mb.class_('CAutoGameSystemPerFrame').include()
        mb.member_functions('IsPerFrame').virtuality = 'not virtual' 
        mb.member_functions('SafeRemoveIfDesired').virtuality = 'not virtual'
        mb.class_('CAutoGameSystem').rename('AutoGameSystem')
        mb.class_('CAutoGameSystemPerFrame').rename('AutoGameSystemPerFrame')
        
        cls = mb.class_('IGameSystem')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        if self.isserver:
            cls.member_functions('RunCommandPlayer').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            cls.member_functions('RunCommandUserCmd').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            
        # Engine
        if self.isserver:
            cls = mb.class_('PyVEngineServer')
            cls.rename('VEngineServer')
            cls.include()
        else:
            cls = mb.class_('PyVEngineClient')
            cls.rename('VEngineClient')
            cls.include()
            cls.member_functions('LoadModel').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        mb.add_registration_code( "bp::scope().attr( \"engine\" ) = boost::ref(pyengine);" )   
        
        # Command line access
        cls = mb.class_('ICommandLine')
        cls.include()
        cls.member_functions().virtuality = 'not virtual'
        
        if self.settings.branch == 'swarm':
            mb.free_function('CommandLine').include()
            mb.free_function('CommandLine').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        else:
            mb.free_function('CommandLine_Tier0').include()
            mb.free_function('CommandLine_Tier0').rename('CommandLine')
            mb.free_function('CommandLine_Tier0').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
            
        # Accessing model info
        cls = mb.class_('PyVModelInfo')
        cls.rename('VModelInfo')
        cls.include()
        cls.member_functions('GetModel').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        cls.member_functions('FindOrLoadModel').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        mb.add_registration_code( "bp::scope().attr( \"modelinfo\" ) = boost::ref(pymodelinfo);" )   
        
        if self.isserver:
            mb.free_function('PyMapEntity_ParseAllEntities').include()
            mb.free_function('PyMapEntity_ParseAllEntities').rename('MapEntity_ParseAllEntities')
            
            cls = mb.class_('IMapEntityFilter')
            cls.include()
            cls.member_function('CreateNextEntity').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
            
            mb.class_('CMapEntityRef').include()
            mb.free_function('PyGetMapEntityRef').include()
            mb.free_function('PyGetMapEntityRef').rename('GetMapEntityRef')
            mb.free_function('PyGetMapEntityRefIteratorHead').include()
            mb.free_function('PyGetMapEntityRefIteratorHead').rename('GetMapEntityRefIteratorHead')
            mb.free_function('PyGetMapEntityRefIteratorNext').include()
            mb.free_function('PyGetMapEntityRefIteratorNext').rename('GetMapEntityRefIteratorNext')
            
            # Edicts
            cls = mb.class_('edict_t')
            cls.include()
            cls.member_function('GetCollideable').exclude()
            
            # Event queue
            mb.free_function('ServiceEventQueue').include()
            
            cls = mb.class_('CServerGameDLL')
            cls.include()
            cls.member_functions().virtuality = 'not virtual'
            cls.member_function('SaveReadFields').exclude()
            cls.member_function('SaveWriteFields').exclude()
            
            if self.settings.branch == 'swarm':
                cls.member_function('FindLaunchOptionByValue').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
            if self.settings.branch == 'source2013':
                cls.member_function('GetServerGCLobby').exclude()
            cls.member_function('GetAllServerClasses').exclude()
            cls.member_function('GetStandardSendProxies').exclude()
            cls.member_function('SaveInit').exclude()
            
            mb.add_declaration_code('extern CServerGameDLL g_ServerGameDLL;')
            mb.add_registration_code("bp::scope().attr( \"servergamedll\" ) = boost::ref(g_ServerGameDLL);")
            
        if self.isclient:
            cls = mb.class_('CClientSteamContext')
            cls.include()
            cls.member_function('InstallCallback').exclude()
            cls.member_function('RemoveCallback').exclude()
            cls.member_function('Activate').exclude()
            cls.member_function('Shutdown').exclude()
            
            mb.free_function('ClientSteamContext').include()
            mb.free_function('ClientSteamContext').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
            
        # model_t
        cls = mb.class_('wrap_model_t')
        cls.include()
        cls.rename('model_t')
        cls.calldefs('wrap_model_t').exclude()
        cls.no_init = True
        cls.variables('pModel').exclude()

        mb.add_registration_code( "ptr_model_t_to_wrap_model_t();" )
        mb.add_registration_code( "const_ptr_model_t_to_wrap_model_t();" )
        mb.add_registration_code( "wrap_model_t_to_model_t();" )
        
        # LUMPS
        mb.add_registration_code( "bp::scope().attr( \"LUMP_ENTITIES\" ) = (int)LUMP_ENTITIES;" )
        
        # Defines
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_NONE\" ) = (int)FCVAR_NONE;" )
        
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_UNREGISTERED\" ) = (int)FCVAR_UNREGISTERED;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_DEVELOPMENTONLY\" ) = (int)FCVAR_DEVELOPMENTONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_GAMEDLL\" ) = (int)FCVAR_GAMEDLL;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_CLIENTDLL\" ) = (int)FCVAR_CLIENTDLL;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_HIDDEN\" ) = (int)FCVAR_HIDDEN;" )
        
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_PROTECTED\" ) = (int)FCVAR_PROTECTED;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_SPONLY\" ) = (int)FCVAR_SPONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_ARCHIVE\" ) = (int)FCVAR_ARCHIVE;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_NOTIFY\" ) = (int)FCVAR_NOTIFY;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_USERINFO\" ) = (int)FCVAR_USERINFO;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_CHEAT\" ) = (int)FCVAR_CHEAT;" )
        
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_PRINTABLEONLY\" ) = (int)FCVAR_PRINTABLEONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_UNLOGGED\" ) = (int)FCVAR_UNLOGGED;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_NEVER_AS_STRING\" ) = (int)FCVAR_NEVER_AS_STRING;" )
        
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_REPLICATED\" ) = (int)FCVAR_REPLICATED;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_DEMO\" ) = (int)FCVAR_DEMO;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_DONTRECORD\" ) = (int)FCVAR_DONTRECORD;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_NOT_CONNECTED\" ) = (int)FCVAR_NOT_CONNECTED;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_ARCHIVE_XBOX\" ) = (int)FCVAR_ARCHIVE_XBOX;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_SERVER_CAN_EXECUTE\" ) = (int)FCVAR_SERVER_CAN_EXECUTE;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_SERVER_CANNOT_QUERY\" ) = (int)FCVAR_SERVER_CANNOT_QUERY;" )
        mb.add_registration_code( "bp::scope().attr( \"FCVAR_CLIENTCMD_CAN_EXECUTE\" ) = (int)FCVAR_CLIENTCMD_CAN_EXECUTE;" )
        
        # Excludes
        excludetypes = [
            pointer_t(declarated_t(mb.class_('CTeam' if self.isserver else 'C_Team'))),
        ]
        mb.calldefs( calldef_withtypes( excludetypes ) ).exclude()
    
    def AddAdditionalCode(self, mb):
        header = code_creators.include_t( 'srcpy_gameinterface_converters.h' )
        mb.code_creator.adopt_include(header)
        super(GameInterface, self).AddAdditionalCode(mb)
