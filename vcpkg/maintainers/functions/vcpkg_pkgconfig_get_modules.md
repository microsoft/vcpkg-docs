---
title: x_vcpkg_pkgconfig_get_modules
description: Learn how to use x_vcpkg_pkgconfig_get_modules.
ms.date: 11/30/2022
---
# x_vcpkg_pkgconfig_get_modules

[!INCLUDE [experimental](../../../includes/experimental.md)]

Retrieve required module information from pkgconfig modules

## Usage

```cmake
x_vcpkg_pkgconfig_get_modules(
    PREFIX <prefix>
    MODULES <pkgconfig_modules>...
    [CFLAGS]
    [LIBS]
    [LIBRARIES]
    [LIBRARIES_DIRS]
    [INCLUDE_DIRS]
)
```

## Parameters

### PREFIX

Prefix to use for all output variables.

### MODULES

List of pkgconfig modules to retrieve information for.

### LIBS

Returns `"${PKGCONFIG}" --libs` as `<prefix>_LIBS_RELEASE` and `<prefix>_LIBS_DEBUG`.

### LIBRARIES

Returns `"${PKGCONFIG}" --libs-only-l` in `<prefix>_LIBRARIES_RELEASE` and `<prefix>_LIBRARIES_DEBUG`.

### LIBRARIES_DIRS

Returns `"${PKGCONFIG}" --libs-only-L` in `<prefix>_LIBRARIES_DIRS_RELEASE` and `<prefix>_LIBRARIES_DIRS_DEBUG`.

### INCLUDE_DIRS

Returns `"${PKGCONFIG}"  --cflags-only-I` in `<prefix>_INCLUDE_DIRS_RELEASE` and `<prefix>_INCLUDE_DIRS_DEBUG`.

## Examples

- [qt5-base](https://github.com/microsoft/vcpkg/blob/master/ports/qt5-base/portfile.cmake)

## Source

[ports/vcpkg-pkgconfig-get-modules/x\_vcpkg\_pkgconfig\_get\_modules.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-pkgconfig-get-modules/x_vcpkg_pkgconfig_get_modules.cmake)
