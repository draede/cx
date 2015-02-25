rmdir /s /q ".\Bin"
rmdir /s /q ".\Lib"
rmdir /s /q ".\Temp"

del /q ".\Build\VS2013\*.sdf"
del /q ".\Build\VS2013\*.opensdf"
del /q ".\Build\VS2013\*.suo"
del /q ".\Build\VS2013\*.user"

del /q ".\Test\Test1\Build\VS2013\*.sdf"
del /q ".\Test\Test1\Build\VS2013\*.opensdf"
del /q ".\Test\Test1\Build\VS2013\*.suo"
del /q ".\Test\Test1\Build\VS2013\*.user"

del /q ".\Test\UnitTest\Build\VS2013\*.sdf"
del /q ".\Test\UnitTest\Build\VS2013\*.opensdf"
del /q ".\Test\UnitTest\Build\VS2013\*.suo"
del /q ".\Test\UnitTest\Build\VS2013\*.user"
