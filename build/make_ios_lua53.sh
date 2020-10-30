mkdir -p build_ios && cd build_ios
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DDISABLE_SOURCECODE=ON -DCMAKE_TOOLCHAIN_FILE=../cmake/ios.toolchain.cmake -DPLATFORM=OS64 -GXcode ../
cd ..
cmake --build build_ios --config Release
mkdir -p plugin_lua53/Plugins/iOS/
cp build_ios/Release-iphoneos/libxlua.a plugin_lua53/Plugins/iOS/libxlua.a 
