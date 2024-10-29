---
title: vcpkg_install_cmake
description: Learn how to use vcpkg_install_cmake.
ms.date: 01/10/2024
---
# vcpkg_install_cmake

> [!WARNING]
> This function has been deprecated in favor of [`vcpkg_cmake_install`](vcpkg_cmake_install.md) from the vcpkg-cmake port.

Build and install a cmake project.

## Usage

```cmake
vcpkg_install_cmake(...)
```

## Parameters

See [`vcpkg_build_cmake()`](vcpkg_build_cmake.md).

## Notes

This command transparently forwards to [`vcpkg_build_cmake()`](vcpkg_build_cmake.md), adding a `TARGET install`
parameter.

## Source

[scripts/cmake/vcpkg\_install\_cmake.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_install_cmake.cmake)
