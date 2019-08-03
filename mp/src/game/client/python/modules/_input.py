from srcpy.module_generators import ClientModuleGenerator

from pyplusplus.module_builder import call_policies
from pyplusplus import function_transformers as FT
from pygccxml.declarations import matchers


class Input(ClientModuleGenerator):
    module_name = '_input'

    files = [
        'vgui/Cursor.h',
        'inputsystem/ButtonCode.h',
        'kbutton.h',
        'cbase.h',
        'input.h',
    ]

    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        mb.class_('kbutton_t').include()
        if self.settings.branch == 'swarm':
            mb.class_('kbutton_t').member_functions('GetPerUser').exclude()
        
        # Include input class
        cls = mb.class_('CInput')
        cls.include()
        cls.member_functions().virtuality = 'not virtual'
        if self.settings.branch == 'swarm':
            #mb.member_functions('FindKey').exclude() # FIXME
            mb.member_functions('FindKey').call_policies = call_policies.return_value_policy( call_policies.reference_existing_object ) 
        else:
            mb.member_functions('FindKey').call_policies = call_policies.return_value_policy( call_policies.manage_new_object )  
        mb.member_functions('GetUserCmd').call_policies = call_policies.return_value_policy( call_policies.reference_existing_object ) # Can't exclude due being abstract
        
        mb.member_functions( 'CAM_OrthographicSize' ).add_transformation( FT.output('w'), FT.output('h') )
        
        mb.add_declaration_code( "CInput *wrap_Input()\r\n{\r\n\treturn (CInput *)::input;\r\n}\r\n" )
        mb.add_registration_code( 'bp::def( "input", wrap_Input, bp::return_value_policy<bp::reference_existing_object>() );' )
        
        # ButtonCode.  
        mb.enumerations('ButtonCode_t').include()
        
        # Remove any protected function 
        mb.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()
