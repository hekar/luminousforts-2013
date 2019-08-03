''' Creates the vpk containing Python standard library. '''

import os
import subprocess
import shutil
import distutils.dir_util
import glob

basescript = r'''
rem @echo off

@rem Get steam dir
Set Reg.Key=HKEY_CURRENT_USER\Software\Valve\Steam
Set Reg.Val=SteamPath

For /F "Tokens=2*" %%A In ('Reg Query "%Reg.Key%" /v "%Reg.Val%" ^| Find /I "%Reg.Val%"' ) Do Call Set steamdir=%%B
echo %steamdir%

PATH=PATH;"%steamdir%/SteamApps/common/Source SDK Base 2013 Multiplayer/bin"
'''

target_folder = 'python/lib'
file_types = ['.py']

# Patch target folder
distutils.dir_util.copy_tree('../srcpypp/srcpy_patchlib', target_folder)

response_path = os.path.join(os.getcwd(),"vpk_list.txt")
script_path = os.path.join(os.getcwd(),"vpk_script.bat")

len_cd = len(os.getcwd()) + 1
 
with open(response_path,'wt') as fp:
    for root, dirs, files in os.walk(os.path.join(os.getcwd(), target_folder)):
        for file in files:
            fp.write(os.path.join(root[len_cd:].replace("/","\\"),file) + "\n")
                    
for filename in glob.glob('./pysource_lib_*.vpk'):
    if os.path.exists(filename):
        os.remove(filename)
                    
with open(script_path, 'w') as fp:
    fp.write(basescript)
    fp.write('cd %s\n' % os.getcwd())
    fp.write('vpk.exe -M a pysource_lib "@%s"' % response_path)
subprocess.call([script_path])
