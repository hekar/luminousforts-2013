from srcpy.module_generators import SemiSharedModuleGenerator

from pyplusplus.module_builder import call_policies
from pygccxml.declarations import matchers

class Materials(SemiSharedModuleGenerator):
    module_name = 'materials'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_materials.cpp',
        '$SRCDIR\game\shared\python\srcpy_materials.h',
    ]

    module_symbols = ['SRCPY_MOD_MATERIALS']

    files = [
        'cbase.h',
        'materialsystem/imaterial.h',
        'materialsystem/MaterialSystemUtil.h',
        'python/srcpy_materials.h',
        'view_shared.h',
        '$cdll_client_int.h',
        '$viewpostprocess.h',
        '$glow_outline_effect.h',
    ]

    def Parse(self, mb):
        mb.decls().exclude()  

        # Material reference
        cls = mb.class_('CMaterialReference')
        cls.include()
        mb.global_ns.member_operators('*').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        mb.global_ns.member_operators().exclude()
        mb.global_ns.casting_operators().exclude()

        if self.isclient:
            # Glow Outline manager
            cls = mb.class_('CGlowObjectManager')
            cls.include()
            cls.member_functions().virtuality = 'not virtual'
            cls.no_init = True
            
            mb.add_registration_code( "bp::scope().attr( \"glowobjectmanager\" ) = boost::ref(g_GlowObjectManager);" )
            
            # A way for creating procedural materials in Python
            cls = mb.class_('PyProceduralTexture')
            cls.rename('ProceduralTexture')
            cls.include()
            
            mb.enumeration('ImageFormat').include()
            
            # Material lights
            cls = mb.class_('LightDesc_t')
            cls.include()
        
        # Remove any protected function 
        mb.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
