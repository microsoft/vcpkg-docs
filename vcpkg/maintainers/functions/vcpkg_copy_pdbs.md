---
title: vcpkg_copy_pdbs
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
