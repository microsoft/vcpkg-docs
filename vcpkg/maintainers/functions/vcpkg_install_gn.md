---
title: vcpkg_install_gn
---

# vcpkg_install_gn

**This function has been deprecated in favor of [`vcpkg_gn_install`](vcpkg_gn_install.md).**

Installs a GN project.

In order to build a GN project without installing, use [`vcpkg_build_ninja()`].

## Usage:
```cmake
vcpkg_install_gn(
     SOURCE_PATH <SOURCE_PATH>
     [TARGETS <target>...]
)
```

## Parameters:
### SOURCE_PATH
The path to the source directory

### TARGETS
Only install the specified targets.

Note: includes must be handled separately

[`vcpkg_build_ninja()`]: vcpkg_build_ninja.md

## Source
[scripts/cmake/vcpkg\_install\_gn.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_install_gn.cmake)
