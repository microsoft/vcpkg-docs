---
title: vcpkg_gn_install
description: Learn how to use vcpkg_gn_install.
ms.date: 01/10/2024
---
# vcpkg_gn_install

Installs a GN project.

In order to build a GN project without installing, use [`vcpkg_build_ninja()`](vcpkg_build_ninja.md).

## Usage

```cmake
vcpkg_gn_install(
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

[ports/vcpkg-gn/vcpkg\_gn\_install.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-gn/vcpkg_gn_install.cmake)
