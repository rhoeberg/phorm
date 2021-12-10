SET VC_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community
IF NOT DEFINED LIB (IF EXIST "%VC_PATH%" (call "%VC_PATH%\VC\Auxiliary\Build\vcvars32.bat" %1))

cl code\meta.cpp /Femeta.exe
.\meta.exe
DEL .\meta.exe
DEL .\meta.obj
