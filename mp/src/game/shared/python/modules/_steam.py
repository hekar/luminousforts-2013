import re

from srcpy.module_generators import SemiSharedModuleGenerator

from pyplusplus import function_transformers as FT
from pyplusplus.module_builder import call_policies
from pygccxml.declarations import calldef_matcher_t, pointer_t, const_t, declarated_t, char_t

# Templates for the most common callback cases
callback_wrapper_tmpl = '''struct %(name)sCallback_wrapper : %(name)sCallback, bp::wrapper< %(name)sCallback > {

    %(name)sCallback_wrapper()
    : %(name)sCallback()
      , bp::wrapper< %(name)sCallback >(){
        // constructor
    
    }

    virtual void On%(name)s( ::%(dataclass)s * pData ) {
        PY_OVERRIDE_CHECK( %(name)sCallback, On%(name)s )
        PY_OVERRIDE_LOG( _steam, %(name)sCallback, On%(name)s )
        bp::override func_On%(name)s = this->get_override( "On%(name)s" );
        if( func_On%(name)s.ptr() != Py_None )
            try {
                func_On%(name)s( boost::python::ptr(pData) );
            } catch(bp::error_already_set &) {
                PyErr_Print();
                this->%(name)sCallback::On%(name)s( pData );
            }
        else
            this->%(name)sCallback::On%(name)s( pData );
    }
    
    void default_On%(name)s( ::%(dataclass)s * pData ) {
        %(name)sCallback::On%(name)s( pData );
    }
};
'''

callback_reg_tmpl = '''{ //::%(name)sCallback
        typedef bp::class_< %(name)sCallback_wrapper, boost::noncopyable > %(name)sCallback_exposer_t;
        %(name)sCallback_exposer_t %(name)sCallback_exposer = %(name)sCallback_exposer_t( "%(name)sCallback", bp::init<>() );
        bp::scope %(name)sCallback_scope( %(name)sCallback_exposer );
        { //::%(name)sCallback::On%(name)s
        
            typedef void ( ::%(name)sCallback::*On%(name)s_function_type )( ::%(dataclass)s * ) ;
            typedef void ( %(name)sCallback_wrapper::*default_On%(name)s_function_type )( ::%(dataclass)s * ) ;
            
            %(name)sCallback_exposer.def( 
                "On%(name)s"
                , On%(name)s_function_type(&::%(name)sCallback::On%(name)s)
                , default_On%(name)s_function_type(&%(name)sCallback_wrapper::default_On%(name)s)
                , ( bp::arg("data") ) );
        
        }
    }
'''

callresult_wrapper_tmpl = '''struct %(name)sCallResult_wrapper : %(name)sCallResult, bp::wrapper< %(name)sCallResult > {

    %(name)sCallResult_wrapper(::SteamAPICall_t steamapicall )
    : %(name)sCallResult( steamapicall )
      , bp::wrapper< %(name)sCallResult >(){
        // constructor
    }

    virtual void On%(name)s( ::%(dataclass)s * pData, bool bIOFailure ) {
        PY_OVERRIDE_CHECK( %(name)sCallResult, On%(name)s )
        PY_OVERRIDE_LOG( _steam, %(name)sCallResult, On%(name)s )
        bp::override func_On%(name)s = this->get_override( "On%(name)s" );
        if( func_On%(name)s.ptr() != Py_None )
            try {
                func_On%(name)s( boost::python::ptr(pData), bIOFailure );
            } catch(bp::error_already_set &) {
                PyErr_Print();
                this->%(name)sCallResult::On%(name)s( pData, bIOFailure );
            }
        else
            this->%(name)sCallResult::On%(name)s( pData, bIOFailure );
    }
    
    void default_On%(name)s( ::%(dataclass)s * pData, bool bIOFailure ) {
        %(name)sCallResult::On%(name)s( pData, bIOFailure );
    }
};
'''

