---
title: vcpkg_acquire_msys
description: Use vcpkg_acquire_msys to download and prepare an MSYS2 instance.
ms.date: 01/10/2024
---
# vcpkg_acquire_msys

Download and prepare an MSYS2 instance.

## Usage

```cmake
vcpkg_acquire_msys(<MSYS_ROOT_VAR>
    PACKAGES <package>...
    [NO_DEFAULT_PACKAGES]
    [DIRECT_PACKAGES <URL> <SHA512> <URL> <SHA512> ...]
)
```

## Parameters

### MSYS_ROOT_VAR

An out-variable that will be set to the path to MSYS2.

### PACKAGES

A list of packages to acquire in msys.

To ensure a package is available: `vcpkg_acquire_msys(MSYS_ROOT PACKAGES make automake1.16)`

### NO_DEFAULT_PACKAGES

Exclude the normal base packages.

The list of base packages includes: bash, coreutils, sed, grep, gawk, gzip, diffutils, make, and pkg-config

### DIRECT_PACKAGES

A list of URL/SHA512 pairs to acquire in msys.

This parameter can be used by a port to privately extend the list of msys packages to be acquired.
The URLs can be found on the [msys2 website](https://packages.msys2.org/search) and should be a direct archive link:

> `https://mirror.msys2.org/msys/x86_64/gettext-0.22.4-1-x86_64.pkg.tar.zst`

## Notes

A call to `vcpkg_acquire_msys` will usually be followed by a call to `bash.exe`:

```cmake
vcpkg_acquire_msys(MSYS_ROOT)
set(BASH "${MSYS_ROOT}/usr/bin/bash.exe")

vcpkg_execute_required_process(
    COMMAND "${BASH}" --noprofile --norc "${CMAKE_CURRENT_LIST_DIR}\\build.sh"
    WORKING_DIRECTORY "${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-rel"
    LOGNAME "build-${TARGET_TRIPLET}-rel"
)
```

## Examples

- [ffmpeg](https://github.com/Microsoft/vcpkg/blob/master/ports/ffmpeg/portfile.cmake)
- [libvpx](https://github.com/Microsoft/vcpkg/blob/master/ports/libvpx/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_acquire\_msys.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_acquire_msys.cmake)
