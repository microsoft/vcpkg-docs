---
title: vcpkg_replace_string
description: Learn how to use vcpkg_replace_string.
ms.date: 01/10/2024
---
# vcpkg_replace_string

Replace a string in a file.

## Usage

```cmake
vcpkg_replace_string(<filename> <match> <replace> [REGEX] [IGNORE_UNCHANGED])
```

## Parameters

### `<filename>`

The path to the file to modify.

### `<match>`

The string to match.

### `<replace>`

The string to replace all matched sequences with.

### `[REGEX]`

If this argument is present, the `<match>` argument is treated as a regular expression.

### `[IGNORE_UNCHANGED]`

Disable the warning if the file is not changed.

## Source

[scripts/cmake/vcpkg\_replace\_string.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_replace_string.cmake)
