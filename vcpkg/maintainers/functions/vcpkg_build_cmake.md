---
title: vcpkg_build_cmake
description: Use vcpkg_build_cmake to build a cmake project.
ms.date: 01/10/2024
---
# vcpkg_build_cmake

> [!WARNING]
> This function has been deprecated in favor of [`vcpkg_cmake_build`](vcpkg_cmake_build.md).

Build a cmake project.

## Usage

```cmake
vcpkg_build_cmake([DISABLE_PARALLEL] [TARGET <target>])
```

## Parameters

### DISABLE_PARALLEL

The underlying buildsystem will be instructed to not parallelize

### TARGET

The target passed to the cmake build command (`cmake --build . --target <target>`). If not specified, no target will
be passed.

### ADD_BIN_TO_PATH

Adds the appropriate Release and Debug `bin` directories to the path during the build such that executables can run against the in-tree DLLs.

## Notes

This command should be preceded by a call to [`vcpkg_configure_cmake()`](vcpkg_configure_cmake.md).
You can use the alias [`vcpkg_install_cmake()`](vcpkg_configure_cmake.md) function if your CMake script supports the
"install" target

## Source

[scripts/cmake/vcpkg\_build\_cmake.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_build_cmake.cmake)
