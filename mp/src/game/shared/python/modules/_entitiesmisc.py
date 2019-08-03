from srcpy.module_generators import SemiSharedModuleGenerator
from pyplusplus.module_builder import call_policies
from pygccxml.declarations import matchers

class EntitiesMisc(SemiSharedModuleGenerator):
    module_name = '_entitiesmisc'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_entities.cpp',
        '$SRCDIR\game\shared\python\srcpy_entities.h',
    ]

    module_symbols = ['SRCPY_MOD_ENTITIES']
    
    files = [
        'cbase.h',
        'takedamageinfo.h',
        '$cliententitylist.h',
        '$soundinfo.h',
        
        '#mathlib/vmatrix.h', 
        '#utlvector.h', 
        '#shareddefs.h', 
        '#util.h',
        
        '#networkvar.h',
        '#eventqueue.h',
        '#entitylist.h',
        '#takedamageinfo.h',
        '#srcpy_networkvar.h',
        '#soundent.h',
        '#entityoutput.h',
        '#SkyCamera.h',
        '#world.h',
        '#globals.h',
        '#physics_prop_ragdoll.h',

        '#srcpy_srcbuiltins.h',
        
        'shared_classnames.h',
        'npcevent.h',
        'studio.h',
        'srcpy_entities.h',
        'isaverestore.h',
        'saverestore.h',
        'mapentities_shared.h',
        'vcollide_parse.h',
        'props_shared.h',
        'beam_shared.h',
        'shot_manipulator.h',
    ]
        
    def ParseClientEntityRelated(self, mb):    
        # Creating a client class
        cls =  mb.class_('NetworkedClass')
        cls.include()
        cls.variables('m_pClientClass').exclude()
        
        cls = self.IncludeEmptyClass(mb, 'ClientClass')
        cls.no_init = True
        cls.calldefs('ClientClass').exclude()
        cls.member_function('GetName').include() # For debugging purposes
        
        # Client Entity List
        cls = mb.class_('CClientEntityList')
        cls.include()
        cls.calldefs().virtuality = 'not virtual'
        cls.no_init = True

        cls.member_functions().exclude()
        
        cls.member_function('NumberOfEntities').include()
        cls.member_function('GetHighestEntityIndex').include()
        cls.member_function('GetBaseEntity').include()
        cls.member_function('GetBaseEntity').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_function('GetBaseEntityFromHandle').include()
        cls.member_function('GetBaseEntityFromHandle').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_function('EntIndexToHandle').include()
        cls.member_function('HandleToEntIndex').include()
        cls.member_function('IsHandleValid').include()
        cls.member_function('FirstBaseEntity').include()
        cls.member_function('FirstBaseEntity').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_function('NextBaseEntity').include()
        cls.member_function('NextBaseEntity').call_policies = call_policies.return_value_policy(call_policies.return_by_value)

        mb.free_function('ClientEntityList').include()
        mb.free_function('ClientEntityList').call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
        
        # PyEntityFactory    
        self.IncludeEmptyClass(mb, 'PyEntityFactory')
        mb.class_('PyEntityFactory').rename('EntityFactory')
        mb.free_function('PyGetClassByClassname').include()
        mb.free_function('PyGetClassByClassname').rename('GetClassByClassname')
        mb.free_function('PyGetAllClassnames').include()
        mb.free_function('PyGetAllClassnames').rename('GetAllClassnames')
        if self.isserver or self.settings.branch == 'swarm':
            mb.free_function('PyReadDataDesc').include()
            mb.free_function('PyReadDataDesc').rename('ReadDataDesc')
        
        # Client only structs
        cls = mb.class_('SpatializationInfo_t')
        cls.include()
        cls.variable('pOrigin').exclude()
        cls.variable('pAngles').exclude()
        cls.variable('pflRadius').exclude()
        
    def ParseServerEntityRelated(self, mb):
        # PyEntityFactory    
        self.IncludeEmptyClass(mb, 'PyEntityFactory')
        mb.class_('PyEntityFactory').rename('EntityFactory')
        mb.free_function('PyGetClassByClassname').include()
        mb.free_function('PyGetClassByClassname').rename('GetClassByClassname')
        mb.free_function('PyGetAllClassnames').include()
        mb.free_function('PyGetAllClassnames').rename('GetAllClassnames')
        
        # Creating a server class
        cls = mb.class_('NetworkedClass')
        cls.include()
        cls.variables('m_pServerClass').exclude()
        
        cls = self.IncludeEmptyClass(mb, 'ServerClass')
        cls.no_init = True
        cls.calldefs('ServerClass').exclude()
        cls.member_function('GetName').include() # For debugging purposes
        
        self.IncludeEmptyClass(mb, 'SendTable')

        # Creating a networked variable
        cls = mb.class_('CPythonNetworkVarBase')
        cls.include()
        cls.no_init = True
        cls.calldefs().exclude()
        cls.variables().exclude()
        cls.member_function('NetworkStateChanged').include()
        
        cls = mb.class_('CPythonNetworkVar')
        cls.include()
        cls.rename('NetworkVarInternal') 
        cls.member_functions('Str').rename('__str__')
        cls.member_functions('NetworkVarsUpdateClient').exclude()

        cls = mb.class_('CPythonNetworkArray')
        cls.include()
        cls.rename('NetworkArrayInternal') 
        cls.member_functions('NetworkVarsUpdateClient').exclude()
        cls.member_functions('GetItem').rename('__getitem__')
        cls.member_functions('SetItem').rename('__setitem__')
        cls.member_functions('DelItem').rename('__delitem__')
        cls.member_functions('Str').rename('__str__')
        
        cls = mb.class_('CPythonNetworkDict')
        cls.include()
        cls.rename('NetworkDictInternal') 
        cls.member_functions('NetworkVarsUpdateClient').exclude()
        cls.member_functions('GetItem').rename('__getitem__')
        cls.member_functions('SetItem').rename('__setitem__')
        cls.member_functions('Str').rename('__str__')
        
        # Send proxies for python network variables
        cls = mb.class_('CPythonSendProxyBase')
        cls.rename('SendProxyBase') 
        cls.include()
        
        # Ugly globals
        mb.add_registration_code( "bp::scope().attr( \"g_vecAttackDir\" ) = boost::ref(g_vecAttackDir);" )
    
        # Event queue
        cls = mb.class_('CEventQueue')
        cls.include()
        mb.variables('g_EventQueue').include()
        cls.member_functions('Init').exclude()
        cls.member_functions('Restore').exclude()
        cls.member_functions('Save').exclude()
        cls.member_functions('ValidateQueue').exclude()
        cls.member_functions('GetBaseMap').exclude()    
        
        # Global entity list
        cls = mb.class_('IEntityFindFilter')
        cls.include()
        cls.member_functions('GetFilterResult').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
        cls = mb.class_('CGlobalEntityList')
        cls.include()
        cls.calldefs().virtuality = 'not virtual'
        cls.no_init = True
        cls.member_functions('GetServerNetworkable').exclude()
        cls.member_functions('GetBaseNetworkable').exclude()
        cls.member_functions('GetBaseEntity').exclude()
        cls.member_functions('GetEdict').exclude()    
        cls.member_functions('AddToDeleteList').exclude()
        #cls.member_functions('CleanupDeleteList').exclude() # Keep
        #cls.member_functions('ResetDeleteList').exclude() # Keep
        cls.member_functions('Clear').exclude()
        cls.member_functions('NotifyCreateEntity').exclude()
        cls.member_functions('NotifySpawn').exclude()
        cls.member_functions('NotifyRemoveEntity').exclude()
        cls.member_functions('AddListenerEntity').exclude() # Would require some kind of python version
        cls.member_functions('RemoveListenerEntity').exclude()
        cls.member_functions('IsEntityPtr').exclude() # void pointer
        
        cls.member_functions('NextEnt').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FirstEnt').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByClassname').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByName').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityInSphere').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByTarget').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByModel').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByNameNearest').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByNameWithin').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByClassnameNearest').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityByClassnameWithin').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityGeneric').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityGenericWithin').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityGenericNearest').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityNearestFacing').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityClassNearestFacing').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('FindEntityProcedural').call_policies = call_policies.return_value_policy(call_policies.return_by_value)

        if self.settings.branch == 'swarm':
            cls.member_functions('FindEntityByClassnameFast').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
            cls.member_functions('FindEntityByClassnameNearest2D').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
            cls.member_functions('FindEntityByClassnameNearestFast').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
            cls.member_functions('FindEntityByNameFast').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
            cls.member_functions('FindEntityByOutputTarget').call_policies = call_policies.return_value_policy(call_policies.return_by_value)

        mb.variables('gEntList').include()
        mb.variables('gEntList').rename('entlist')
        
        # Respawning the player using a python entity class
        mb.free_function('PyRespawnPlayer').include()
        mb.free_function('PyRespawnPlayer').rename('RespawnPlayer')
        
        # Getting the world
        mb.free_function('PyGetWorldEntity').include()
        mb.free_function('PyGetWorldEntity').rename('GetWorldEntity')
        
        # Entity input/output
        cls = mb.class_('CBaseEntityOutput')
        cls.include()
        cls.member_functions('GetBaseMap').exclude()
        cls.member_functions('Save').exclude()
        cls.member_functions('Restore').exclude()
        
        if self.settings.branch == 'swarm':
            cls.member_functions('GetActionForTarget').exclude()
            cls.member_functions('GetFirstAction').exclude()
            
        # Note: PyOutputEvent covers all the other types
        # There is no need for COutputInt, COutputFloat, etc.
        cls = mb.class_('PyOutputEvent')
        cls.include()
        cls.rename('COutputEvent') 
        
        # Inputdata_t and variant_t
        cls = mb.class_('inputdata_t')
        cls.include()
        cls.variable('pActivator').getter_call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.variable('pCaller').getter_call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.variable('nOutputID').rename('outputid')
        cls.variable('pActivator').rename('activator')
        cls.variable('pCaller').rename('caller')
        
        cls = mb.class_('variant_t')
        cls.include()
        cls.variables( lambda decl: 'm_Save' in decl.name ).exclude()
        cls.variables('vecVal').exclude()
        cls.member_function('Entity').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
        cls.member_function('Entity').exclude()
        cls.add_wrapper_code(   'static bp::object PyEntity( variant_t &inst )\r\n' + \
                                '{\r\n' + \
                                '   return inst.Entity() ? inst.Entity()->GetPyHandle() : bp::object();\r\n' + \
                                '}\r\n' )
        cls.add_registration_code( 'def("Entity", &::variant_t_wrapper::PyEntity)')
        
        cls.member_function('Set').exclude() # void * argument
        cls.member_function('SetOther').exclude() # void * argument
        
        # Collision
        cls = mb.class_('vcollisionevent_t')
        cls.include()
        cls.variables('pObjects').exclude()
        
        # Wrap internal data functions
        cls.variables('pInternalData').exclude()
        cls.add_wrapper_code(   'static void GetSurfaceNormal( vcollisionevent_t &inst, Vector &out )\r\n' + \
                                '{\r\n' + \
                                '   inst.pInternalData->GetSurfaceNormal(out);\r\n' + \
                                '}\r\n' )
        cls.add_registration_code( 'def("GetSurfaceNormal", &::vcollisionevent_t_wrapper::GetSurfaceNormal)')
        cls.add_wrapper_code(   'static void GetContactPoint( vcollisionevent_t &inst, Vector &out )\r\n' + \
                                '{\r\n' + \
                                '   inst.pInternalData->GetContactPoint(out);\r\n' + \
                                '}\r\n' )
        cls.add_registration_code( 'def("GetContactPoint", &::vcollisionevent_t_wrapper::GetContactPoint)')
        cls.add_wrapper_code(   'static void GetContactSpeed( vcollisionevent_t &inst, Vector &out )\r\n' + \
                                '{\r\n' + \
                                '   inst.pInternalData->GetContactSpeed(out);\r\n' + \
                                '}\r\n' )
        cls.add_registration_code( 'def("GetContactSpeed", &::vcollisionevent_t_wrapper::GetContactSpeed)')
        
        cls = mb.class_('gamevcollisionevent_t')
        cls.include()
        cls.member_functions('Init').exclude()
        cls.variables('pEntities').exclude()
        
        cls.add_wrapper_code(   'static bp::object GetEnt( gamevcollisionevent_t &inst, int index )\r\n' + \
                                '{\r\n' + \
                                '   if( index < 0 || index > 1 )\r\n' + \
                                '       return bp::object();\r\n' + \
                                '   return inst.pEntities[index] ? inst.pEntities[index]->GetPyHandle() : bp::object();\r\n' + \
                                '}\r\n' )
        cls.add_registration_code( 'def("GetEnt", &::gamevcollisionevent_t_wrapper::GetEnt)')
            
    def ParseMisc(self, mb):
        # CUserCmd, player commands
        cls = mb.class_('CUserCmd')
        cls.include()
        cls.calldefs().exclude()
        self.PythonfyVariables(cls)
    
        # Save/restore interfaces. Just empty.
        cls = mb.class_('IRestore')
        cls.include()
        cls.calldefs().exclude()
        cls = mb.class_('ISave')
        cls.include()
        cls.calldefs().exclude()
    
        if self.isserver:
            # Sky camera
            mb.free_function('GetCurrentSkyCamera').include()
            mb.free_function('GetCurrentSkyCamera').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
        # CTakeDamageInfo
        cls = mb.class_('CTakeDamageInfo')
        cls.include()
        cls.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        
        cls.member_functions('GetInflictor').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        cls.member_functions('GetWeapon').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        cls.member_functions('GetAttacker').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        
        cls = mb.class_('CMultiDamage')
        cls.include()
        cls.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        cls.member_functions('GetTarget').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        
        mb.free_functions('ClearMultiDamage').include()
        mb.free_functions('ApplyMultiDamage').include()
        mb.free_functions('AddMultiDamage').include()
        mb.free_functions('ImpulseScale').include()
        mb.free_functions('CalculateExplosiveDamageForce').include()
        mb.free_functions('CalculateBulletDamageForce').include()
        mb.free_functions('CalculateMeleeDamageForce').include()
        mb.free_functions('GuessDamageForce').include()
        
        # FireBulletsInfo_t
        cls = mb.class_('FireBulletsInfo_t')
        cls.include()
        cls.variable('m_iShots').rename('shots')
        cls.variable('m_vecSrc').rename('vecsrc')
        cls.variable('m_vecDirShooting').rename('vecdirshooting')
        cls.variable('m_vecSpread').rename('vecspread')
        cls.variable('m_flDistance').rename('distance')
        cls.variable('m_iAmmoType').rename('ammotype')
        cls.variable('m_iTracerFreq').rename('tracerfreq')
        cls.variable('m_nFlags').rename('flags')
        cls.variable('m_flDamageForceScale').rename('damageforcescale')
        cls.variable('m_pAttacker').rename('attacker')
        cls.variable('m_pAdditionalIgnoreEnt').rename('additionalignoreent')
        cls.variable('m_bPrimaryAttack').rename('primaryattack')
        cls.variable('m_flDamage').rename('damage')
        cls.variable('m_iPlayerDamage' if self.settings.branch == 'source2013' else 'm_flPlayerDamage').rename('playerdamage')
        
        mb.enumeration('FireBulletsFlags_t').include()
        
        # CShotManipulator
        cls = mb.class_('CShotManipulator')
        cls.include()
        
        if self.isserver:
            # Bone follower
            cls = mb.class_('pyphysfollower_t')
            cls.include()
            cls.rename('physfollower')
            
            cls = mb.class_('PyBoneFollowerManager')
            cls.include()
            cls.rename('BoneFollowerManager')
            
            mb.free_function('GetAttachmentPositionInSpaceOfBone').include()
            
            # Ragdoll functions
            mb.free_function('CreateServerRagdoll').include()
            mb.free_function('CreateServerRagdoll').call_policies = call_policies.return_value_policy(call_policies.return_by_value)

            if self.settings.branch == 'swarm':
                mb.free_function('PyCreateServerRagdollAttached').include()
                mb.free_function('PyCreateServerRagdollAttached').rename('CreateServerRagdollAttached')
                mb.free_function('PyCreateServerRagdollAttached').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
            mb.free_function('DetachAttachedRagdoll').include()
            mb.free_function('DetachAttachedRagdollsForEntity').include()
        
        # Shared Props
        mb.class_('breakablepropparams_t').include()
        mb.free_functions('GetMassEquivalent').include()
        mb.free_functions('GetAutoMultiplayerPhysicsMode').include()
        mb.free_functions('PropBreakableCreateAll').include()
        mb.free_functions('PrecacheGibsForModel').include()
    
        # Enums
        mb.enumerations('MoveType_t').include()
        mb.enumerations('SolidType_t').include()
        mb.enumerations('SolidFlags_t').include()
        mb.enumerations('Collision_Group_t').include()
        mb.enumerations('RenderMode_t').include()
        mb.enumerations('RenderFx_t').include()
        
        if self.isclient:
            mb.enumerations('ShadowType_t').include()
            mb.enumerations('RenderGroup_t').include()
            mb.enumerations('DataUpdateType_t').include()
            mb.enumerations('CollideType_t').include()
            mb.enumerations('ShouldTransmitState_t').include()
            mb.enumerations('OverrideType_t').include()
        else:
            mb.enumerations('EntityEvent_t').include()
            mb.enumerations('Class_T').include()
        mb.enumerations('_fieldtypes').include()
            
        mb.enumerations('USE_TYPE').include()
        mb.enumerations('Activity').include()
        
        # Anim event
        cls = mb.class_('animevent_t')
        cls.include()
        
        cls.add_wrapper_code(   'static bp::object get_options(animevent_t const & inst){\r\n' + \
                                '    return bp::object(inst.options);\r\n' + \
                                '}\r\n'
                            )
                            
        cls.add_registration_code( 'add_property("options"\r\n' + \
                                                "    , bp::make_function( (bp::object (*)( ::animevent_t & ))(&animevent_t_wrapper::get_options) ) )\r\n"
                                              )
        
        # Solid_t class
        mb.classes('solid_t').include()
        
        # Types/Contants
        if self.isserver:
            mb.add_registration_code( "bp::scope().attr( \"SOUND_NONE\" ) = (int)SOUND_NONE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_COMBAT\" ) = (int)SOUND_COMBAT;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_WORLD\" ) = (int)SOUND_WORLD;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_PLAYER\" ) = (int)SOUND_PLAYER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_DANGER\" ) = (int)SOUND_DANGER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_BULLET_IMPACT\" ) = (int)SOUND_BULLET_IMPACT;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CARCASS\" ) = (int)SOUND_CARCASS;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_GARBAGE\" ) = (int)SOUND_GARBAGE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_THUMPER\" ) = (int)SOUND_THUMPER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_BUGBAIT\" ) = (int)SOUND_BUGBAIT;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_PHYSICS_DANGER\" ) = (int)SOUND_PHYSICS_DANGER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_DANGER_SNIPERONLY\" ) = (int)SOUND_DANGER_SNIPERONLY;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_MOVE_AWAY\" ) = (int)SOUND_MOVE_AWAY;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_PLAYER_VEHICLE\" ) = (int)SOUND_PLAYER_VEHICLE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_READINESS_LOW\" ) = (int)SOUND_READINESS_LOW;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_READINESS_MEDIUM\" ) = (int)SOUND_READINESS_MEDIUM;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_READINESS_HIGH\" ) = (int)SOUND_READINESS_HIGH;" )
            
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_FROM_SNIPER\" ) = (int)SOUND_CONTEXT_FROM_SNIPER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_GUNFIRE\" ) = (int)SOUND_CONTEXT_GUNFIRE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_MORTAR\" ) = (int)SOUND_CONTEXT_MORTAR;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_COMBINE_ONLY\" ) = (int)SOUND_CONTEXT_COMBINE_ONLY;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_REACT_TO_SOURCE\" ) = (int)SOUND_CONTEXT_REACT_TO_SOURCE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_EXPLOSION\" ) = (int)SOUND_CONTEXT_EXPLOSION;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_EXCLUDE_COMBINE\" ) = (int)SOUND_CONTEXT_EXCLUDE_COMBINE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_DANGER_APPROACH\" ) = (int)SOUND_CONTEXT_DANGER_APPROACH;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_ALLIES_ONLY\" ) = (int)SOUND_CONTEXT_ALLIES_ONLY;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUND_CONTEXT_PLAYER_VEHICLE\" ) = (int)SOUND_CONTEXT_PLAYER_VEHICLE;" )
            
            mb.add_registration_code( "bp::scope().attr( \"ALL_CONTEXTS\" ) = (int)ALL_CONTEXTS;" )
            mb.add_registration_code( "bp::scope().attr( \"ALL_SCENTS\" ) = (int)ALL_SCENTS;" )
            mb.add_registration_code( "bp::scope().attr( \"ALL_SOUNDS\" ) = (int)ALL_SOUNDS;" )
            
            # Channels
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_UNSPECIFIED\" ) = (int)SOUNDENT_CHANNEL_UNSPECIFIED;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_REPEATING\" ) = (int)SOUNDENT_CHANNEL_REPEATING;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_REPEATED_DANGER\" ) = (int)SOUNDENT_CHANNEL_REPEATED_DANGER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_REPEATED_PHYSICS_DANGER\" ) = (int)SOUNDENT_CHANNEL_REPEATED_PHYSICS_DANGER;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_WEAPON\" ) = (int)SOUNDENT_CHANNEL_WEAPON;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_INJURY\" ) = (int)SOUNDENT_CHANNEL_INJURY;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_BULLET_IMPACT\" ) = (int)SOUNDENT_CHANNEL_BULLET_IMPACT;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_NPC_FOOTSTEP\" ) = (int)SOUNDENT_CHANNEL_NPC_FOOTSTEP;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_SPOOKY_NOISE\" ) = (int)SOUNDENT_CHANNEL_SPOOKY_NOISE;" )
            mb.add_registration_code( "bp::scope().attr( \"SOUNDENT_CHANNEL_ZOMBINE_GRENADE\" ) = (int)SOUNDENT_CHANNEL_ZOMBINE_GRENADE;" )
            
            # Transmit state
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_CHANGED\" ) = (int)FL_EDICT_CHANGED;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_FREE\" ) = (int)FL_EDICT_FREE;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_FULL\" ) = (int)FL_EDICT_FULL;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_FULLCHECK\" ) = (int)FL_EDICT_FULLCHECK;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_ALWAYS\" ) = (int)FL_EDICT_ALWAYS;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_DONTSEND\" ) = (int)FL_EDICT_DONTSEND;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_PVSCHECK\" ) = (int)FL_EDICT_PVSCHECK;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_PENDING_DORMANT_CHECK\" ) = (int)FL_EDICT_PENDING_DORMANT_CHECK;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_EDICT_DIRTY_PVS_INFORMATION\" ) = (int)FL_EDICT_DIRTY_PVS_INFORMATION;" )
            mb.add_registration_code( "bp::scope().attr( \"FL_FULL_EDICT_CHANGED\" ) = (int)FL_FULL_EDICT_CHANGED;" )
                    
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()

        if self.isclient:
            self.ParseClientEntityRelated(mb)
        else:
            self.ParseServerEntityRelated(mb)
        self.ParseMisc(mb)
        
        # Finally apply common rules to all includes functions and classes, etc.
        self.ApplyCommonRules(mb)
