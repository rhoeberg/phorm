SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community

IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvars64.bat" %1))

set includeFlags=-I ..\3rdparty\glfw\include -I ..\3rdparty\glew\include -I ..\3rdparty\portaudio\include -I ..\3rdparty 
set libraryFlags=glfw3dll.lib OpenGL32.lib glew32.lib portaudio_x64.lib /link /LIBPATH:"..\3rdparty\glfw\lib" /LIBPATH:"..\3rdparty\glew\lib\Release\x64" /LIBPATH:"..\3rdparty" /LIBPATH:"..\3rdparty\portaudio\lib"

IF NOT EXIST build (
   mkdir build
)
xcopy 3rdparty\portaudio\lib\portaudio_x64.dll build\ /Y
xcopy 3rdparty\glew\bin\Release\x64\glew32.dll build\ /Y
xcopy 3rdparty\glfw\lib\glfw3.dll build\ /Y
xcopy assets build\assets\ /Y/E/H/C/I
 
pushd build
cl -MD -FC -Zi -EHsc ..\code\main.cpp /Fetest.exe %includeFlags% %libraryFlags%
popd
