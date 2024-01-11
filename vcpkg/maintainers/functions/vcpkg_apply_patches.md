---
title: vcpkg_apply_patches
description: Use vcpkg_apply_patches to apply a set of patches to a source tree.
ms.date: 01/10/2024
---
# vcpkg_apply_patches

**This function has been deprecated in favor of the `PATCHES` argument to [`vcpkg_from_github()`](vcpkg_from_github.md#patches) and similar functions.**

Apply a set of patches to a source tree.

## Usage

```cmake
vcpkg_apply_patches(
    SOURCE_PATH <${SOURCE_PATH}>
    [QUIET]
    PATCHES <patch1.patch>...
)
```

## Source

[scripts/cmake/vcpkg\_apply\_patches.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_apply_patches.cmake)
