---
title: Android Support in vcpkg
description: How to target Android devices using C++ with vcpkg
---

# Android

> [!NOTE]
> Android is not tested as part of vcpkg repository's CI process, so regressions can occur as part of library updates. PRs improving support are welcome!

## Android build requirements

1. Download the [android ndk](https://developer.android.com/ndk/downloads/)

1. Set environment variable `ANDROID_NDK_HOME` to your android ndk installation. 
   For example:

````bash
export ANDROID_NDK_HOME=/home/your-account/Android/Sdk/ndk-bundle
````

Or:
````bash
export ANDROID_NDK_HOME=/home/your-account/Android/android-ndk-r21b
````

1. Install [vcpkg](https://github.com/microsoft/vcpkg)

1. Set environment variable `VCPKG_ROOT` to your vcpkg installation.
````bash
export VCPKG_ROOT=/path/to/vcpkg
````

## vcpkg triplets and their corresponding android ABI

There are four different Android ABI, each of which maps to 
a vcpkg triplet. The following table outlines the mapping from vcpkg architectures to android architectures

|VCPKG_TARGET_TRIPLET       | ANDROID_ABI          |
|---------------------------|----------------------|
|arm64-android              | arm64-v8a            |
|arm-android                | armeabi-v7a          |
|x64-android                | x86_64               |
|x86-android                | x86                  |

## Install libraries for Android using vcpkg

Example for jsoncpp:

```bash
cd $VCPKG_ROOT

# specify the triplet like this
./vcpkg install jsoncpp --triplet arm-android
# or like this
./vcpkg install jsoncpp:arm64-android
./vcpkg install jsoncpp:x86-android
./vcpkg install jsoncpp:x64-android
```

### Using Vulkan SDK

vcpkg has a [`vulkan` package](https://github.com/microsoft/vcpkg/blob/master/ports/vulkan/portfile.cmake) which allows you to `find_package(Vulkan)`. To use it you have to provide the `VULKAN_SDK` environment variable.

```bash
export VULKAN_SDK=/usr/local
./vcpkg install vulkan
```

NDK already contains [Vulkan](https://developer.android.com/ndk/guides/graphics/getting-started) headers and `libvulkan.so` binaries for each of its architectures.

To expose them to vcpkg, you can consider `export VULKAN_SDK=...` for each installation. But by placing `set(ENV{VULKAN_SDK} ...)` in the triplet files, you can skip the tedious work.

If you are using NDK 21.3.6528147 or an earlier version, it will be like the following:

```cmake
# In android triplets... (e.g. arm64-android.cmake)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
# ...
# If your API level is 30, libvulkan.so is at $ENV{ANDROID_NDK_HOME}/platforms/android-30/arch-arm64/usr/lib
set(ENV{VULKAN_SDK} $ENV{ANDROID_NDK_HOME}/sysroot/usr)
```

Notice that **the location of the sysroot has changed since NDK 22**. (see https://github.com/android/ndk/issues/1407).

If you prefer using [the latest version](https://developer.android.com/studio/projects/install-ndk#default-ndk-per-agp), check the [BuildSystemMaintainers.md of the NDK document](https://android.googlesource.com/platform/ndk/+/master/docs/BuildSystemMaintainers.md#sysroot) and then put appropriate path for your system.

For example, Mac OS users will use the path like this example:

```cmake
# In android triplets... (e.g. arm64-android.cmake)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
# ...
# If your API level is 30, libvulkan.so is at $ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib/aarch64-linux-android/30
set(ENV{VULKAN_SDK} $ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr)
```

By doing this for all android triplets, you can install `vulkan` and the packages that require it. (e.g. `vulkan-hpp`)

<details>
  <summary>`vcpkg install vulkan-hpp:arm64-android`</summary>

```console
user@host$ ./vcpkg install vulkan-hpp:arm64-android
Computing installation plan...
The following packages will be built and installed:
  * vulkan[core]:arm64-android -> 1.1.82.1-1
    vulkan-hpp[core]:arm64-android -> 2019-05-11-1
Additional packages (*) will be modified to complete this operation.
Detecting compiler hash for triplet arm64-android...
...
Starting package 1/2: vulkan:arm64-android
Building package vulkan[core]:arm64-android...
-- Using community triplet arm64-android. This triplet configuration is not guaranteed to succeed.
-- [COMMUNITY] Loading triplet configuration from: /.../vcpkg/triplets/community/arm64-android.cmake
-- Querying VULKAN_SDK Environment variable
-- Searching /.../Library/Android/sdk/ndk/22.1.7171670/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/vulkan/ for vulkan.h
-- Found vulkan.h
-- Performing post-build validation
-- Performing post-build validation done
...
Building package vulkan[core]:arm64-android... done
Installing package vulkan[core]:arm64-android...
Installing package vulkan[core]:arm64-android... done
Elapsed time for package vulkan:arm64-android: 35.9 ms
Starting package 2/2: vulkan-hpp:arm64-android
Building package vulkan-hpp[core]:arm64-android...
-- Using community triplet arm64-android. This triplet configuration is not guaranteed to succeed.
-- [COMMUNITY] Loading triplet configuration from: /.../vcpkg/triplets/community/arm64-android.cmake
-- Using cached /.../vcpkg/downloads/KhronosGroup-Vulkan-Hpp-5ce8ae7fd0d9c0543d02f33cfa8a66e6a43e2150.tar.gz
-- Cleaning sources at /.../vcpkg/buildtrees/vulkan-hpp/src/e6a43e2150-4f344cd911.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source /.../vcpkg/downloads/KhronosGroup-Vulkan-Hpp-5ce8ae7fd0d9c0543d02f33cfa8a66e6a43e2150.tar.gz
-- Using source at /.../vcpkg/buildtrees/vulkan-hpp/src/e6a43e2150-4f344cd911.clean
-- Performing post-build validation
-- Performing post-build validation done
...
Building package vulkan-hpp[core]:arm64-android... done
Installing package vulkan-hpp[core]:arm64-android...
Installing package vulkan-hpp[core]:arm64-android... done
Elapsed time for package vulkan-hpp:arm64-android: 144.5 ms

Total elapsed time: 1.013 s

The package vulkan-hpp:arm64-android is header only and can be used from CMake via:

    find_path(VULKAN_HPP_INCLUDE_DIRS "vulkan/vulkan.hpp")
    target_include_directories(main PRIVATE ${VULKAN_HPP_INCLUDE_DIRS})

```

</details>

## Consume libraries using vpckg, cmake and the android toolchain

1. Combine vcpkg and Android toolchains

vcpkg and android both provide dedicated toolchains:

```bash
vcpkg_toolchain_file=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
android_toolchain_file=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake
```

When using vcpkg, the vcpkg toolchain shall be specified first. 

However, vcpkg provides a way to preload and additional toolchain, with the VCPKG_CHAINLOAD_TOOLCHAIN_FILE option. 

```bash
cmake \
  -DCMAKE_TOOLCHAIN_FILE=$vcpkg_toolchain_file \
  -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=$android_toolchain_file \
  ...
```

1. Specify the android abi and vcpkg triplet

When compiling for android, you need to select a matching "android abi" / "vcpkg triplet" pair.

For example:

```bash
android_abi=armeabi-v7a
vcpkg_target_triplet=arm-android

cmake 
  ...
  -DVCPKG_TARGET_TRIPLET=$vcpkg_target_triplet \
  -DANDROID_ABI=$android_abi
```

### Test on an example

The folder [docs/examples/vcpkg_android_example_cmake](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake) provides a working example, with an android library that consumes the jsoncpp library:

*Details*

- The [CMakeLists](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake/CMakeLists.txt) simply uses `find_package` and `target_link_library`

- The [compile.sh](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake/compile.sh) script enables you to select any matching pair of "android abi" /  "vcpkg triplet" and to test the compilation

- The dummy [my_lib.cpp](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake/my_lib.cpp) file uses the jsoncpp library

> [!NOTE]
> This example only compiles an Android library, as the compilation of a full-fledged Android App is beyond the scope of this document.

### Test on an example, using [vcpkg_android.cmake](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake_script/cmake/vcpkg_android.cmake)

The folder [vcpkg_android_example_cmake_script](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake_script) provides the same example, and uses a cmake script in order to simplify the usage.

*Details*

- The main [CMakeLists](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake_script/CMakeLists.txt) loads [vcpkg_android.cmake](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake_script/cmake/vcpkg_android.cmake) if the flag `VCPKG_TARGET_ANDROID` is set:

  ```cmake
  if (VCPKG_TARGET_ANDROID)
      include("cmake/vcpkg_android.cmake")
  endif()
  ```

  > [!IMPORTANT]
  > Place these lines before calling `project()`.

- The [compile.sh](https://github.com/Microsoft/vcpkg-docs/tree/main/vcpkg/users/examples/vcpkg_android_example_cmake_script/compile.sh) script shows that it is then possible to compile for android using a simple cmake invocation, for example:

  ```bash
  cmake .. -DVCPKG_TARGET_ANDROID=ON -DANDROID_ABI=armeabi-v7a
  ```

## Consume libraries using vcpkg, and Android prefab Archives (AAR files)

[!INCLUDE [experimental](../../../includes/experimental.md)]

vcpkg can export android archives ([AAR files](https://developer.android.com/studio/projects/android-library)). Once an archive is created, it can imported in Android Studio as a native dependent.  The archive is automatically consumed using [android studio's prefab tool](https://github.com/google/prefab).

For more information on Prefab, refer to:

- The [official prefab documentation](https://google.github.io/prefab).
- a blog post from Android developers blog: [Native Dependencies in Android Studio 4.0](https://android-developers.googleblog.com/2020/02/native-dependencies-in-android-studio-40.html)

> [!NOTE]
> Android Studio users: prefab packages are supported on Android Studio 4+.

### Requirements

1. `ndk <required>`

Set environment variable `ANDROID_NDK_HOME` to your android ndk installation. For example:

```console
export ANDROID_NDK_HOME=/home/your-account/Android/Sdk/ndk-bundle
```

1. `7zip <required on windows>` or `zip <required on linux>`

1. `maven <optional>`

1. Android triplets

> [!NOTE] 
> To use "prefab" (see below), all four architectures are required. If any are missing the export will fail.

### Example exporting [jsoncpp]

First, "vcpkg install" the 4 android architectures (it is mandatory to export all 4 of them):

```console
./vcpkg install jsoncpp:arm-android  jsoncpp:arm64-android  jsoncpp:x64-android  jsoncpp:x86-android
```

Then, export the prefab:

Notes:

- The `--prefab-maven` flag is optional. Call it if maven is installed.
- The `--prefab-debug` flag will output instructions on how to use the prefab archive via gradle.

```console
./vcpkg export --triplet x64-android jsoncpp --prefab --prefab-maven --prefab-debug
```

You will see output like this:

```console
The following packages are already built and will be exported:
    jsoncpp:arm64-android

Exporting package jsoncpp...
[DEBUG] Found 4 triplets
  arm64-android
  x64-android
  x86-android
  arm-android

...
... Lots of output...
...

[INFO] Scanning for projects...
Downloading from central: https://repo.maven.apache.org/maven2/org/apache/maven/plugins/maven-clean-plugin/2.5/maven-clean-plugin-2.5.pom

...
... Lots of output...
...

[INFO] BUILD SUCCESS
[INFO] Total time:  2.207 s
[INFO] Finished at: 2020-05-10T14:42:28+02:00


...
... Lots of output...
...

[DEBUG] Configuration properties in Android Studio
In app/build.gradle

  com.vcpkg.ndk.support:jsoncpp:1.9.2

And cmake flags

    externalNativeBuild {
                cmake {
                    arguments '-DANDROID_STL=c++_shared'
                    cppFlags "-std=c++17"
                }
            }

In gradle.properties

    android.enablePrefab=true
    android.enableParallelJsonGen=false
    android.prefabVersion=${prefab.version}

Successfully exported jsoncpp. Checkout .../vcpkg/prefab 

```

#### The output directory after export

```no-highlight
prefab
└── jsoncpp/
    ├── aar/
    │   ├── AndroidManifest.xml
    │   ├── META-INF/
    │   │   └── LICENSE
    │   └── prefab/
    │       ├── modules/
    │       │   └── jsoncpp/
    │       │       ├── libs/
    │       │       │   ├── android.arm64-v8a/
    │       │       │   │   ├── abi.json
    │       │       │   │   ├── include/
    │       │       │   │   │   └── json/
    │       │       │   │   │       ├── json.h
    │       │       │   │   │       └── ....
    │       │       │   │   └── libjsoncpp.so
    │       │       │   ├── android.armeabi-v7a/
    │       │       │   │   ├── abi.json
    │       │       │   │   ├── include/
    │       │       │   │   │   └── json/
    │       │       │   │   │       ├── json.h
    │       │       │   │   │       └── ....
    │       │       │   │   └── libjsoncpp.so
    │       │       │   ├── android.x86/
    │       │       │   │   ├── abi.json
    │       │       │   │   ├── include/
    │       │       │   │   │   └── json/
    │       │       │   │   │       ├── json.h
    │       │       │   │   │       └── ....
    │       │       │   │   └── libjsoncpp.so
    │       │       │   └── android.x86_64/
    │       │       │       ├── abi.json
    │       │       │       ├── include/
    │       │       │       │   └── json/
    │       │       │       │       ├── json.h
    │       │       │       │       └── ....
    │       │       │       └── libjsoncpp.so
    │       │       └── module.json
    │       └── prefab.json
    ├── jsoncpp-1.9.2.aar
    └── pom.xml
````

### Example consuming [jsoncpp] via vcpkg and prefab

See the example repo here:

https://github.com/atkawa7/prefab-vpkg-integration-sample
