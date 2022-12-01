---
title: vcpkg_add_to_path
description: Use vcpkg_add_to_path to add directories to the PATH.
ms.date: 11/30/2022
---
# vcpkg_add_to_path

Add a directory or directories to the PATH environment variable.

## Usage

```cmake
vcpkg_add_to_path([PREPEND] [<path>...])
```

## Parameters

### PREPEND

Prepend items to path instead of appending.

### `<path>...`

Paths to append to the PATH environment variable.

## Examples

- [curl](https://github.com/Microsoft/vcpkg/blob/master/ports/curl/portfile.cmake#L75)
- [folly](https://github.com/Microsoft/vcpkg/blob/master/ports/folly/portfile.cmake#L15)
- [z3](https://github.com/Microsoft/vcpkg/blob/master/ports/z3/portfile.cmake#L13)

## Source

[scripts/cmake/vcpkg\_add\_to\_path.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_add_to_path.cmake)
