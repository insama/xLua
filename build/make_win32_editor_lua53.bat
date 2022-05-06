mkdir build32_editor & pushd build32_editor
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DDISABLE_SOURCECODE=OFF -G "Visual Studio 16 2019" ..
popd
cmake --build build32_editor --config Release
md plugin_lua53\Plugins\Editor\Windows\x86
copy /Y build32_editor\Release\xlua.dll plugin_lua53\Plugins\Editor\Windows\x86\xlua.dll
pause