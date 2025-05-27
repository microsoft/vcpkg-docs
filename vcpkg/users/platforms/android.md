---
title: Android support
description: How to target Android devices using C++ with vcpkg
ms.date: 07/11/2024
ms.topic: concept-article
---
# Android

The triplets x64-android, arm-neon-android, and arm64-android are tested by vcpkg's curated registry continuous integration.

## Android build requirements

1. Download the [Android NDK](https://developer.android.com/ndk/downloads/)

2. Set environment variable `ANDROID_NDK_HOME` to your Android NDK installation to set up the Android toolchain. For example:

   ```bash
   export ANDROID_NDK_HOME=/home/your-account/Android/Sdk/ndk-bundle
   ```

   Or:

   ```bash
   export ANDROID_NDK_HOME=/home/your-account/Android/android-ndk-r26d
   ```

Note: you will still need to install g++ or a C++ compiler that targets your host for any host dependencies.

## vcpkg triplets and their corresponding Android ABI

There are six different Android ABIs, each of which maps to a vcpkg triplet. The following table outlines the mapping from vcpkg architectures to android architectures:

|VCPKG_TARGET_TRIPLET       | ANDROID_ABI          | ANDROID_ARM_NEON |
|---------------------------|----------------------|------------------|
|arm64-android              | arm64-v8a            |                  |
|arm-android                | armeabi-v7a          | OFF              |
|arm-neon-android           | armeabi-v7a          | ON               |
|x64-android                | x86_64               |                  |
|x86-android                | x86                  |                  |
|armv6-android              | armeabi              |                  |

## Building Android libraries in a Docker container

You can build Android libraries, such as `jsoncpp` in a Ubuntu Docker container.

Create a `Dockerfile` with the following contents:

```Dockerfile
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive

RUN \
  apt-get update && \
  apt-get -y upgrade

RUN \
  apt-get -y --no-install-recommends install git g++ wget curl zip vim pkg-config tar cmake unzip ca-certificates

# Download Android NDK
RUN \
  wget https://dl.google.com/android/repository/android-ndk-r26d-linux.zip && \
  unzip android-ndk-r26d-linux.zip && \
  rm -rf android-ndk-r26d-linux.zip

ENV ANDROID_NDK_HOME /android-ndk-r26d

RUN git clone https://github.com/microsoft/vcpkg
WORKDIR vcpkg
RUN ./bootstrap-vcpkg.sh

ENV PATH "/vcpkg:$PATH"
ENV VCPKG_ROOT "/vcpkg"

WORKDIR /project
```

Build the image and launch a new container:

```sh
docker build . -t "vcpkg-android"
docker run -it "vcpkg-android" bash
```

In the container, create `/project/vcpkg.json` with the following contents:

```json
{
  "dependencies": [
    "jsoncpp"
  ],
  "builtin-baseline": "1e68748a7c6914642ed686b2e19c3d688bca150a"
}
```

Finally, run `vcpkg install --triplet x64-android` to build `jsoncpp` for android.

## Using Vulkan SDK

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

Notice that **the location of the sysroot has changed since NDK 22**. (For more information, see [\[BUG\] sysroot headers missing since r22?](https://github.com/android/ndk/issues/1407)).

If you prefer using [the latest version](https://developer.android.com/studio/projects/install-ndk#default-ndk-per-agp), check the [BuildSystemMaintainers.md of the NDK document](https://android.googlesource.com/platform/ndk/+/master/docs/BuildSystemMaintainers.md#sysroot) and then put appropriate path for your system.

For example, MacOS users will use the path like this example:

```cmake
# In android triplets... (e.g. arm64-android.cmake)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
# ...
# If your API level is 30, libvulkan.so is at $ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/lib/aarch64-linux-android/30
set(ENV{VULKAN_SDK} $ENV{ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr)
```

By doing this for all Android triplets, you can install `vulkan` and the packages that require it. (e.g. `vulkan-hpp`)

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

## Example Android Project

The folder [docs/examples/vcpkg_android_example_cmake](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake) provides a working example, with an Android library that consumes the jsoncpp library:

### Details

- The [CMakeLists.txt](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake/CMakeLists.txt) simply uses `find_package` and `target_link_library`

- The [compile.sh](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake/compile.sh) script enables you to select any matching pair of "android abi" /  "vcpkg triplet" and to test the compilation

- The dummy [my_lib.cpp](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake/my_lib.cpp) file uses the jsoncpp library

> [!NOTE]
> This example only compiles an Android library, as the compilation of a full-fledged Android App is beyond the scope of this document.

## Test on an example: `vcpkg_android.cmake`

Test using [vcpkg_android.cmake](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake_script/cmake/vcpkg_android.cmake).

The folder [vcpkg_android_example_cmake_script](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake_script) provides the same example, and uses a cmake script in order to simplify the usage.

### Details
- The main [CMakeLists.txt](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake_script/CMakeLists.txt) loads [vcpkg_android.cmake](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake_script/cmake/vcpkg_android.cmake) if the flag `VCPKG_TARGET_ANDROID` is set:

  ```cmake
  if (VCPKG_TARGET_ANDROID)
      include("cmake/vcpkg_android.cmake")
  endif()
  ```

  > [!IMPORTANT]
  > Place these lines before calling `project()`.

- The [compile.sh](https://github.com/MicrosoftDocs/vcpkg-docs/tree/main/vcpkg/examples/vcpkg_android_example_cmake_script/compile.sh) script shows that it is then possible to compile for android using a simple cmake invocation, for example:

  ```bash
  cmake .. -DVCPKG_TARGET_ANDROID=ON -DANDROID_ABI=armeabi-v7a
  ```
