#!/bin/bash
rm -rf target
rm -rf build

toolchain="toolchain/x86-toolchain.cmake"
buildtype="Debug"
if [ ! $1 ];then
    echo -e "Using default toolchain: \e[32mPC.\e[39m"
else
    toolchain=$1
    echo -e "Using toolchain: \e[32m$toolchain\e[39m"
fi
if [ ! $2 ];then
    echo -e "Using default buildtype: \e[32mDebug.\e[39m"
else
    buildtype=$2
    echo -e "build type: \e[32m$buildtype\e[39m"
fi

cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=$toolchain -DCMAKE_BUILD_TYPE=$buildtype
cmake --build build/ -j --target install