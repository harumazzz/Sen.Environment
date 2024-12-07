# Sen: Environment

Additional modules to Sen.

## Project development

-   [Flutter 3.24](https://docs.flutter.dev/get-started/install)

-   Exploration:

    > See Flutter official documentation for more details:
    > [Flutter Document](https://docs.flutter.dev/get-started/editor)

-   Dependencies: See `pubspec.yaml` for more details.

-   Before compilation run these command lines first:

```ps1
	flutter pub get
```

-   Build command lines:

    -   Android: `flutter build apk --target-platform android-arm64`
    -   Windows: `flutter build windows`
    -   Linux: `flutter build linux`
    -   MacOS: `flutter build macos`
    -   iOS: `flutter build ios`

-   Android Development Special:

    -   For android, you need to compile
        [Kernel](https://github.com/Haruma-VN/Sen.Environment/tree/master/Kernel) yourself, put it
        in `android/app/src/main/jniLibs/arm64-v8a/` as `libKernel.so` along with `libc++_shared.so`
        from Android Studio NDK.

    -   The `libc++_shared.so` is taken from:
        `<NDK_PATH>/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so`.

        > The version libc++\_shared.so included in the current project is NDK r27c.

        > See the
        > [Android documentation](https://source.android.com/docs/core/architecture/vndk/linker-namespace)
        > for details.

    -   CMake Project Setup:

        -   Generator: Ninja

        -   ANDROID_PLATFORM: 30

        -   ANDROID_ABI: arm64-v8a

        -   ANDROID_STL: c++\_shared

        -   Compiler:
            [Clang 18.0.3 for Android (NDK r27c)](https://developer.android.com/ndk/downloads)

## Localization

-   If you want to add language to this project, please add an `app_[language-code].arb`.

-   Would be more convenient if you make a pull request and merge with the current project.

## Splash Screen

-   In order to build native splash screen for `iOS` and `Android`, please run this command:
    `dart run flutter_native_splash:create --path=native_splash.yaml`

-   All configuration has been made in `native_splash.yaml`, however if you want better splash
    screen, please change it as your wish.
