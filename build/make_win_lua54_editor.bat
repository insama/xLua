mkdir build64_54_editor & pushd build64_54_editor
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DLUA_VERSION=5.4.1 -G "Visual Studio 16 2019 -A Win64" ..
popd
cmake --build build64_54_editor --config Release
md plugin_lua54\Plugins\Editor\Windows\x86_64
copy /Y build64_54_editor\Release\xlua.dll plugin_lua54\Plugins\Editor\Windows\x86_64\xlua.dll

mkdir build32_54_editor & pushd build32_54_editor
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DLUA_VERSION=5.4.1 -G "Visual Studio 16 2019" ..
popd
cmake --build build32_54_editor --config Release
md plugin_lua54\Plugins\Editor\Windows\x86
copy /Y build32_54_editor\Release\xlua.dll plugin_lua54\Plugins\Editor\Windows\x86\xlua.dll

pause