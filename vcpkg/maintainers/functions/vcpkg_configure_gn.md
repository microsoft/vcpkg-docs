---
title: vcpkg_configure_gn
description: Use vcpkg_configure_gn to generate ninja targets.
ms.date: 01/10/2024
---
# vcpkg_configure_gn

> [!WARNING]
> This function has been deprecated in favor of [`vcpkg_gn_configure`](vcpkg_gn_configure.md).

Generate Ninja (GN) targets

## Usage

```cmake
vcpkg_configure_gn(
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

Options to be passed to both the debug and release targets. The options must be provided as a space-separated string.

### OPTIONS_DEBUG (space-separated string)

Options to be passed to the debug target.

### OPTIONS_RELEASE (space-separated string)

Options to be passed to the release target.

## Source

[scripts/cmake/vcpkg\_configure\_gn.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_configure_gn.cmake)
