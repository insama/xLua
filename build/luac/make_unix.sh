mkdir -p build_unix && cd build_unix
cmake -DLUAC_COMPATIBLE_FORMAT=ON ../
cd ..
cmake --build build_unix --config Release
mkdir -p luac_lua53/unix/
cp build_unix/luac luac_lua53/unix/luac 
