---
title: vcpkg_extract_source_archive_ex
description: Learn how to use vcpkg_extract_source_archive_ex.
ms.date: 01/10/2024
---
# vcpkg_extract_source_archive_ex

> [!WARNING]
> This function has been deprecated in favor of [`vcpkg_extract_source_archive()`](vcpkg_extract_source_archive.md).

Extract an archive.

## Usage

```cmake
vcpkg_extract_source_archive_ex(
    [OUT_SOURCE_PATH <out-var>]
    [<options>...]
)
```

This command forwards all options to [`vcpkg_extract_source_archive()`], with `<out-var>` as the first argument. Equivalent to `vcpkg_extract_source_archive(<out-var> <options>...)`. For parameter help, see the documentation for [`vcpkg_extract_source_archive()`](vcpkg_extract_source_archive.md).

## Source

[`scripts/cmake/vcpkg_extract_source_archive_ex.cmake`](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_extract_source_archive_ex.cmake)
