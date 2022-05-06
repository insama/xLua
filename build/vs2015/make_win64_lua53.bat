mkdir build64 & pushd build64
cmake -G "Visual Studio 14 2015 Win64" ..
IF %ERRORLEVEL% NEQ 0 cmake -G "Visual Studio 16 2019" -A "Win64"
popd
cmake --build build64 --config Release
md plugin_lua53\Plugins\x86_64
copy /Y build64\Release\xlua.dll plugin_lua53\Plugins\x86_64\xlua.dll
pause