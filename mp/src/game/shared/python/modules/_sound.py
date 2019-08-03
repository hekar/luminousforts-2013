from srcpy.module_generators import SemiSharedModuleGenerator
from pyplusplus.module_builder import call_policies
from pygccxml.declarations import matchers

class Sound(SemiSharedModuleGenerator):
    module_name = '_sound'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_sound.cpp',
        '$SRCDIR\game\shared\python\srcpy_sound.h',
    ]

    files = [
        'wchar.h',
        'string_t.h',
        'cbase.h',
        'shareddefs.h',
        'srcpy_sound.h',
        'soundflags.h',
    ]

    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        # EngineSound access
        mb.class_('PyEngineSound').include()
        mb.class_('PyEngineSound').rename('EngineSound')
        
        # soundengine should follow after PyEngineSound!
        mb.class_('PyEngineSound').add_registration_code( "}bp::scope().attr( \"soundengine\" ) = boost::ref(pysoundengine);{", False )   
        
        # EmitSound_t
        cls = mb.class_('EmitSound_t')
        cls.rename('EmitSound')
        cls.include()
        cls.variable('m_UtlVecSoundOrigin').exclude()
        
        cls.variable('m_nChannel').rename('channel')
        cls.variable('m_pSoundName').rename('soundname')
        cls.variable('m_flVolume').rename('volume')
        cls.variable('m_SoundLevel').rename('soundlevel')
        cls.variable('m_nFlags').rename('flags')
        cls.variable('m_nPitch').rename('pitch')
        cls.variable('m_pOrigin').rename('origin')
        cls.variable('m_flSoundTime').rename('soundtime')
        cls.variable('m_pflSoundDuration').rename('soundduration')
        cls.variable('m_bEmitCloseCaption').rename('emitclosecaption')
        cls.variable('m_bWarnOnMissingCloseCaption').rename('warnonmissingclosecaption')
        cls.variable('m_bWarnOnDirectWaveReference').rename('warnondirectwavereference')
        cls.variable('m_nSpeakerEntity').rename('speakerentity')
        cls.variable('m_UtlVecSoundOrigin').rename('utlvecsoundorigin')
        cls.variable('m_hSoundScriptHandle').rename('soundscripthandle')
    
        # SoundEnvelope
        mb.class_('CSoundEnvelopeControllerHandle').include()
        mb.class_('CSoundEnvelopeControllerHandle').calldefs('CSoundEnvelopeControllerHandle').exclude()
        mb.class_('CSoundEnvelopeControllerHandle').rename('CSoundEnvelopeController')
        mb.class_('CSoundPatchHandle').include()
        mb.class_('CSoundPatchHandle').rename('CSoundPatch')
        mb.member_functions('SoundCreate').call_policies = call_policies.return_value_policy( call_policies.return_by_value )
        mb.variables('m_pSoundPatch').exclude()
        mb.class_('CSoundPatchHandle').calldefs('CSoundPatchHandle').exclude()
        
        # CSoundParameters
        mb.class_('CSoundParameters').include()
        
        # Enums
        mb.enumerations('soundcommands_t').include()
        mb.enumerations('soundcommands_t').rename('soundcommands')
        mb.enumerations('soundlevel_t').include()
        mb.enumerations('soundlevel_t').rename('soundlevel')
        
        # //--------------------------------------------------------------------------------------------------------------------------------
        # Remove any protected function 
        mb.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
        
        
    def AddAdditionalCode(self, mb):
        super(Sound, self).AddAdditionalCode(mb)
        
        # Some vars
        mb.add_registration_code( "bp::scope().attr( \"PITCH_NORM\" ) = PITCH_NORM;" )
        mb.add_registration_code( "bp::scope().attr( \"PITCH_LOW\" ) = PITCH_LOW;" )
        mb.add_registration_code( "bp::scope().attr( \"PITCH_HIGH\" ) = PITCH_HIGH;" )        
        
        #mb.add_registration_code( "bp::scope().attr( \"soundengine\" ) = boost::ref(pysoundengine);" )   