callresult_reg_tmpl = '''{ //::%(name)sCallResult
        typedef bp::class_< %(name)sCallResult_wrapper, boost::noncopyable > %(name)sCallResult_exposer_t;
        %(name)sCallResult_exposer_t %(name)sCallResult_exposer = %(name)sCallResult_exposer_t( "%(name)sCallResult", bp::init< SteamAPICall_t >(( bp::arg("steamapicall") )) );
        bp::scope %(name)sCallResult_scope( %(name)sCallResult_exposer );
        bp::implicitly_convertible< SteamAPICall_t, %(name)sCallResult >();
        { //::%(name)sCallResult::On%(name)s
        
            typedef void ( ::%(name)sCallResult::*On%(name)s_function_type )( ::%(dataclass)s *,bool ) ;
            typedef void ( %(name)sCallResult_wrapper::*default_On%(name)s_function_type )( ::%(dataclass)s *,bool ) ;
            
            %(name)sCallResult_exposer.def( 
                "On%(name)s"
                , On%(name)s_function_type(&::%(name)sCallResult::On%(name)s)
                , default_On%(name)s_function_type(&%(name)sCallResult_wrapper::default_On%(name)s)
                , ( bp::arg("data"), bp::arg("iofailure") ) );
        
        }
    }
'''

