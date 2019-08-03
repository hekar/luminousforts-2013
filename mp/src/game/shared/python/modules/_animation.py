from srcpy.module_generators import SemiSharedModuleGenerator
from pyplusplus.module_builder import call_policies

class Animation(SemiSharedModuleGenerator):
    module_name = '_animation'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_animation.cpp',
        '$SRCDIR\game\shared\python\srcpy_animation.h',
    ]
    
    files = [
        'bone_setup.h',
        'eventlist.h',
        'animation.h',
        'ai_activity.h',
        'activitylist.h',
        'npcevent.h',
        'srcpy_animation.h',
    ]
    
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        # CStudioHdr
        cls = mb.class_('CStudioHdr')
        cls.include()
        cls.calldefs('CStudioHdr').exclude()
        #cls.no_init = True
        cls.member_functions('pszName').rename('name')
        
        cls.member_function('pSeqdesc').call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
        cls.member_function('pSeqdesc').rename('Seqdesc')
        
        # Excludes
        cls.member_function('Init').exclude()
        if self.settings.branch == 'swarm':
            cls.member_function('FindMapping').exclude()
        cls.member_function('GetSequences').exclude()
        cls.member_function('BoneFlexDriver').exclude()
        cls.member_function('GetBoneTableSortedByName').exclude()
        cls.member_function('GetRenderHdr').exclude()
        cls.member_function('GetVirtualModel').exclude()
        cls.member_function('pAnimStudioHdr').exclude()
        cls.member_function('pAnimdesc').exclude()
        cls.member_function('pBodypart').exclude()
        cls.member_function('pBone').exclude()
        cls.member_function('pBonecontroller').exclude()
        cls.member_function('pFlexRule').exclude()
        cls.member_function('pFlexcontroller').exclude()
        cls.member_function('pFlexcontrollerUI').exclude()
        cls.member_function('pFlexdesc').exclude()
        cls.member_function('pHitbox').exclude()
        cls.member_function('pHitboxSet').exclude()
        cls.member_function('pIKChain').exclude()
        cls.member_function('pLinearBones').exclude()
        cls.member_function('pSeqStudioHdr').exclude()
        #cls.member_function('pSeqdesc').exclude()
        cls.member_function('pszNodeName').exclude()
        cls.member_function('pszSurfaceProp').exclude()
        cls.member_function('SetSequenceActivity').exclude() # Declared, but no def
        cls.member_function('IsSequenceLooping').exclude() # Declared, but no def
        cls.member_function('GetSequenceCycleRate').exclude() # Declared, but no def
        cls.member_function('GetSequenceActivity').exclude() # Declared, but no def
        
        # mstudioseqdesc_t
        cls = mb.class_('mstudioseqdesc_t')
        cls.include()
        cls.no_init = True
        
        # Excludes
        cls.member_function('GetBaseMap').exclude()
        cls.variable('m_DataMap').exclude()
        cls.member_function('pActivityModifier').exclude()
        cls.member_function('pAutolayer').exclude()
        cls.member_function('pBoneweight').exclude()
        cls.member_function('pEvent').exclude()
        cls.member_function('pIKLock').exclude()
        cls.member_function('pPoseKey').exclude()
        cls.member_function('pStudiohdr').exclude()
        cls.member_function('pszActivityName').exclude()
        cls.member_function('pszLabel').exclude()
        
        mb.free_function('Py_GetSeqdescActivityName').include()
        mb.free_function('Py_GetSeqdescActivityName').rename('GetSeqdescActivityName')
        mb.free_function('Py_GetSeqdescLabel').include()
        mb.free_function('Py_GetSeqdescLabel').rename('GetSeqdescLabel')
        
        # Useful bone_setup functions
        mb.free_function('Py_Studio_GetMass').include()
        mb.free_function('Py_Studio_GetMass').rename('Studio_GetMass')
        
        # Event list
        mb.free_function('EventList_RegisterPrivateEvent').include()
        mb.free_function('EventList_IndexForName').include()
        mb.free_function('EventList_NameForIndex').include()
        mb.free_function('ResetEventIndexes').include()
        
        # Animation
        mb.free_function('ExtractBbox').include()
        mb.free_function('BuildAllAnimationEventIndexes').include()
        mb.free_function('LookupActivity').include()
        mb.free_function('LookupSequence').include()
        mb.free_function('GetSequenceName').include()
        mb.free_function('GetSequenceActivityName').include()
        mb.free_function('GetSequenceFlags').include()
        mb.free_function('GetAnimationEvent').include()
        mb.free_function('HasAnimationEventOfType').include()
        mb.free_function('SelectWeightedSequence').include()
        mb.free_function('SelectHeaviestSequence').include()
        
        # Activity
        mb.free_function('ActivityList_RegisterPrivateActivity').include()
        mb.free_function('ActivityList_IndexForName').include()
        mb.free_function('ActivityList_NameForIndex').include()
        mb.free_function('IndexModelSequences').include()
        mb.free_function('ResetActivityIndexes').include()
        mb.free_function('VerifySequenceIndex').include()
        
        # Enums
        mb.enumeration('Animevent').include()