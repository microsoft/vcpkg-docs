---
title: Create and publish packages
description: Tutorial guides the user through the process of packaging a library for vcpkg.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 08/23/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to package libraries for vcpkg.
---

# Tutorial: Create and publish packages

This tutorial shows you how to publish a simle library for the vcpkg registry.

## Prerequisites

- A terminal
- A C++ compiler
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)

::: zone pivot="os-windows"

> [!NOTE]
> For Windows users, Visual Studio's MSVC (Microsoft Visual C++ Compiler) is the required compiler for C++ development.

::: zone-end

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

## 2 - Configure the `VCPKG_ROOT` environment variable

To set the `VCPKG_ROOT` environment variables, run the following commands:

::: zone pivot="os-linux,os-macos"

```bash
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

> [!NOTE]
> Setting the `VCPKG_ROOT` environment variable using the `export` command only affects the current shell session. To make this change permanent across sessions, you'll need to add the `export` command to your shell's profile script (e.g., `~/.bashrc` or `~/.zshrc`).

::: zone-end

::: zone pivot="os-windows"

```console
set VCPKG_ROOT="C:\path\to\vcpkg"
set PATH=%VCPKG_ROOT%;%PATH%
```

> [!NOTE]
> Setting the `VCPKG_ROOT` environment variable using the `set` command only affects the current shell session. To make this change permanent across sessions, you can use the `setx` command and restart the shell session.

::: zone-end

Setting `VCPKG_ROOT` tells vcpkg where your vcpkg instance is located.
Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

## 3 - Set up the port

To create the `portfile.cmake` and `vcpkg.json` files for the new port, run:

```console
vcpkg create vcpkg-sample-library https://github.com/JavierMatosD/vcpkg-sample-library/archive/refs/tags/1.0.3.tar.gz vcpkg-sample-library.tar.gz

A suitable version of cmake was not found (required v3.27.1) Downloading portable cmake 3.27.1...
Downloading cmake...
https://github.com/Kitware/CMake/releases/download/v3.27.1/cmake-3.27.1-windows-i386.zip->C:\dev\vcpkg\downloads\cmake-3.27.1-windows-i386.zip
Downloading https://github.com/Kitware/CMake/releases/download/v3.27.1/cmake-3.27.1-windows-i386.zip
Extracting cmake...
-- Downloading https://github.com/JavierMatosD/vcpkg-sample-library/archive/refs/tags/1.0.3.tar.gz -> vcpkg-sample-library.tar.gz...
-- Generated portfile: C:\dev\vcpkg\ports\vcpkg-sample-library\portfile.cmake
-- Generated manifest: C:\dev\vcpkg\ports\vcpkg-sample-library\vcpkg.json
-- To launch an editor for these new files, run
--     .\vcpkg edit vcpkg-sample-library
```

The `create` command performs the following actions:

1. Creates a port folder named `vcpkg-sample-library` containing a `portfile.cmake` file and a `vcpkg.json` file.
2. Downloads `https://github.com/JavierMatosD/vcpkg-sample-library/archive/refs/tags/1.0.3.tar.gz` as `vcpkg-sample-library.tar.gz` in the downloads directory.

## 4 - Modify the `portfile.cmake`

To open the `vcpkg-sample-library` port in a text editor, run:

```console
vcpkg edit vcpkg-sample-library
```

The generated `portfile.cmake` should look like the following:

