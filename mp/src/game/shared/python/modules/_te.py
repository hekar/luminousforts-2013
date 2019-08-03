from srcpy.module_generators import SemiSharedModuleGenerator
from srcpy.matchers import calldef_withtypes

from pyplusplus.module_builder import call_policies
from pygccxml.declarations import matchers, pointer_t, const_t, reference_t, declarated_t

class TE(SemiSharedModuleGenerator):
    module_name = '_te'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_te.cpp',
        '$SRCDIR\game\shared\python\srcpy_te.h',
    ]

    module_symbols = ['SRCPY_MOD_TE']

    files = [
        'cbase.h',
        'effect_dispatch_data.h',
        'srcpy_te.h',
        
        '$tempent.h',
        '$c_te_legacytempents.h',
        '$c_te_effect_dispatch.h',
        '$fx.h',
        '$fx_quad.h',
        '$fx_line.h',
        '$clientsideeffects.h',
        '$fx_envelope.h',
        #'$c_strider_fx.h',
        
        '#te_effect_dispatch.h',
    ]
    
    def ParseServer(self, mb):
        mb.class_('CEffectData').variables('m_nEntIndex').exclude()
        
        mb.free_function('CreateConcussiveBlast').include()
        
    def ParseClient(self, mb):
        # Don't care
        mb.class_('CEffectData').member_functions().exclude()
        mb.class_('CEffectData').variables('m_hEntity').exclude()
        
        # Registering new effects
        cls = mb.class_('PyClientEffectRegistration')
        cls.include()
        cls.rename('ClientEffectRegistration')
        cls.variables().exclude()
        
        # Functions to do some effects
        mb.free_functions('FX_AddQuad').include()
        
        # fx.h
        mb.free_functions('FX_RicochetSound').include()
        mb.free_functions('FX_AntlionImpact').include()
        mb.free_functions('FX_DebrisFlecks').include()
        mb.free_functions('FX_Tracer').include()
        mb.free_functions('FX_GunshipTracer').include()
        mb.free_functions('FX_StriderTracer').include()
        mb.free_functions('FX_HunterTracer').include()
        mb.free_functions('FX_PlayerTracer').include()
        #mb.free_functions('FX_BulletPass').include()
        mb.free_functions('FX_MetalSpark').include()
        mb.free_functions('FX_MetalScrape').include()
        mb.free_functions('FX_Sparks').include()
        mb.free_functions('FX_ElectricSpark').include()
        mb.free_functions('FX_BugBlood').include()
        mb.free_functions('FX_Blood').include()
        #mb.free_functions('FX_CreateImpactDust').include()
        mb.free_functions('FX_EnergySplash').include()
        mb.free_functions('FX_MicroExplosion').include()
        mb.free_functions('FX_Explosion').include()
        mb.free_functions('FX_ConcussiveExplosion').include()
        mb.free_functions('FX_DustImpact').include()
        mb.free_functions('FX_MuzzleEffect').include()
        mb.free_functions('FX_MuzzleEffectAttached').include()
        mb.free_functions('FX_StriderMuzzleEffect').include()
        mb.free_functions('FX_GunshipMuzzleEffect').include()
        mb.free_functions('FX_Smoke').include()
        mb.free_functions('FX_Dust').include()
        #mb.free_functions('FX_CreateGaussExplosion').include()
        mb.free_functions('FX_GaussTracer').include()
        mb.free_functions('FX_TracerSound').include()

        # Temp Ents
        cls = mb.class_('CTempEnts')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        cls.calldefs(matchers.access_type_matcher_t( 'protected' ), allow_empty=True).exclude()
        cls.member_function('RicochetSprite').exclude() # Exclude because of model_t

        mb.add_registration_code( 'bp::scope().attr( "tempents" ) = boost::ref(tempents);' )
        
        # C_LocalTempEntity is not exposed and shouldn't be needed (deprecated)
        localtempentity = mb.class_('C_LocalTempEntity')
        excludetypes = [
            pointer_t(declarated_t(localtempentity)),
            pointer_t(const_t(declarated_t(localtempentity))),
        ]
        cls.calldefs(calldef_withtypes(excludetypes), allow_empty=True).exclude()
        
        # Add client effects class (you can only add mesh builders to it)
        cls = mb.class_('PyClientSideEffect')
        cls.include()
        cls.rename('ClientSideEffect')
        cls.member_functions().virtuality = 'not virtual' 
        cls.member_functions('AddToEffectList').exclude()
        cls.member_functions('Draw').virtuality = 'virtual'
        
        mb.free_function('AddToClientEffectList').include()
        
        # Mesh builder
        cls = mb.class_('PyMeshVertex')
        cls.include()
        cls.rename('MeshVertex')
        
        cls.variable('m_hEnt').exclude()
        cls.member_functions('GetEnt').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        self.SetupProperty(cls, 'ent', 'GetEnt', 'SetEnt')
                         
        cls = mb.class_('PyMeshBuilder')
        cls.include()
        cls.rename('MeshBuilder')
        cls.member_functions().virtuality = 'not virtual' 
        
        cls = mb.class_('PyMeshRallyLine')
        cls.include()
        cls.rename('MeshRallyLine')
        cls.member_functions().virtuality = 'not virtual'
        cls.member_functions('GetEnt1').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        cls.member_functions('GetEnt2').call_policies = call_policies.return_value_policy(call_policies.return_by_value)
        self.SetupProperty(cls, 'ent1', 'GetEnt1', 'SetEnt1')
        self.SetupProperty(cls, 'ent2', 'GetEnt2', 'SetEnt2')
        
        mb.enumeration('MaterialPrimitiveType_t').include()
        
        # FX Envelope + strider fx
        cls = mb.class_('C_EnvelopeFX')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 
        
        #cls = mb.class_('C_StriderFX')
        #cls.include()
        #cls.member_functions().virtuality = 'not virtual' 
        
        # Constants
        mb.add_registration_code( "bp::scope().attr( \"FTENT_NONE\" ) = (int)FTENT_NONE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SINEWAVE\" ) = (int)FTENT_SINEWAVE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_GRAVITY\" ) = (int)FTENT_GRAVITY;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_ROTATE\" ) = (int)FTENT_ROTATE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SLOWGRAVITY\" ) = (int)FTENT_SLOWGRAVITY;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SMOKETRAIL\" ) = (int)FTENT_SMOKETRAIL;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_COLLIDEWORLD\" ) = (int)FTENT_COLLIDEWORLD;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_FLICKER\" ) = (int)FTENT_FLICKER;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_FADEOUT\" ) = (int)FTENT_FADEOUT;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SPRANIMATE\" ) = (int)FTENT_SPRANIMATE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_HITSOUND\" ) = (int)FTENT_HITSOUND;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SPIRAL\" ) = (int)FTENT_SPIRAL;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SPRCYCLE\" ) = (int)FTENT_SPRCYCLE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_COLLIDEALL\" ) = (int)FTENT_COLLIDEALL;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_PERSIST\" ) = (int)FTENT_PERSIST;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_COLLIDEKILL\" ) = (int)FTENT_COLLIDEKILL;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_PLYRATTACHMENT\" ) = (int)FTENT_PLYRATTACHMENT;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SPRANIMATELOOP\" ) = (int)FTENT_SPRANIMATELOOP;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_SMOKEGROWANDFADE\" ) = (int)FTENT_SMOKEGROWANDFADE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_ATTACHTOTARGET\" ) = (int)FTENT_ATTACHTOTARGET;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_NOMODEL\" ) = (int)FTENT_NOMODEL;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_CLIENTCUSTOM\" ) = (int)FTENT_CLIENTCUSTOM;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_WINDBLOWN\" ) = (int)FTENT_WINDBLOWN;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_NEVERDIE\" ) = (int)FTENT_NEVERDIE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_BEOCCLUDED\" ) = (int)FTENT_BEOCCLUDED;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_CHANGERENDERONCOLLIDE\" ) = (int)FTENT_CHANGERENDERONCOLLIDE;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_COLLISIONGROUP\" ) = (int)FTENT_COLLISIONGROUP;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_ALIGNTOMOTION\" ) = (int)FTENT_ALIGNTOMOTION;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_CLIENTSIDEPARTICLES\" ) = (int)FTENT_CLIENTSIDEPARTICLES;" )
        mb.add_registration_code( "bp::scope().attr( \"FTENT_USEFASTCOLLISIONS\" ) = (int)FTENT_USEFASTCOLLISIONS;" )
        
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude() 
        
        mb.free_functions('DispatchEffect').include()
        
        # Data for DispatchEffect
        mb.class_('CEffectData').include()
        mb.class_('CEffectData').variables('m_vOrigin').rename('origin')
        mb.class_('CEffectData').variables('m_vStart').rename('start')
        mb.class_('CEffectData').variables('m_vNormal').rename('normal')
        mb.class_('CEffectData').variables('m_vAngles').rename('angles')
        mb.class_('CEffectData').variables('m_fFlags').rename('flags')
        mb.class_('CEffectData').variables('m_flScale').rename('scale')
        mb.class_('CEffectData').variables('m_flMagnitude').rename('magnitude')
        mb.class_('CEffectData').variables('m_flRadius').rename('radius')
        mb.class_('CEffectData').variables('m_nAttachmentIndex').rename('attachmentindex')
        mb.class_('CEffectData').variables('m_nSurfaceProp').rename('surfaceprop')
        mb.class_('CEffectData').variables('m_nMaterial').rename('material')
        mb.class_('CEffectData').variables('m_nDamageType').rename('damagetype')
        mb.class_('CEffectData').variables('m_nHitBox').rename('hitbox')
        mb.class_('CEffectData').variables('m_nColor').rename('color')
        
        # ITempEntsSystem
        cls = mb.class_('ITempEntsSystem')
        cls.include()
        cls.member_functions().virtuality = 'not virtual' 

        mb.add_registration_code('bp::scope().attr( "te" ) = boost::ref(te);')
         
        if self.isserver:
            self.ParseServer(mb)
        else:
            self.ParseClient(mb)
