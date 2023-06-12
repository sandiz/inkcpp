#!/bin/bash

argument=$(echo "$1" | tr '[:upper:]' '[:lower:]')

# Set the build configuration based on the argument
if [[ $argument == "debug" ]] 
then
    configuration="Debug"
elif [[ $argument == "release" ]]
then
    configuration="Release"
else
    configuration="Release"
fi

path='/Users/sandi/Projects/AHF/Source/ThirdParty/libink/osx64/'


echo "Building inkcpp $configuration version..."
cd build
cmake .. -DCMAKE_BUILD_TYPE=$configuration -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config $configuration --target clean
cmake --build . --config $configuration

echo "Moving built files to $path."
cmake --install . --component lib --prefix=$path
