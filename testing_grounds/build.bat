@echo off
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

REM -O2 makes the compile REALLY slow
set CompilerFlags=-UTF8 -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4221 -wd4505 -wd4201 -wd4100 -wd4189 -wd4115 -wd4101 -wd4996 -FAsc -Z7 -DBLIB_DEBUG=1 -DUNICODE=1 -IW:\BLib\src\
set LinkerFlags=-incremental:no -opt:ref kernel32.lib user32.lib gdi32.lib winmm.lib d3d11.lib d3dcompiler.lib dxguid.lib dxgi.lib xaudio2.lib ole32.lib

del *.pdb > NUL 2> NUL
cl %CompilerFlags% W:\BLib\testing_grounds\main.c /link -incremental:no -opt:ref