---
title: vcpkg_install_qmake
description: Learn how to use vcpkg_install_qmake.
ms.date: 01/10/2024
---
# vcpkg_install_qmake

Build and install a qmake project.

## Usage

```cmake
vcpkg_install_qmake(...)
```

## Parameters

See [`vcpkg_build_qmake()`](vcpkg_build_qmake.md).

## Notes

This command transparently forwards to [`vcpkg_build_qmake()`](vcpkg_build_qmake.md).

Additionally, this command will copy produced .libs/.dlls/.as/.dylibs/.sos to the appropriate
staging directories.

## Source

[scripts/cmake/vcpkg\_install\_qmake.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_install_qmake.cmake)
