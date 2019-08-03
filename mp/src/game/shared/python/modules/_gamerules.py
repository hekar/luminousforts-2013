from srcpy.module_generators import SemiSharedModuleGenerator

from pyplusplus.module_builder import call_policies
from pygccxml.declarations import matchers

class GameRules(SemiSharedModuleGenerator):
    module_name = '_gamerules'
    split = True

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_gamerules.cpp',
        '$SRCDIR\game\shared\python\srcpy_gamerules.h',
    ]

    module_symbols = ['SRCPY_MOD_GAMERULES']

    @property
    def files(self):
        files = [
            'cbase.h',
            'gamerules.h',
            'multiplay_gamerules.h',
            'singleplay_gamerules.h',
            'teamplay_gamerules.h',
            'srcpy_gamerules.h',
            'ammodef.h',
            '#items.h',
            '$takedamageinfo.h',
        ]
        if 'HL2MP' in self.symbols:
            files.extend([
                '#hl2mp/hl2mp_player.h',
                'hl2mp/hl2mp_gamerules.h',
            ])
        return files
        
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        gamerules = [
            ('CGameRules', 'C_GameRules', True),
            ('CMultiplayRules', 'C_MultiplayRules', True),
            ('CSingleplayRules', 'C_SingleplayRules', True),
            ('CTeamplayRules', 'C_TeamplayRules', False),
        ]
        if 'HL2MP' in self.symbols:
            gamerules.append(('CHL2MPRules', 'C_HL2MPRules', False))
        
        for gamerulename, clientgamerulename, baseonly in gamerules:
            cls_name = gamerulename if self.isserver else clientgamerulename
            cls = mb.class_(cls_name)
            cls.include()
            
            if not baseonly:
                # Used internally
                cls.member_functions('GetPySelf', allow_empty=True).exclude()
                cls.add_wrapper_code(
                    'virtual PyObject *GetPySelf() const { return boost::python::detail::wrapper_base_::get_owner(*this); }'
                )
            else:
                cls.no_init = True
            
            # Always use server class name
            cls.rename(gamerulename)
            
        mb.member_functions('ShouldCollide').virtuality = 'not virtual'
        mb.member_functions('GetAmmoDamage').virtuality = 'not virtual' # Just modify the ammo table when needed...
        
        mb.member_functions('NetworkStateChanged').exclude()
        
        if self.isserver:
            if self.settings.branch == 'source2013':
                mb.member_function('TacticalMissionManagerFactory').exclude()
            if self.settings.branch == 'swarm':
                mb.member_functions('DoFindClientInPVS').exclude()
                
                mb.member_functions('IsTopDown').virtuality = 'not virtual'
                mb.member_functions('ForceSplitScreenPlayersOnToSameTeam').virtuality = 'not virtual'

            # Excludes
            mb.member_functions('VoiceCommand').exclude()
            
            # Remove virtuality from  or include some unneeded functions (would just slow down things)
            mb.member_functions('FrameUpdatePostEntityThink').virtuality = 'not virtual' # Calls Think
            mb.member_functions('EndGameFrame').virtuality = 'not virtual'
            mb.member_functions('FAllowFlashlight', lambda d: d.virtuality == 'virtual').virtuality = 'not virtual'

        if 'HL2MP' in self.symbols:
            mb.member_functions('GetHL2MPViewVectors').exclude()
            
            if self.isclient:
                mb.member_functions('RestartGame').exclude()
                mb.member_functions('CheckAllPlayersReady').exclude()
                mb.member_functions('CheckRestartGame').exclude()
                mb.member_functions('CleanUpMap').exclude()
            
        # Temp excludes
        mb.member_functions('GetEncryptionKey').exclude()
        mb.member_functions('GetViewVectors').exclude()

        # Call policies
        mb.member_functions('GetNextBestWeapon').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        
        # Rename PyGameRules to just GameRules
        mb.free_function('PyGameRules').include()
        mb.free_function('PyGameRules').rename('GameRules')
        
        # Installing the gamerules
        mb.free_function('PyInstallGameRules').include()
        mb.free_function('PyInstallGameRules').rename('InstallGameRules')
        
        # CAmmoDef
        cls = mb.class_('CAmmoDef')
        cls.include()
        cls.member_function('GetAmmoOfIndex').exclude()
        cls.variable('m_AmmoType').exclude()
        cls.variable('m_nAmmoIndex').rename('ammoindex')
        
        mb.free_function('GetAmmoDef').include()
        mb.free_function('GetAmmoDef').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        
        # Remove any protected function 
        mb.calldefs( matchers.access_type_matcher_t('protected') ).exclude()
        
        # Finally apply common rules to all includes functions and classes, etc.
        self.ApplyCommonRules(mb)
