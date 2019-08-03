
rem @echo off

@rem Get steam dir
Set Reg.Key=HKEY_CURRENT_USER\Software\Valve\Steam
Set Reg.Val=SteamPath

For /F "Tokens=2*" %%A In ('Reg Query "%Reg.Key%" /v "%Reg.Val%" ^| Find /I "%Reg.Val%"' ) Do Call Set steamdir=%%B
echo %steamdir%

PATH=PATH;"%steamdir%/SteamApps/common/Source SDK Base 2013 Multiplayer/bin"
cd /home/hekar/code/luminousforts-2013/mp/src/thirdparty
vpk.exe -M a pysource_lib "@/home/hekar/code/luminousforts-2013/mp/src/thirdparty/vpk_list.txt"