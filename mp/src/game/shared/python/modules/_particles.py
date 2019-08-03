from srcpy.module_generators import SemiSharedModuleGenerator
from pyplusplus.module_builder import call_policies

class Particles(SemiSharedModuleGenerator):
    module_name = '_particles'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_particles.cpp',
        '$SRCDIR\game\shared\python\srcpy_particles.h',
    ]

    files = [
        '$tier1/utlvector.h',
        '$tier1/UtlSortVector.h',
        '$tier1/utlobjectreference.h',
        
        'cbase.h',
        '#gameinterface.h',
        '$particles_ez.h',
        '$icliententityinternal.h',
        '$convar.h',
        '$fx.h',
        'particle_parse.h',
        'srcpy_particles.h',
    ]
    
    def ParseClient(self, mb):
        # Dynamic lights
        mb.class_('PyDLight').include()
        mb.class_('PyDLight').rename('DLight')
        mb.class_('PyELight').include()
        mb.class_('PyELight').rename('ELight')
        
        cls = mb.class_('PyDBaseLight')
        cls.include()
        cls.rename('DBaseLight')
        cls.member_functions().exclude()
        cls.member_functions('GetColor').call_policies = call_policies.return_internal_reference() 
        cls.member_functions('GetDirection').call_policies = call_policies.return_internal_reference() 
        cls.member_functions('GetOrigin').call_policies = call_policies.return_internal_reference() 
        cls.add_property( 'origin' , cls.member_function( 'GetOrigin' ), cls.member_function( 'SetOrigin' ) )
        cls.add_property( 'color' , cls.member_function( 'GetColor' ), cls.member_function( 'SetColor' ) )
        cls.add_property( 'die' , cls.member_function( 'GetDie' ), cls.member_function( 'SetDie' ) )
        cls.add_property( 'radius' , cls.member_function( 'GetRadius' ), cls.member_function( 'SetRadius' ) )
        cls.add_property( 'flags' , cls.member_function( 'GetFlags' ), cls.member_function( 'SetFlags' ) )
        cls.add_property( 'minlight' , cls.member_function( 'GetMinlight' ), cls.member_function( 'SetMinlight' ) )
        cls.add_property( 'decay' , cls.member_function( 'GetDecay' ), cls.member_function( 'SetDecay' ) )
        cls.add_property( 'style' , cls.member_function( 'GetStyle' ), cls.member_function( 'SetStyle' ) )
        cls.add_property( 'direction' , cls.member_function( 'GetDirection' ), cls.member_function( 'SetDirection' ) )
        cls.add_property( 'innerangle' , cls.member_function( 'GetInnerAngle' ), cls.member_function( 'SetInnerAngle' ) )
        cls.add_property( 'outerangle' , cls.member_function( 'GetOuterAngle' ), cls.member_function( 'SetOuterAngle' ) )
        
        mb.add_registration_code( "bp::scope().attr( \"DLIGHT_NO_WORLD_ILLUMINATION\" ) = (int)DLIGHT_NO_WORLD_ILLUMINATION;" )
        mb.add_registration_code( "bp::scope().attr( \"DLIGHT_NO_MODEL_ILLUMINATION\" ) = (int)DLIGHT_NO_MODEL_ILLUMINATION;" )
        mb.add_registration_code( "bp::scope().attr( \"DLIGHT_ADD_DISPLACEMENT_ALPHA\" ) = (int)DLIGHT_ADD_DISPLACEMENT_ALPHA;" )
        mb.add_registration_code( "bp::scope().attr( \"DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA\" ) = (int)DLIGHT_SUBTRACT_DISPLACEMENT_ALPHA;" )
        mb.add_registration_code( "bp::scope().attr( \"DLIGHT_DISPLACEMENT_MASK\" ) = (int)DLIGHT_DISPLACEMENT_MASK;" )
        
        # Base particle
        cls = mb.class_('Particle')
        cls.include()
        cls.variables('m_Pos').rename('pos')
        cls.variable('m_pNext').exclude()
        cls.variable('m_pPrev').exclude()
        cls.variable('m_pSubTexture').exclude()
        
        # Simple particle
        mb.class_('SimpleParticle').include()
        mb.class_('SimpleParticle').variables('m_flDieTime').rename('dietime')
        mb.class_('SimpleParticle').variables('m_flLifetime').rename('lifetime')
        mb.class_('SimpleParticle').variables('m_flRoll').rename('roll')
        mb.class_('SimpleParticle').variables('m_flRollDelta').rename('rolldelta')
        mb.class_('SimpleParticle').variables('m_iFlags').rename('flags')
        mb.class_('SimpleParticle').variables('m_uchColor').rename('color')
        mb.class_('SimpleParticle').variables('m_uchEndAlpha').rename('endalpha')
        mb.class_('SimpleParticle').variables('m_uchEndSize').rename('endsize')
        mb.class_('SimpleParticle').variables('m_uchStartAlpha').rename('startalpha')
        mb.class_('SimpleParticle').variables('m_uchStartSize').rename('startsize')
        mb.class_('SimpleParticle').variables('m_vecVelocity').rename('velocity')
        mb.enumeration('SimpleParticleFlag_t').include()
        
        # Textures
        cls = mb.class_('CParticleSubTexture')
        cls.include()
        cls.no_init = True
        cls.calldefs().exclude()
        cls.variables().exclude()
        
        cls = mb.class_('CParticleSubTextureGroup')
        cls.include()
        cls.no_init = True
        cls.calldefs().exclude()
        cls.variables().exclude()
        
        # CParticleCollection
        cls = mb.class_('CParticleCollection')
        cls.include()
        cls.no_init = True
        cls.member_functions().virtuality = 'not virtual'
        cls.variables().exclude()
        cls.calldefs().exclude()
        
        cls.member_function('IsValid').include()
        
        # CNewParticleEffect
        cls = mb.class_('CNewParticleEffect')
        cls.include()
        cls.no_init = True
        cls.member_functions().virtuality = 'not virtual'
        
        cls.member_function('IsReleased').exclude()
        cls.member_functions('AddParticle').exclude()
        cls.member_functions('GetPMaterial').call_policies = call_policies.return_internal_reference() 
        cls.member_functions('ReplaceWith').exclude()
        cls.variables('m_pNext').exclude()
        cls.variables('m_pPrev').exclude()

        cls.member_functions('GetOwner').call_policies = call_policies.return_value_policy(call_policies.return_by_value) 
        
        if self.settings.branch == 'swarm':
            cls.member_functions('Create').call_policies = call_policies.return_internal_reference()
            cls.member_functions('CreateOrAggregate').call_policies = call_policies.return_internal_reference()
            cls.member_functions('CreateOrAggregatePrecached').call_policies = call_policies.return_internal_reference()
        else:
            cls.member_functions('Create').exclude() # TODO/FIXME

        #mb.class_('CNewParticleEffectHandle').include()
        #mb.class_('CNewParticleEffectHandle').member_functions('GetParticleEffect').exclude()
        
        # ParticleManager
        cls =  mb.class_('CParticleMgr')
        cls.include()
        cls.calldefs('CParticleMgr').exclude()
        cls.no_init = True
        cls.member_functions('GetPMaterial').call_policies = call_policies.return_internal_reference() 
        cls.member_functions('AllocParticle').exclude()
        cls.member_functions('CreateEffect').exclude()
        cls.member_functions('GetModelView').exclude()
        cls.member_functions('PMaterialToIMaterial').exclude()
        
        if self.settings.branch == 'swarm':
            cls.member_functions('CreateNonDrawingEffect').call_policies = call_policies.return_internal_reference()
            cls.member_functions('FirstNewEffect').call_policies = call_policies.return_internal_reference()
            cls.member_functions('NextNewEffect').call_policies = call_policies.return_internal_reference()

        mb.free_function('ParticleMgr').include()
        mb.free_function('ParticleMgr').call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
        
        # Add effects
        mb.free_function('AddSimpleParticle').include()
        mb.free_function('AddEmberParticle').include()
        mb.free_function('AddFireSmokeParticle').include()
        mb.free_function('AddFireParticle').include()

        cls = mb.class_('CParticleProperty')
        cls.include()
        cls.member_functions('Create').call_policies = call_policies.return_internal_reference() 
        cls.member_functions('GetParticleEffectFromIdx').call_policies = call_policies.return_internal_reference()
        mb.member_functions('GetOuter').call_policies = call_policies.return_value_policy( call_policies.return_by_value ) 
        mb.member_functions('GetBaseMap').exclude()
        mb.member_functions('GetDataDescMap').exclude()
        mb.member_functions('GetPredDescMap').exclude()
        if self.settings.branch == 'swarm':
            mb.member_functions('CreatePrecached').exclude()
        
        # Frequently used materials with particles
        #mb.variables('g_Mat_Fleck_Wood').include()
        #mb.variables('g_Mat_Fleck_Cement').include()
        #mb.variables('g_Mat_Fleck_Antlion').include()
        #mb.variables('g_Mat_Fleck_Tile').include()
        #mb.variables('g_Mat_DustPuff').include()
        #mb.variables('g_Mat_BloodPuff').include()
        #mb.variables('g_Mat_Fleck_Glass').include()
        #mb.variables('g_Mat_SMG_Muzzleflash').include()
        #mb.variables('g_Mat_Combine_Muzzleflash').include()
        
        #mb.add_registration_code( "ptr_newparticleeffect_to_handle();" )
        #mb.add_registration_code( "handle_to_newparticleeffect();" )      
            
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        mb.enumerations('ParticleAttachment_t').include()
        
        mb.free_functions('GetAttachTypeFromString').include()
        mb.free_functions('PrecacheParticleSystem').include()
        mb.free_functions('DispatchParticleEffect').include()
        mb.free_functions('StopParticleEffects').include()
        if self.settings.branch == 'swarm':
                mb.free_functions('StopParticleEffect').include()
        
        mb.free_functions('PyShouldLoadSheets').include()
        mb.free_functions('PyShouldLoadSheets').rename('ShouldLoadSheets')
        mb.free_functions('PyReadParticleConfigFile').include()
        mb.free_functions('PyReadParticleConfigFile').rename('ReadParticleConfigFile')
        mb.free_functions('PyDecommitTempMemory').include()
        mb.free_functions('PyDecommitTempMemory').rename('DecommitTempMemory')
        if self.isclient:
            self.ParseClient(mb)

    #def AddAdditionalCode(self, mb):
        #super(Particles, self).AddAdditionalCode(mb)
        #if self.isclient:
            #header = code_creators.include_t( 'srcpy_particles_converters.h' )
            #mb.code_creator.adopt_include(header)   
