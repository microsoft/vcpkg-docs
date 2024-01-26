---
title: vcpkg_qmake_install
description: Use vcpkg_qmake_install to build a QMake project.
ms.date: 01/10/2024
---

# vcpkg_qmake_install

_Added in `vcpkg-qmake` version `2022-11-16`_.

Build and install a qmake project previously configured with [`vcpkg_qmake_configure()`](vcpkg_qmake_configure.md).

```cmake
vcpkg_qmake_install(...)
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

### Parameters:
[See `vcpkg_qmake_install()`](vcpkg_qmake_install.md).

### Notes:
This command transparently forwards to [`vcpkg_qmake_install()`](vcpkg_qmake_install.md) and appends the 'install' target

## Source
[ports/vcpkg-qmake/vcpkg\_qmake\_install.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-qmake/vcpkg_qmake_install.cmake)
