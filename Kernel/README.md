# Kernel

-   `Kernel` - Backend, the main core of the program.

-   This project is a cross-platform `C++` project that support many operating systems such:
    `Windows x64+`, `Linux x64+`, `Macintosh x64+`, `iPhone iOS 10.0+`, `Android 7+`

## Development

-   [CMake 3.8+](https://cmake.org/)

-   [Powershell for Windows](https://learn.microsoft.com/en-us/powershell/)

-   [Ubuntu for Linux](https://ubuntu.com/tutorials/command-line-for-beginners)

-   [Terminal for Macintosh](https://developer.apple.com/library/archive/documentation/OpenSource/Conceptual/ShellScripting/Introduction/Introduction.html)

-   [MSVC 19.40 with MSVC for Windows](https://visualstudio.microsoft.com/downloads/)

-   [Clang 19.1 for Linux](https://llvm.org/)

-   [Clang 19.1 for Macintosh](https://llvm.org/)

-   [Clang 18.0 for Android (NDK r27.1)](https://developer.android.com/ndk/downloads)

-   [Clang 19.1 for iPhone](https://llvm.org/)

## Subprojects

-   [quickjs](https://github.com/quickjs-ng/quickjs): JavaScript Engine used by `Kernel`

-   [libpng](http://www.libpng.org/pub/png/libpng.html): PNG Support

-   [zlib](https://www.zlib.net/): Zlib compress/uncompress

-   [fmt](https://github.com/fmtlib/fmt): Format library

-   [json](https://github.com/nlohmann/json): JSON Deserialize/Serialize

-   [tinyxml2](https://github.com/leethomason/tinyxml2): XML Deserialize/Serialize

-   [simdjson](https://simdjson.org/): JSON Deserialize

-   [open_vcdiff](https://github.com/google/open-vcdiff): Diff two file and generate the different
    after compare

-   [Rijndael](#): Rijndael encrypt/decrypt

-   [zip](https://github.com/kuba--/zip): zip compress/uncompress

-   [lzma](https://www.7-zip.org/sdk.html): lzma compress/uncompress

-   [bzip2](https://sourceware.org/bzip2): Bzip2 Compress/Uncompress

-   [avir](https://github.com/avaneev/avir): Image scaling algorithm

-   [dirent](https://github.com/tronkko/dirent): dirent for `MSVC`

-   [canvas_ity](https://github.com/a-e-k/canvas_ity/tree/main): canvas for drawing image

-   [etc1](https://developer.android.com/guide/playcore/asset-delivery/texture-compression): Google
    Standard ETC1 for encode/decode image

## Modification

-   To make sure all the libraries work perfectly fine with `Kernel`, there are some modification
    compare to original project. All modification will be added `Haruma :: Add`, `Haruma :: Change`,
    `Haruma :: Delete`

-   Special thanks to [TwinStar](https://github.com/twinstar6980) for his modification on some
    libraries which makes all the process easier.

-   All the modification has been made:

|                           Library                           |              Modification               |
| :---------------------------------------------------------: | :-------------------------------------: |
| [canvas_ity](https://github.com/a-e-k/canvas_ity/tree/main) | Fix some issues when assigning JS class |
