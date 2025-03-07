---
title: vcpkg_find_acquire_program
description: Learn how to use vcpkg_find_acquire_program.
ms.date: 01/22/2025
---
# vcpkg_find_acquire_program

Download or find a well-known tool.

## Usage

```cmake
vcpkg_find_acquire_program(<program>)
```

## Parameters

### program

This variable specifies both the program to be acquired as well as the out parameter that will be set to the path of the program executable.

## Notes

The current list of programs includes:

- 7Z
- BISON
- CLANG
- DARK
- DOXYGEN
- FLEX
- GASPREPROCESSOR
- GPERF
- PERL
- PYTHON2
- PYTHON3
- GIT
- GN
- GO
- JOM
- MESON
- NASM
- NINJA
- NUGET
- SCONS
- SWIG
- YASM

msys2 has a dedicated helper function: [`vcpkg_acquire_msys`](vcpkg_acquire_msys.md).

## Examples

- [ffmpeg](https://github.com/Microsoft/vcpkg/blob/master/ports/ffmpeg/portfile.cmake)

## Source

[`scripts/cmake/vcpkg_find_acquire_program.cmake`](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_find_acquire_program.cmake)
