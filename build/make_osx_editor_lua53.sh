mkdir -p build_osx_editor && cd build_osx_editor
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DDISABLE_SOURCECODE=OFF -GXcode ../
cd ..
cmake --build build_osx_editor --config Release
mkdir -p plugin_lua53/Plugins/Editor/OSX/xlua.bundle/Contents/MacOS/
cp build_osx_editor/Release/xlua.bundle/Contents/MacOS/xlua plugin_lua53/Plugins/Editor/OSX/xlua.bundle/Contents/MacOS/xlua