class Steam(SemiSharedModuleGenerator):
    module_name = '_steam'
    steamsdkversion = (1, 15)

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_steam.cpp',
        '$SRCDIR\game\shared\python\srcpy_steam.h',
    ]

    @property
    def files(self):
        files = [
            'cbase.h',
            'steam/steam_api.h',
            'steam/isteamfriends.h',
            'steam/isteamutils.h',
            'steam/isteamuser.h',
            'steam/steamclientpublic.h',
            'steam/isteamuserstats.h',
            
            'srcpy_steam.h',
        ]
        return files
        
    def PythonfyVariables(self, cls):
        ''' Removes prefixes from variable names and lower cases the variable. '''
        for var in cls.variables(allow_empty=True):
            varname = var.name
            varname = re.sub('^(m_ul|m_un|m_us|m_n|m_e|m_E|m_i|m_b|m_c|m_rgf|m_sz)', '', varname)
            varname = re.sub('^(m_)', '', varname)
            varname = varname.lower()
            var.rename(varname)
            
    def AddSteamCallback(self, name, dataclsname):
        mb = self.mb
        
        # Include the dataclass
        cls = mb.class_(dataclsname)
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        self.PythonfyVariables(cls)
        
        # Generate the wrapper callback
        mb.add_declaration_code('PY_STEAM_CALLBACK_WRAPPER( %s, %s );' % (name, dataclsname))
        mb.add_declaration_code(callback_wrapper_tmpl % {'name' : name, 'dataclass' : dataclsname})
        mb.add_registration_code(callback_reg_tmpl % {'name' : name, 'dataclass' : dataclsname})
        
    def AddSteamCallResult(self, name, dataclsname):
        mb = self.mb
        
        # Include the dataclass
        cls = mb.class_(dataclsname)
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        self.PythonfyVariables(cls)
        
        # Generate the wrapper callback
        mb.add_declaration_code('PY_STEAM_CALLRESULT_WRAPPER( %s, %s );' % (name, dataclsname))
        mb.add_declaration_code(callresult_wrapper_tmpl % {'name' : name, 'dataclass' : dataclsname})
        mb.add_registration_code(callresult_reg_tmpl % {'name' : name, 'dataclass' : dataclsname})
        
    def ParseSteamApps(self, mb):
        cls = mb.class_('ISteamApps')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        
        cls.member_function('GetCurrentBetaName').exclude()
        mb.add_declaration_code('''static bp::tuple GetCurrentBetaName_44cfc773ce5b4e98b34b5a2219de76ad( ::ISteamApps & inst ){
    char buf[1024];
    bool rv = inst.GetCurrentBetaName( buf, sizeof(buf) );
    return bp::make_tuple( rv, bp::object( buf ) );
}''')
        cls.add_registration_code('''def( 
            "GetCurrentBetaName"
            , (bp::tuple (*)( ::ISteamApps & ) )( &GetCurrentBetaName_44cfc773ce5b4e98b34b5a2219de76ad )
            , ( bp::arg("inst") ) )''')
        
    def ParseSteamFriends(self, mb):
        cls = mb.class_('ISteamFriends')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        cls.member_function('GetFriendGamePlayed').exclude()
        cls.member_function('GetClanChatMessage').exclude()
        cls.member_function('GetFriendMessage').exclude()
        
        mb.enumeration('EFriendRelationship').include()
        mb.enumeration('EPersonaState').include()
        mb.enumeration('EPersonaChange').include()
        mb.add_registration_code( "bp::scope().attr( \"k_cchPersonaNameMax\" ) = (int)k_cchPersonaNameMax;" )
        
        self.AddSteamCallback('PersonaStateChange', 'PersonaStateChange_t')
        self.AddSteamCallback('GameOverlayActivated', 'GameOverlayActivated_t')
        self.AddSteamCallback('GameServerChangeRequested', 'GameServerChangeRequested_t')
        self.AddSteamCallback('GameLobbyJoinRequested', 'GameLobbyJoinRequested_t')
        self.AddSteamCallback('AvatarImageLoaded', 'AvatarImageLoaded_t')
        self.AddSteamCallback('ClanOfficerListResponse', 'ClanOfficerListResponse_t')
        self.AddSteamCallback('FriendRichPresenceUpdate', 'FriendRichPresenceUpdate_t')
        self.AddSteamCallback('GameRichPresenceJoinRequested', 'GameRichPresenceJoinRequested_t')
        self.AddSteamCallback('GameConnectedClanChatMsg', 'GameConnectedClanChatMsg_t')
        self.AddSteamCallback('GameConnectedChatJoin', 'GameConnectedChatJoin_t')
        self.AddSteamCallback('GameConnectedChatLeave', 'GameConnectedChatLeave_t')
        self.AddSteamCallback('DownloadClanActivityCountsResult', 'DownloadClanActivityCountsResult_t')
        self.AddSteamCallback('JoinClanChatRoomCompletionResult', 'JoinClanChatRoomCompletionResult_t')
        self.AddSteamCallback('GameConnectedFriendChatMsg', 'GameConnectedFriendChatMsg_t')
        self.AddSteamCallback('FriendsGetFollowerCount', 'FriendsGetFollowerCount_t')
        self.AddSteamCallback('FriendsIsFollowing', 'FriendsIsFollowing_t')
        self.AddSteamCallback('FriendsEnumerateFollowingList', 'FriendsEnumerateFollowingList_t')
        self.AddSteamCallback('SetPersonaNameResponse', 'SetPersonaNameResponse_t')
        
    def ParseMatchmaking(self, mb):
        # The main matchmaking interface
        cls = mb.class_('ISteamMatchmaking')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        cls.member_functions('GetLobbyGameServer').add_transformation(FT.output('punGameServerIP'), FT.output('punGameServerPort'), FT.output('psteamIDGameServer'))
        
        cls.member_function('GetLobbyDataByIndex').exclude()
        cls.member_function('SendLobbyChatMsg').exclude()
        cls.member_function('GetLobbyChatEntry').exclude()
        
        mb.free_function('PyGetLobbyDataByIndex').include()
        mb.free_function('PySendLobbyChatMsg').include()
        mb.free_function('PyGetLobbyChatEntry').include()
        
        mb.variable('k_uAPICallInvalid').include()
        
        self.AddSteamCallResult('LobbyMatchList', 'LobbyMatchList_t')
        self.AddSteamCallResult('LobbyGameCreated', 'LobbyGameCreated_t')
        self.AddSteamCallResult('LobbyCreated', 'LobbyCreated_t')
        self.AddSteamCallResult('LobbyEnter', 'LobbyEnter_t')
        
        self.AddSteamCallback('LobbyChatUpdate', 'LobbyChatUpdate_t')
        self.AddSteamCallback('LobbyChatMsg', 'LobbyChatMsg_t')
        self.AddSteamCallback('LobbyDataUpdate', 'LobbyDataUpdate_t')
        
        # Servers matchmaking interface
        cls = mb.class_('PySteamMatchmakingServers')
        cls.include()
        cls.rename('SteamMatchmakingServers')
        
        cls = mb.class_('PySteamMatchmakingServerListResponse')
        cls.include()
        cls.rename('SteamMatchmakingServerListResponse')
        cls.member_function('PyServerResponded').rename('ServerResponded')
        cls.member_function('PyServerFailedToRespond').rename('ServerFailedToRespond')
        cls.member_function('PyRefreshComplete').rename('RefreshComplete')
        
        cls = mb.class_('gameserveritem_t')
        cls.include()
        cls.member_function('SetName').exclude()
        self.PythonfyVariables(cls)
        cls.variable('m_szGameDir').exclude()
        cls.variable('m_szMap').exclude()
        cls.variable('m_szGameDescription').exclude()
        cls.variable('m_szGameTags').exclude()
        
        cls = mb.class_('pygameserveritem_t')
        cls.include()
        self.PythonfyVariables(cls)
        self.AddProperty(cls, 'gamedir', 'GetGameDir')
        self.AddProperty(cls, 'map', 'GetMap')
        self.AddProperty(cls, 'gamedescription', 'GetGameDescription')
        self.AddProperty(cls, 'gametags', 'GetGameTags')
        
        cls = mb.class_('servernetadr_t')
        cls.include()
        cls.rename('servernetadr')
        
        cls = mb.class_('PySteamMatchmakingPingResponse')
        cls.include()
        cls.rename('SteamMatchmakingPingResponse')
        
        cls = mb.class_('PySteamMatchmakingPlayersResponse')
        cls.include()
        cls.rename('SteamMatchmakingPlayersResponse')
        
        cls = mb.class_('PySteamMatchmakingRulesResponse')
        cls.include()
        cls.rename('SteamMatchmakingRulesResponse')
        
        # Enums
        mb.enumerations('ELobbyType').include()
        mb.enumerations('ELobbyComparison').include()
        mb.enumerations('ELobbyDistanceFilter').include()
        mb.enumerations('EMatchMakingServerResponse').include()
        
    def ParseUserStats(self, mb):
        cls = mb.class_('ISteamUserStats')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        
        self.AddSteamCallResult('NumberOfCurrentPlayers', 'NumberOfCurrentPlayers_t')
        
        mb.free_function('PyGetStatFloat').include()
        mb.free_function('PyGetStatInt').include()
        
    def ParseGameServer(self, mb):
        cls = mb.class_('ISteamGameServer')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        
        cls.member_function('BeginAuthSession').exclude()
        cls.member_function('GetAuthSessionTicket').exclude()
        cls.member_function('GetNextOutgoingPacket').exclude()
        cls.member_function('HandleIncomingPacket').exclude()
        cls.member_function('SendUserConnectAndAuthenticate').exclude()
        
        mb.free_function('PyGameServer_GetAuthSessionTicket').include()
        mb.free_function('PyGameServer_GetAuthSessionTicket').rename('GameServer_GetAuthSessionTicket')
        
    def ParseServerOnly(self, mb):
        # Accessor class game server
        mb.add_registration_code( "bp::scope().attr( \"steamgameserverapicontext\" ) = boost::ref(steamgameserverapicontext);" )
        cls = mb.class_('CSteamGameServerAPIContext')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_function('Init').exclude()
        cls.member_function('Clear').exclude()
        cls.member_function('SteamGameServerUtils').exclude()
        cls.member_function('SteamGameServerNetworking').exclude()
        cls.member_function('SteamGameServerStats').exclude()
        
        if self.steamsdkversion > (1, 16):
            cls.member_function('SteamHTTP').exclude()
            cls.member_function('SteamUGC').exclude()
            
        cls.member_functions('SteamGameServer').call_policies = call_policies.return_internal_reference()
    
        self.ParseGameServer(mb)

    def Parse(self, mb):
        if self.settings.branch == 'source2013':
            self.steamsdkversion = (1, 30)
    
        # Exclude everything by default
        mb.decls().exclude()  

        # Generic steam api call return result
        mb.typedef('SteamAPICall_t').include()

        # CSteamID
        cls = mb.class_('CSteamID')
        cls.include()
        constpchararg = pointer_t(const_t(declarated_t(char_t())))
        cls.constructors(calldef_matcher_t(arg_types=[constpchararg, None])).exclude()
        cls.member_functions('Render').exclude()
        cls.member_functions('SetFromStringStrict').exclude()
        cls.member_functions('SetFromString').exclude() # No definition...
        cls.member_functions('SetFromSteam2String').exclude() # No definition...
        cls.member_functions('BValidExternalSteamID').exclude() # No definition...
        
        mb.enumeration('EResult').include()
        mb.enumeration('EDenyReason').include()
        mb.enumeration('EUniverse').include()
        mb.enumeration('EAccountType').include()
        mb.enumeration('ESteamUserStatType').include()
        mb.enumeration('EChatEntryType').include()
        mb.enumeration('EChatRoomEnterResponse').include()
        mb.enumeration('EChatMemberStateChange').include()
        
        # Generic API functions
        mb.free_function('SteamAPI_RunCallbacks').include()
        
        # Accessor class client
        mb.add_registration_code( "bp::scope().attr( \"steamapicontext\" ) = boost::ref(steamapicontext);" )
        cls = mb.class_('CSteamAPIContext')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_function('Init').exclude()
        cls.member_function('Clear').exclude()
        
        if self.steamsdkversion > (1, 11):
            cls.member_function('SteamHTTP').exclude()
        if self.steamsdkversion > (1, 15):
            cls.member_function('SteamScreenshots').exclude()
        if self.steamsdkversion > (1, 20):
            cls.member_function('SteamUnifiedMessages').exclude()
            
        cls.member_function('SteamMatchmakingServers').exclude() # Full python class wrapper

        cls.member_function('SteamNetworking').exclude()
        cls.member_function('SteamRemoteStorage').exclude()
        if self.steamsdkversion > (1, 16):
            cls.member_function('SteamAppList').exclude()
            cls.member_function('SteamController').exclude()
            cls.member_function('SteamMusic').exclude()
            cls.member_function('SteamMusicRemote').exclude()
            cls.member_function('SteamUGC').exclude() 
            cls.member_function('SteamHTMLSurface').exclude()
            
        cls.member_functions('SteamApps').call_policies = call_policies.return_internal_reference()
        cls.member_functions('SteamFriends').call_policies = call_policies.return_internal_reference()
        cls.member_functions('SteamUtils').call_policies = call_policies.return_internal_reference()
        cls.member_functions('SteamMatchmaking').call_policies = call_policies.return_internal_reference()
        cls.member_functions('SteamMatchmakingServers').call_policies = call_policies.return_internal_reference()
        cls.member_functions('SteamUser').call_policies = call_policies.return_internal_reference()
        cls.member_functions('SteamUserStats').call_policies = call_policies.return_internal_reference()
        
        mb.add_registration_code( "bp::scope().attr( \"QUERY_PORT_NOT_INITIALIZED\" ) = (int)QUERY_PORT_NOT_INITIALIZED;" )
        mb.add_registration_code( "bp::scope().attr( \"QUERY_PORT_ERROR\" ) = (int)QUERY_PORT_ERROR;" )
        
        self.ParseSteamApps(mb)
        self.ParseSteamFriends(mb)
        
        # User
        cls = mb.class_('ISteamUser')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        
        cls.member_function('DecompressVoice').exclude()
        cls.member_function('GetAuthSessionTicket').exclude()
        cls.member_function('GetEncryptedAppTicket').exclude()
        cls.member_function('GetVoice').exclude()
        cls.member_function('InitiateGameConnection').exclude()
        cls.member_function('RequestEncryptedAppTicket').exclude()
        cls.member_function('BeginAuthSession').exclude()
        
        mb.free_function('PySteamUser_GetAuthSessionTicket').include()
        mb.free_function('PySteamUser_GetAuthSessionTicket').rename('SteamUser_GetAuthSessionTicket')
        
        # Utils
        cls = mb.class_('ISteamUtils')
        cls.include()
        cls.no_init = True
        cls.noncopyable = True
        cls.member_functions().virtuality = 'not virtual'
        cls.member_function('GetImageRGBA').exclude()
        cls.member_function('GetImageSize').exclude()
        cls.member_function('GetAPICallResult').exclude()
        
        self.ParseMatchmaking(mb)
        self.ParseUserStats(mb)
        
        #mb.class_('ISteamUtils').member_functions('GetImageSize').add_transformation( FT.output('pnWidth'), FT.output('pnHeight'))
        #mb.class_('ISteamUtils').member_functions('GetCSERIPPort').add_transformation( FT.output('unIP'), FT.output('usPort'))
        
        if self.isserver:
            self.ParseServerOnly(mb)
        