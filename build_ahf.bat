cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config Release --target clean
cmake --build . --config Release
cmake --install . --component lib --prefix='E:\UnrealProjects\AHF\Source\ThirdParty\libink\win64'
