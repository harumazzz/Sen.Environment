#!/bin/sh
cd ./
mkdir ./build
cd ./build

if [ "$(uname)" = "Linux" ]; then
    cmake -DCMAKE_BUILD_TYPE=Release .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON
elif [ "$(uname)" = "Darwin" ]; then
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang-19 -DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++ ..
fi

make
