## Usage
The modules can be generated using generatemods.py. Generation requires a unix like environment. 
On Windows you can use [Bash on Ubuntu on Windows](https://msdn.microsoft.com/en-us/commandline/wsl/about).

The following dependencies need to be installed on Ubuntu:

        sudo apt-get install castxml gcc-4.7 gcc-4.7-multilib g++-4.7 g++-4.7-multilib

All modules can be parsed by running:

        python3 generatemods.py

A single module can be generated a follows:

        python3 generatemods.py -m _entities

## Added a new module
To create a new module you must do two things:
1. Add a new python files with parse code to either one of the following folders 
(depending on whether it is intended for the server, client or is shared code):
- [mp/src/game/shared/python/modules](/mp/src/game/shared/python/modules)
- [mp/src/game/client/python/modules](/mp/src/game/client/python/modules)
- [mp/src/game/server/python/modules](/mp/src/game/server/python/modules)

2. Open settings.py and add the new binding code to the module list.

## Updating PyGCCXML library
Merge changes from https://github.com/gccxml/pygccxml

## Updating PyPlusPlus library
Merge changes from https://bitbucket.org/ompl/pyplusplus

## Modifying importlib/_bootstrap.py
Contains modified importlib._bootstrap, to allow loading modules from vpk files. This module must be frozen and included in the server/client dlls.

Steps:
1. Compile _freeze_importlib project (thirdparty/python/pcbuild/_freeze_importlib)
2. Use it on _bootstrap.py: .\_freeze_importlib.exe _bootstrap.py srcpy_importlib.h
3. Update shared/python/srcpy_importlib.h and recompile game