---
title: vcpkg_install_meson
description: Learn how to use vcpkg_install_meson.
ms.date: 11/30/2022
---
# vcpkg_install_meson

Builds a meson project previously configured with `vcpkg_configure_meson()`.

## Usage

```cmake
vcpkg_install_meson([ADD_BIN_TO_PATH])
```

## Parameters

### ADD_BIN_TO_PATH
Adds the appropriate Release and Debug `bin\` directories to the path during the build such that executables can run against the in-tree DLLs.

## Examples

- [fribidi](https://github.com/Microsoft/vcpkg/blob/master/ports/fribidi/portfile.cmake)
- [libepoxy](https://github.com/Microsoft/vcpkg/blob/master/ports/libepoxy/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_install\_meson.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_install_meson.cmake)

