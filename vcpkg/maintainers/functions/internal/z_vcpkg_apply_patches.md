---
title: z_vcpkg_apply_patches
description: Internal helper to apply a set of patches to a source tree.
ms.date: 11/30/2022
---
# z_vcpkg_apply_patches

[!INCLUDE [internal-helper](../../../../includes/internal-helper.md)]

Apply a set of patches to a source tree.

## Usage

```cmake
z_vcpkg_apply_patches(
    SOURCE_PATH <path-to-source>
    [QUIET]
    PATCHES <patch>...
)
```

## Parameters

### SOURCE_PATH

The path to apply patches into.

### PATCHES

The list of patches to apply.

Relative paths are interpreted from the root of the port directory.

### QUIET

Do not print any messages and do not fatal error on failure to apply patches.

## Notes

A common pattern for large patches is to download them with `vcpkg_download_distfile()` and pass the downloaded path into `PATCHES`.

## Source

[scripts/cmake/z\_vcpkg\_apply\_patches.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/z_vcpkg_apply_patches.cmake)
