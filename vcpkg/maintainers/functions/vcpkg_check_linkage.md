---
title: vcpkg_check_linkage
description: Assert the available library and CRT linkage options for the port.
ms.date: 12/05/2024
ms.topic: reference
---
# vcpkg_check_linkage

Asserts the available library and CRT linkage options for the port.

## Usage

```cmake
vcpkg_check_linkage(
    [ONLY_STATIC_LIBRARY | ONLY_DYNAMIC_LIBRARY]
    [ONLY_STATIC_CRT | ONLY_DYNAMIC_CRT]
)
```

## Parameters

### ONLY_STATIC_LIBRARY

Indicates that this port can only be built with static library linkage.

If the user requested a dynamic build, `ONLY_STATIC_LIBRARY` will result in a note being printed,
not a fatal error.

### ONLY_DYNAMIC_LIBRARY

Indicates that this port can only be built with dynamic/shared library linkage. When using this
option, port authors should add `!(static & static-crt)` to the [`"supports"`](../../reference/vcpkg-json.md#supports) expression in the
port's `vcpkg.json` to warn consumers it will fail early.

### ONLY_STATIC_CRT

Indicates that this port can only be built with static CRT linkage.

### ONLY_DYNAMIC_CRT

Indicates that this port can only be built with dynamic/shared CRT linkage.

## Notes

If the triplet requests a setting of `VCPKG_LIBRARY_LINKAGE` different than the port supports,
and changing `VCPKG_LIBRARY_LINKAGE` to that the port supports is considered safe,
`vcpkg_check_linkage` changes `VCPKG_LIBRARY_LINKAGE` to that the port supports and emits a warning.
This means that the port may produce something that the user does not expect, but the alternative
would be to just fail.

If `vcpkg_check_linkage` would change `VCPKG_LIBRARY_LINKAGE` to `dynamic` while the triplet
requests `VCPKG_CRT_LINKAGE` of `static`, the change is not considered safe and
`vcpkg_check_linkage` fails. Building a dynamic library with a static CRT creates conditions many
developers find surprising, and for which most ports are unprepared.

For example, on Windows, each DLL will get its own copy of the CRT, meaning such DLLs cannot share
standard library components over the DLL boundary. On non-Windows, different .sos or .dylibs may
cause mutually incompatible symbols from different CRT versions to be concurrently loaded.

If you are consuming a port and it fails in `vcpkg_check_linkage`, consider choosing a triplet that
sets `VCPKG_CRT_LINKAGE` to `dynamic`. If you really know what you're doing, understand the
potential problems a static CRT with a dynamic library can cause, and are confident that the port
safely handles that configuration, author a custom triplet which explicitly sets
`VCPKG_LIBRARY_LINKAGE` to `dynamic` and `VCPKG_CRT_LINKAGE` to static. For example:

```cmake
if("${PORT}" STREQUAL "the-name-of-the-port-you-want-to-control")
    set(VCPKG_LIBRARY_LINKAGE dynamic)
    set(VCPKG_CRT_LINKAGE static)
endif()
```

## Examples

- [abseil](https://github.com/Microsoft/vcpkg/blob/master/ports/abseil/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_check\_linkage.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_check_linkage.cmake)
