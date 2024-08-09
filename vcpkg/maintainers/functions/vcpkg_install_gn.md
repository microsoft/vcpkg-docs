---
title: vcpkg_install_gn
description: Learn how to use vcpkg_install_gn.
ms.date: 01/10/2024
---
# vcpkg_install_gn

> [!WARNING]
> This function has been deprecated in favor of [`vcpkg_gn_install`](vcpkg_gn_install.md).

Installs a GN project.

To build a GN project without installing, use [`vcpkg_build_ninja()`](vcpkg_build_ninja.md).

## Usage

```cmake
vcpkg_install_gn(
     SOURCE_PATH <SOURCE_PATH>
     [TARGETS <target>...]
)
```

## Parameters

### SOURCE_PATH

The path to the source directory

### TARGETS

Only install the specified targets.

Include files must be handled separately.

## Source

[`scripts/cmake/vcpkg_install_gn.cmake`](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_install_gn.cmake)
