setlocal
set PRJDIR=%~dp0..\..

REM remove temp/lib dirs
if exist "%PRJDIR%\Temp\NDK" rmdir /s /q "%PRJDIR%\Temp\NDK"
if exist "%PRJDIR%\Lib\NDK\ARMEABI\Release" rmdir /s /q "%PRJDIR%\Lib\NDK\ARMEABI\Release"
if exist "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Release" rmdir /s /q "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Release"

REM compile
call ndk-build -j2
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM create lib dirs
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

REM copy libs to Lib folder
copy /y "%PRJDIR%\Temp\NDK\local\armeabi\*.a" "%PRJDIR%\Lib\NDK\ARMEABI\Release\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
copy /y "%PRJDIR%\Temp\NDK\local\armeabi-v7a\*.a" "%PRJDIR%\Lib\NDK\ARMEABI-V7A\Release\"
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%


endlocal
