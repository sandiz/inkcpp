#!/bin/bash

cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config Release --target clean
cmake --build . --config Release
cmake --install . --component lib --prefix='/Users/sandi/Projects/AHF/Source/ThirdParty/libink/osx64/'
