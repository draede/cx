SET DIR=%~dp0
rmdir /q /s "%DIR%Temp\BIN"
mkdir "%DIR%Temp\BIN"
xcopy "%DIR%Bin\*cx.pdb" "%DIR%Temp\BIN\Bin\" /sy
xcopy "%DIR%Bin\*cx.dll" "%DIR%Temp\BIN\Bin\" /sy
xcopy "%DIR%Lib\*cx.pdb" "%DIR%Temp\BIN\Lib\" /sy
xcopy "%DIR%Lib\*cx.lib" "%DIR%Temp\BIN\Lib\" /sy
xcopy "%DIR%Include" "%DIR%Temp\BIN\Include\" /sy
xcopy "%DIR%Contrib\*.h" "%DIR%Temp\BIN\Contrib\" /sy
xcopy "%DIR%Contrib\*.hpp" "%DIR%Temp\BIN\Contrib\" /sy
FOR /D /R "%DIR%Temp\BIN\Contrib" %%X IN (Src) DO RMDIR /S /Q "%%X" 2> NUL
"%DIR%Tool\Bin\7za.exe" a -t7z -mx9 "%DIR%Temp\cx-bin.7z" "%DIR%Temp\BIN\*"
rmdir /q /s "%DIR%Temp\BIN"