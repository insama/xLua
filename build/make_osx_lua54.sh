mkdir -p build_osx_54 && cd build_osx_54
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DLUA_VERSION=5.4.1 -GXcode ../
cd ..
cmake --build build_osx_54 --config Release
mkdir -p plugin_lua54/Plugins/OSX/xlua.bundle/Contents/MacOS/
cp build_osx_54/Release/xlua.bundle/Contents/MacOS/xlua plugin_lua54/Plugins/OSX/xlua.bundle/Contents/MacOS/xlua
