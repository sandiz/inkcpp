cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config Release --target clean
cmake --build . --config Release
cmake --install . --config Release --component lib --prefix='C:\Users\sandi\Documents\Unreal~1\AHF\Source\ThirdParty\libink\'