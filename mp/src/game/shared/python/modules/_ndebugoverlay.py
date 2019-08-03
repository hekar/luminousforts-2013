from srcpy.module_generators import SemiSharedModuleGenerator
from pyplusplus import messages

class NDebugOverlay(SemiSharedModuleGenerator):
    module_name = '_ndebugoverlay'
    
    files = [
        'cbase.h',
        'debugoverlay_shared.h'
    ]
    
    def Parse(self, mb):
        # Exclude everything, then add what we need
        mb.decls().exclude()  
        
        mb.add_declaration_code( 'class CBaseEntity;\r\n')
        
        mb.namespace('NDebugOverlay').include()
        mb.free_functions('DrawOverlayLines').exclude()

        # Silent warnings of generating class wrappers
        mb.classes().disable_warnings( messages.W1027 )
