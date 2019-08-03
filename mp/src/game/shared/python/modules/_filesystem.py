from srcpy.module_generators import SharedModuleGenerator

class SrcFilesystem(SharedModuleGenerator):
    module_name = '_filesystem'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_filesystem.cpp',
        '$SRCDIR\game\shared\python\srcpy_filesystem.h',
        '$SRCDIR\game\shared\python\srcpy_importlib.h',
    ]

    module_symbols = ['SRCPY_MOD_FILESYSTEM']

    files = [
        'filesystem.h',
        'srcpy_filesystem.h',
    ]
    
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()
        
        mb.typedef('FileNameHandle_t').include()
        
        # All functions named 'PyFS_' are part of the _filessytem module
        # Just strip the "PyFS_' suffix and include the function
        for decl in mb.free_functions():
            if decl.name.startswith('PyFS_'):
                decl.include()
                decl.rename(decl.name.split('PyFS_')[1])
