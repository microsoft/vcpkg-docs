---
title: vcpkg_gn_configure
description: Learn how to use vcpkg_gn_configure.
ms.date: 01/10/2024
---
# vcpkg_gn_configure

Generate Ninja (GN) targets

## Usage

```cmake
vcpkg_gn_configure(
    SOURCE_PATH <SOURCE_PATH>
    [OPTIONS <OPTIONS>]
    [OPTIONS_DEBUG <OPTIONS_DEBUG>]
    [OPTIONS_RELEASE <OPTIONS_RELEASE>]
)
```

## Parameters

### SOURCE_PATH (required)
The path to the GN project.

### OPTIONS

Options to be passed to both the debug and release targets.
The options must be provided as a space-separated string.

### OPTIONS_DEBUG (space-separated string)

Options to be passed to the debug target.

### OPTIONS_RELEASE (space-separated string)

Options to be passed to the release target.

## Source

[ports/vcpkg-gn/vcpkg\_gn\_configure.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-gn/vcpkg_gn_configure.cmake)
