from srcpy.module_generators import SemiSharedModuleGenerator

from pygccxml.declarations import matchers

class VProf(SemiSharedModuleGenerator):
    module_name = 'vprof'
    
    files = [
        'vprof.h'
    ]

    def Parse(self, mb):
        # Exclude everything, then add what we need
        mb.decls().exclude() 
        
        mb.class_('CVProfile').include()
        
        mb.member_functions('GetRoot').exclude()
        mb.member_functions('FindNode').exclude()
        mb.member_functions('GetBudgetGroupColor').exclude()
        mb.member_functions('RegisterNumBudgetGroupsChangedCallBack').exclude()
        mb.member_functions('FindOrCreateCounter').exclude()
        mb.member_functions('GetCounterNameAndValue').exclude()
        if self.settings.branch == 'source2013':
                mb.member_functions('GetCurrentNode').exclude()
        mb.variables('m_pNumBudgetGroupsChangedCallBack').exclude()
        mb.variables('m_pName').exclude()
        
        mb.variables('g_VProfCurrentProfile').include()
        mb.variables('g_VProfCurrentProfile').rename('vprofcurrentprofilee')
        
        # Remove any protected function 
        mb.calldefs( matchers.access_type_matcher_t( 'protected' ) ).exclude()    