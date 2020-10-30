mkdir -p build_unix_54 && cd build_unix_54
cmake -DLUAC_COMPATIBLE_FORMAT=ON -DLUA_VERSION=5.4.1 ../
cd ..
cmake --build build_unix_54 --config Release
mkdir -p luac_lua54/unix/
cp build_unix_54/luac luac_lua54/unix/luac 
