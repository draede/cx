setlocal
set PRJDIR=%~dp0..\..

REM remove temp/lib dirs
if exist "%PRJDIR%\Temp\NDK" rmdir /s /q "%PRJDIR%\Temp\NDK"
if exist "%PRJDIR%\Lib\NDK\ARMEABI\Debug" rmdir /s /q "%PRJDIR%\Lib\NDK\ARMEABI\Debug"
if exist "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Debug" rmdir /s /q "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Debug"

REM compile
call ndk-build -j2 NDK_DEBUG=1
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM create lib dirs
if not exist "%PRJDIR%\Lib" mkdir "%PRJDIR%\Lib"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK" mkdir "%PRJDIR%\Lib\NDK"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI" mkdir "%PRJDIR%\Lib\NDK\ARMEABI"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI\Debug" mkdir "%PRJDIR%\Lib\NDK\ARMEABI\Debug"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI-V7A" mkdir "%PRJDIR%\Lib\NDK\ARMEABI-V7A"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Debug" mkdir "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Debug"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\x86" mkdir "%PRJDIR%\Lib\NDK\x86"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\x86\Debug" mkdir "%PRJDIR%\Lib\NDK\x86\Debug"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%


REM copy libs to Lib folder
copy /y "%PRJDIR%\Temp\NDK\local\armeabi\*.a" "%PRJDIR%\Lib\NDK\ARMEABI\Debug\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Temp\NDK\local\armeabi-v7a\*.a" "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Debug\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Temp\NDK\local\x86\*.a" "%PRJDIR%\Lib\NDK\x86\Debug\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM copy gdb files to Lib folder
copy /y "%PRJDIR%\Build\NDK\libs\armeabi\*.*" "%PRJDIR%\Lib\NDK\ARMEABI\Debug\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Build\NDK\libs\armeabi-v7a\*.*" "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Debug\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Build\NDK\libs\x86\*.*" "%PRJDIR%\Lib\NDK\x86\Debug\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM remove libs
rmdir /s /q "%PRJDIR%\Build\NDK\libs"

endlocal