```
# Common Ambient Variables:
#   CURRENT_BUILDTREES_DIR    = ${VCPKG_ROOT_DIR}\buildtrees\${PORT}
#   CURRENT_PACKAGES_DIR      = ${VCPKG_ROOT_DIR}\packages\${PORT}_${TARGET_TRIPLET}
#   CURRENT_PORT_DIR          = ${VCPKG_ROOT_DIR}\ports\${PORT}
#   CURRENT_INSTALLED_DIR     = ${VCPKG_ROOT_DIR}\installed\${TRIPLET}
#   DOWNLOADS                 = ${VCPKG_ROOT_DIR}\downloads
#   PORT                      = current port name (zlib, etc)
#   TARGET_TRIPLET            = current triplet (x86-windows, x64-windows-static, etc)
#   VCPKG_CRT_LINKAGE         = C runtime linkage type (static, dynamic)
#   VCPKG_LIBRARY_LINKAGE     = target library linkage type (static, dynamic)
#   VCPKG_ROOT_DIR            = <C:\path\to\current\vcpkg>
#   VCPKG_TARGET_ARCHITECTURE = target architecture (x64, x86, arm)
#   VCPKG_TOOLCHAIN           = ON OFF
#   TRIPLET_SYSTEM_ARCH       = arm x86 x64
#   BUILD_ARCH                = "Win32" "x64" "ARM"
#   DEBUG_CONFIG              = "Debug Static" "Debug Dll"
#   RELEASE_CONFIG            = "Release Static"" "Release DLL"
#   VCPKG_TARGET_IS_WINDOWS
#   VCPKG_TARGET_IS_UWP
#   VCPKG_TARGET_IS_LINUX
#   VCPKG_TARGET_IS_OSX
#   VCPKG_TARGET_IS_FREEBSD
#   VCPKG_TARGET_IS_ANDROID
#   VCPKG_TARGET_IS_MINGW
#   VCPKG_TARGET_EXECUTABLE_SUFFIX
#   VCPKG_TARGET_STATIC_LIBRARY_SUFFIX
#   VCPKG_TARGET_SHARED_LIBRARY_SUFFIX
#
# 	See additional helpful variables in /docs/maintainers/vcpkg_common_definitions.md

# Also consider vcpkg_from_* functions if you can; the generated code here is for any web accessable
# source archive.
#  vcpkg_from_github
#  vcpkg_from_gitlab
#  vcpkg_from_bitbucket
#  vcpkg_from_sourceforge
vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/JavierMatosD/vcpkg-sample-library/archive/refs/tags/1.0.3.tar.gz"
    FILENAME "vcpkg-sample-library.tar.gz"
    SHA512 56cffcf9bfb6e9c6bc41e28ae58e8ad25c112db49eafa63f20f95c1ced5712ba7bada9e0990741f70a3f45a4cfd6cb25a3500ef04d36037309b1b65e2f8b569d
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
    # (Optional) A friendly name to use instead of the filename of the archive (e.g.: a version number or tag).
    # REF 1.0.0
    # (Optional) Read the docs for how to generate patches at:
    # https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/examples/patching.md
    # PATCHES
    #   001_port_fixes.patch
    #   002_more_port_fixes.patch
)

# # Check if one or more features are a part of a package installation.
# # See /docs/maintainers/vcpkg_check_features.md for more details
# vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
#   FEATURES
#     tbb   WITH_TBB
#   INVERTED_FEATURES
#     tbb   ROCKSDB_IGNORE_PACKAGE_TBB
# )

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    # OPTIONS -DUSE_THIS_IN_ALL_BUILDS=1 -DUSE_THIS_TOO=2
    # OPTIONS_RELEASE -DOPTIMIZE=1
    # OPTIONS_DEBUG -DDEBUGGABLE=1
)

vcpkg_cmake_install()

# # Moves all .cmake files from /debug/share/vcpkg-sample-library/ to /share/vcpkg-sample-library/
# # See /docs/maintainers/ports/vcpkg-cmake-config/vcpkg_cmake_config_fixup.md for more details
# When you uncomment "vcpkg_cmake_config_fixup()", you need to add the following to "dependencies" vcpkg.json:
#{
#    "name": "vcpkg-cmake-config",
#    "host": true
#}
# vcpkg_cmake_config_fixup()

# Uncomment the line below if necessary to install the license file for the port
# as a file named `copyright` to the directory `${CURRENT_PACKAGES_DIR}/share/${PORT}`
# vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
```

Modify the `portfile.cmake` with the following content:

```
vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO JavierMatosD/vcpkg-sample-library
    REF "${VERSION}"
    SHA512 70e0b3693fa6afb958117167bb82dbcf12fb3f4cf7ff8ddb19bf46acc645daf6eea0ac861abb9fd18a25342509a4fa76a9bc60582e947b473d69ae9cc602b1da
    HEAD_REF main
)


vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

Line-by-line explanation:

For more information on port files, see the following:
- 

## 5 - Modify the `vcpkg.json`

The `vcpkg.json` file generated by the previous `create` command should look like the following:

```json
{
  "name": "vcpkg-sample-library",
  "version": "",
  "homepage": "",
  "description": "",
  "license": "",
  "dependencies": [
    {
      "name": "vcpkg-cmake",
      "host": true
    }
  ],

  "default-features": [],
  "features": {
    "example-feature": {
      "description": "",
      "dependencies": []
    }
  }
}

```

Modify the `vcpkg.json` file with the following content:

```json
{
  "name": "vcpkg-sample-library",
  "version": "1.0.0",
  "homepage": "https://github.com/JavierMatosD/vcpkg-sample-library",
  "description": "A sample C++ library designed to serve as a foundational example for a tutorial on packaging libraries with vcpkg.",
  "license": "MIT",
  "dependencies": [
    {
      "name" : "vcpkg-cmake",
      "host" : true
    },
    {
      "name" : "vcpkg-cmake-config",
      "host" : true
    },
    {
      "name": "fmt"
    }
  ],

  "default-features": [],
  "features": {
    "example-feature": {
      "description": "",
      "dependencies": []
    }
  }
}
```

TODO: provide line-by-line explanation

TODO: For more information on `vcpkg.json`, see the following:
- 

## 6 - Add usage instructions

Create a file in the `vcpkg/ports/vcpkg-sample-library` directory named `usage` with the following content:

```
vcpkg-sample-library provides CMake targets:

  find_package(my_sample_lib CONFIG REQUIRED)
  target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)

```

Providing usage documentation for ports allows users to easily adopt them in their projects. We highly encourage providing a `usage` file within the port's directory (`ports/<port name>/usage`) that describes the minimal steps necessary to integrate with a build system.

## 7 - Set up version database

TODO: instrunctions for setting up the version db, i.e., `x-add-version`

## 8 - verify the port builds

TODO: provide a method for verifying the library builds

## Next steps

To learn more about packaging libraries, see our reference documentation:

TODO: add appropriate next steps

- Ports concepts
- Updating a library in vcpkg
- maintainer guide
