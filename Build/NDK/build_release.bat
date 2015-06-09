setlocal
set PRJDIR=%~dp0..\..
call ndk-build -j2 APP_OPTIM=release
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib" mkdir "%PRJDIR%\Lib"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK" mkdir "%PRJDIR%\Lib\NDK"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI" mkdir "%PRJDIR%\Lib\NDK\ARMEABI"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI\Release" mkdir "%PRJDIR%\Lib\NDK\ARMEABI\Release"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI-V7A" mkdir "%PRJDIR%\Lib\NDK\ARMEABI-V7A"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
if not exist "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Release" mkdir "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Release"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Build\NDK\obj\local\armeabi\libcx.a" "%PRJDIR%\Lib\NDK\ARMEABI\Release\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Build\NDK\obj\local\armeabi-v7a\libcx.a" "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Release\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
rmdir /s /q "%PRJDIR%\Build\NDK\obj"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
endlocal