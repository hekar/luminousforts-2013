from srcpy.module_generators import SemiSharedModuleGenerator

class _SrcTests(SemiSharedModuleGenerator):
    module_name = '_srctests'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_tests.cpp',
        '$SRCDIR\game\shared\python\srcpy_tests.h',
    ]

    files = [
        'cbase.h',
        'srcpy_tests.h'
    ]
    
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude() 

        mb.free_function('SrcPyTest_EntityArg').include()
        mb.free_function('SrcPyTest_ExtractEntityArg').include()
