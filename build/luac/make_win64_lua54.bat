mkdir build64_54 & pushd build64_54
cmake -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 17 2019 Win64" ..
IF %ERRORLEVEL% NEQ 0 cmake -DLUAC_COMPATIBLE_FORMAT=ON -DLUA_VERSION=5.4.1 -G "Visual Studio 17 2019 Win64" ..
popd
cmake --build build64_54 --config Release
md luac_lua54\win
copy /Y build64_54\Release\luac.exe luac_lua54\win\luac.exe
pause