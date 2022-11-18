---
title: vcpkg_buildpath_length_warning
---

# vcpkg_buildpath_length_warning

Warns the user if their vcpkg installation path might be too long for the package they're installing.

## Usage
```cmake
vcpkg_buildpath_length_warning(<N>)
```

## Parameters
### `<N>`
Warns the user if the number of bytes in the path to `buildtrees` is bigger than `N`.

## Notes
This function only emits a warning and must not be used for correctness.

## Source
[scripts/cmake/vcpkg\_buildpath\_length\_warning.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_buildpath_length_warning.cmake)

