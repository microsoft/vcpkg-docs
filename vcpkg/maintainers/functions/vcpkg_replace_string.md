---
title: vcpkg_replace_string
description: Learn how to use vcpkg_replace_string.
ms.date: 11/30/2022
---
# vcpkg_replace_string

Replace a string in a file.

## Usage

```cmake
vcpkg_replace_string(<filename> <match> <replace>)
```

## Parameters

### `<filename>`
The path to the file to modify.

### `<match>`

The string to match.

### `<replace>`

The string to replace all matched sequences with.

## Source

[scripts/cmake/vcpkg\_replace\_string.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_replace_string.cmake)

