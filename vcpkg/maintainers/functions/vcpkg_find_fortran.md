---
title: vcpkg_find_fortran
description: Learn how to use vcpkg_find_fortran.
ms.date: 01/10/2024
---
# vcpkg_find_fortran

Checks if a Fortran compiler can be found.
Windows(x86/x64) Only: If not it will switch/enable MinGW gfortran
                       and return required cmake args for building.

## Usage

```cmake
vcpkg_find_fortran(<out_var>)
```

## Example

```cmake
vcpkg_find_fortran(fortran_args)
# ...
vcpkg_cmake_configure(...
    OPTIONS
        ${fortran_args}
)
```

## Source

[scripts/cmake/vcpkg\_find\_fortran.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_find_fortran.cmake)
