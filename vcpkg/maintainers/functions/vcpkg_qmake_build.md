---
title: vcpkg_qmake_build
description: Use vcpkg_qmake_build to build a QMake project.
ms.date: 01/10/2024
---

# vcpkg_qmake_build

_Added in `vcpkg-qmake` version `2022-11-16`_.

Build a qmake-based project, previously configured using [vcpkg_qmake_configure](vcpkg_qmake_configure.md).

```cmake
vcpkg_qmake_build(
    [SKIP_MAKEFILES]
    [BUILD_LOGNAME arg1]
    [TARGETS arg1 [arg2 ...]]
    [RELEASE_TARGETS arg1 [arg2 ...]]
    [DEBUG_TARGETS arg1 [arg2 ...]]
)
```

To use this function, you must depend on the helper port `vcpkg-qmake`:

```json
"dependencies": [
  {
    "name": "vcpkg-qmake",
    "host": true
  }
]
```

### SKIP_MAKEFILES

Skip Makefile fixup to redirect _installed_ references to _packages_.

### BUILD_LOGNAME

Configuration independent prefix for the build log files.

Defaults to `build`.

### <a name="targets">TARGETS, RELEASE\_TARGETS, DEBUG\_TARGETS</a>
Targets to build for a certain configuration.

## Examples

[Search microsoft/vcpkg for Examples](https://github.com/microsoft/vcpkg/search?q=vcpkg_cmake_build+path%3A%2Fports)

## Source

[ports/vcpkg-qmake/vcpkg\_qmake\_build.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-qmake/vcpkg_qmake_build.cmake)