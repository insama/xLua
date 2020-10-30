mkdir build32 & pushd build32
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DDISABLE_SOURCECODE=ON -G "Visual Studio 15 2017" ..
popd
cmake --build build32 --config Release
md plugin_lua53\Plugins\Windows\x86
copy /Y build32\Release\xlua.dll plugin_lua53\Plugins\Windows\x86\xlua.dll
pause