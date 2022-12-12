#!/bin/bash
# 2022/01/30  by mky

test -e build/ || mkdir build
#判断build目录是否存在 ,如果不存在则创建
echo "mkdir build/"

test -e target/ || mkdir target
#判断bin目录是否存在 ,如果不存在则创建
echo "mkdir target/"

rm -rf target/*
rm -rf build/*
#清一下文件夹
pwd
#cd build/

#rm -rf *
# DBUILD_TESTS = "OFF"
# BUILD_TYPE = "Debug"
# DCMAKE_TOOLCHAIN_FILE = "toolchain/x86-toolchain.cmake"
# 指定linux armv7-a 交叉编译工具链
#/opt/cmake-3.21.1/bin/cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../platforms/linux/rv1126-toolchain-mky.cmake ..
# if [ ! $1 ];then
#     echo -e "Using default toolchain: \e[32mPC.\e[39m"
# else
#     DCMAKE_TOOLCHAIN_FILE=$1
#     echo -e "Using toolchain: \e[32m$toolchain\e[39m"
# fi
# if [ ! $2 ];then
#     echo -e "Using default buildtype: \e[32mDebug.\e[39m"
# else
#     BUILD_TYPE=$2
#     echo -e "build type: \e[32m$buildtype\e[39m"
# fi


cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/x86-toolchain.cmake -DCMAKE_BUILD_TYPE=debug
 

# if [ $? -ne 0 ];then
#     echo -e "\033[41;30m Build Error \033[0m""]]"
#     exit 1
# fi

make -j install

# if [ $? -ne 0 ];then
#     echo -e "\033[41;30m Build Error \033[0m""]]"
#     exit 1
# fi

cmake --build build/ -j --target install