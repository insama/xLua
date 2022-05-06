mkdir build64 & pushd build64
cmake -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 16 2019" -A "Win64"
IF %ERRORLEVEL% NEQ 0 cmake -DLUAC_COMPATIBLE_FORMAT=ON -G "Visual Studio 16 2019" -A "Win64"
popd
cmake --build build64 --config Release
md luac_lua53\win
copy /Y build64\Release\luac.exe luac_lua53\win\luac.exe
pause