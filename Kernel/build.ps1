cd ./
mkdir ./build
cd ./build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR_PLATFORM=x64 ..
cmake --build . --config Release
cd ..
