---
title: vcpkg_get_program_files_platform_bitness
description: Learn how to use vcpkg_get_program_files_platform_bitness.
ms.date: 11/30/2022
---
# vcpkg_get_program_files_platform_bitness

Get the Program Files directory of the current platform's bitness:
either `$ENV{ProgramW6432}` on 64-bit windows,
or `$ENV{PROGRAMFILES}` on 32-bit windows.

## Usage

```cmake
vcpkg_get_program_files_platform_bitness(<variable>)
```

## Source

[scripts/cmake/vcpkg\_get\_program\_files\_platform\_bitness.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_get_program_files_platform_bitness.cmake)

