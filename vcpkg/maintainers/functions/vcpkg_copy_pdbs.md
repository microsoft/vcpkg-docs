---
title: vcpkg_copy_pdbs
description: Learn how to use vcpkg_copy_pdbs.
ms.date: 01/10/2024
---
# vcpkg_copy_pdbs

Automatically locate pdbs in the build tree and copy them adjacent to all DLLs.

## Usage

```cmake
vcpkg_copy_pdbs(
    [BUILD_PATHS <glob>...])
```

## Parameters

### BUILD_PATHS

Patterns which will be passed to `file(GLOB_RECURSE)`, for locating DLLs.

Defaults to:

- `${CURRENT_PACKAGES_DIR}/bin/*.dll`
- `${CURRENT_PACKAGES_DIR}/debug/bin/*.dll`

## Notes

This command should be called by all portfiles after they have finished rearranging the binary output.

## Examples

- [zlib](https://github.com/Microsoft/vcpkg/blob/master/ports/zlib/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_copy\_pdbs.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_copy_pdbs.cmake)
