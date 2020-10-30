mkdir build64 & pushd build64
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DDISABLE_SOURCECODE=ON -G "Visual Studio 15 2017 Win64" ..
popd
cmake --build build64 --config Release
md plugin_lua53\Plugins\Windows\x86_64
copy /Y build64\Release\xlua.dll plugin_lua53\Plugins\Windows\x86_64\xlua.dll
pause