from srcpy.module_generators import SemiSharedModuleGenerator

from pyplusplus.module_builder import call_policies
from pyplusplus import code_creators

class Physics(SemiSharedModuleGenerator):
    module_name = '_physics'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_physics.cpp',
        '$SRCDIR\game\shared\python\srcpy_physics.h',
    ]

    files = [
        'cbase.h',
        'srcpy_physics.h',
        'vphysics/friction.h',
        'physics.h',
        '#physics_impact_damage.h',
        'vphysics_interface.h',
        'physics_shared.h',
    ]
    
    def ParsePhysicObjects(self, mb):
        # Base Wrapper
        cls = mb.class_('PyPhysicsObject')
        cls.rename('PhysicsObject')
        cls.include()
        cls.calldefs().virtuality = 'not virtual'   
        cls.member_functions('Cmp').rename('__cmp__')
        cls.member_functions('NonZero').rename('__nonzero__')
        cls.member_functions('Bool').rename('__bool__')
        
        cls.member_function('CheckValid').exclude()
        cls.member_functions('GetPySelf').exclude()  
        cls.add_wrapper_code('virtual PyObject *GetPySelf() const { return boost::python::detail::wrapper_base_::get_owner(*this); }')
        
        cls.member_functions('SetEntity').exclude()
        cls.member_functions('GetVPhysicsObject').exclude()
        cls.member_functions('InitFromPhysicsObject').exclude()
        
        mb.add_registration_code( "ptr_IPhysicsObject_to_PyPhysicsObject();" )
        mb.add_registration_code( "const_ptr_IPhysicsObject_to_PyPhysicsObject();" )
        mb.add_registration_code( "PyPhysicsObject_to_IPhysicsObject();" )

        # Shadow controller
        cls = mb.class_('PyPhysicsShadowController')
        cls.rename('PhysicsShadowController')
        cls.include()
        cls.calldefs().virtuality = 'not virtual'   
        cls.member_function('CheckValid').exclude()
        cls.member_functions('Cmp').rename('__cmp__')
        cls.member_functions('NonZero').rename('__nonzero__')
        cls.member_functions('Bool').rename('__bool__')
        
    def ParsePhysicCollision(self, mb):
        cls = mb.class_('PyPhysicsCollision')
        cls.rename('PhysicsCollision')
        cls.include()
        cls.no_init = True
        mb.add_registration_code( "bp::scope().attr( \"physcollision\" ) = boost::ref(pyphyscollision);" )
        
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        # Linux model_t fix ( correct? )
        mb.add_declaration_code( '#ifdef _LINUX\r\n' + \
                             'typedef struct model_t {};\r\n' + \
                             '#endif // _LINUX\r\n'
                           )
                           
        self.ParsePhysicObjects(mb)
        self.ParsePhysicCollision(mb)
        
        # Add useful functions
        mb.free_function('PyPhysModelCreateBox').include()
        mb.free_function('PyPhysModelCreateBox').rename('PhysModelCreateBox')
        mb.free_function('PyPhysModelCreateOBB').include()
        mb.free_function('PyPhysModelCreateOBB').rename('PhysModelCreateOBB')
        if self.settings.branch == 'swarm':
            mb.free_function('PyPhysModelCreateSphere').include()
            mb.free_function('PyPhysModelCreateSphere').rename('PhysModelCreateSphere')
        mb.free_function('PyPhysDestroyObject').include()
        mb.free_function('PyPhysDestroyObject').rename('PhysDestroyObject')
        mb.free_function('PyGetCollideAABB').include()
        mb.free_function('PyGetCollideAABB').rename('GetCollideAABB')
        
        if self.isserver:
            mb.free_function('PyPhysCallbackImpulse').include()
            mb.free_function('PyPhysCallbackImpulse').rename('PhysCallbackImpulse')
            mb.free_function('PyPhysCallbackSetVelocity').include()
            mb.free_function('PyPhysCallbackSetVelocity').rename('PhysCallbackSetVelocity')
            mb.free_functions('PyPhysCallbackDamage').include()
            mb.free_functions('PyPhysCallbackDamage').rename('PhysCallbackDamage')
            mb.free_function('PyPhysCallbackRemove').include()
            mb.free_function('PyPhysCallbackRemove').rename('PhysCallbackRemove')
            mb.free_function('PhysIsInCallback').include()
        
        # CCollisionProperty
        cls = mb.class_('CCollisionProperty')
        cls.include()
        cls.no_init = True
        cls.calldefs().virtuality = 'not virtual' 
        cls.member_functions('GetCollisionModel').call_policies = call_policies.return_value_policy( call_policies.return_by_value )  
        cls.member_functions('GetRootParentToWorldTransform').call_policies = call_policies.return_value_policy( call_policies.return_by_value )  
        cls.member_functions('GetOuter').call_policies = call_policies.return_value_policy( call_policies.return_by_value )  

        mb.member_functions('GetEntityHandle').exclude()
        mb.member_functions('GetIClientUnknown').exclude()
        mb.member_functions('GetBaseMap').exclude()
        mb.member_functions('GetDataDescMap').exclude()
        #mb.member_functions('Init').exclude()
        if self.isclient:
            mb.member_functions('GetPredDescMap').exclude()
            mb.variables('m_PredMap').exclude()
            
        mb.enumeration('SurroundingBoundsType_t').include()
            
        # PyPhysicsSurfaceProps
        mb.class_('surfacedata_t').include()
        mb.class_('surfacephysicsparams_t').include()
        mb.class_('surfacephysicsparams_t').include()
        mb.class_('surfaceaudioparams_t').include()
        mb.class_('surfacesoundnames_t').include()
        mb.class_('surfacegameprops_t').include()
        mb.class_('surfacesoundhandles_t').include()
        
        cls = mb.class_('PyPhysicsSurfaceProps')
        cls.rename('PhysicsSurfaceProps')
        
        cls.include()
        cls.no_init = True
        cls.calldefs().virtuality = 'not virtual' 
        mb.add_registration_code( "bp::scope().attr( \"physprops\" ) = boost::ref(pyphysprops);" )
        
        if self.isserver:
            mb.free_function('PyCalculateDefaultPhysicsDamage').include()
            mb.free_function('PyCalculateDefaultPhysicsDamage').rename('CalculateDefaultPhysicsDamage')
            
        # Physics Hook
        #mb.free_function('PhysicsGameSystem').include()
        #mb.free_function('PhysicsGameSystem').call_policies = call_policies.return_value_policy( call_policies.reference_existing_object )
        
        mb.free_function('PyForcePhysicsSimulate').include()
        mb.free_function('PyForcePhysicsSimulate').rename('ForcePhysicsSimulate')
        
        if self.isserver:
            mb.free_function('Physics_RunThinkFunctions').include()
            
    def AddAdditionalCode(self, mb):
        header = code_creators.include_t( 'srcpy_physics_converters.h' )
        mb.code_creator.adopt_include(header)
        super(Physics, self).AddAdditionalCode(mb)
