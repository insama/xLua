mkdir build64_editor & pushd build64_editor
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DDISABLE_SOURCECODE=OFF -G "Visual Studio 16 2019" -A "x64" ..
popd
cmake --build build64_editor --config Release
md plugin_lua53\Plugins\Editor\Windows\x86_64
copy /Y build64_editor\Release\xlua.dll plugin_lua53\Plugins\Editor\Windows\x86_64\xlua.dll
pause