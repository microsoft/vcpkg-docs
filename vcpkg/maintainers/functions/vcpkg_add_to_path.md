---
title: vcpkg_add_to_path
description: Use vcpkg_add_to_path to add directories to the PATH.
ms.date: 01/10/2024
---
# vcpkg_add_to_path

Add a directory or directories to the PATH environment variable.

## Usage

```cmake
vcpkg_add_to_path([PREPEND] [<path>...])
```

## Parameters

### PREPEND

Prepend items to the PATH environment variable instead of appending.

### `<path>...`

Paths to append to the PATH environment variable.

## Examples

- [folly](https://github.com/Microsoft/vcpkg/blob/master/ports/folly/portfile.cmake)
- [z3](https://github.com/Microsoft/vcpkg/blob/master/ports/z3/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_add\_to\_path.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_add_to_path.cmake)
