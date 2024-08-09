---
title: vcpkg_download_distfile
description: Learn how to use vcpkg_download_distfile.
author: vicroms
ms.author: viromer
ms.topic: reference
ms.date: 01/10/2024
---
# vcpkg_download_distfile

Download and cache a file needed for this port.

This helper should always be used instead of CMake's built-in `file(DOWNLOAD)` command, as it
enables features like [Asset Caching](../../users/assetcaching.md).

If possible, one of the `vcpkg_from_` functions should be used rather than calling this function
directly, such as one of the following:

* [`vcpkg_from_bitbucket`](vcpkg_from_bitbucket.md)
* [`vcpkg_from_git`](vcpkg_from_git.md)
* [`vcpkg_from_github`](vcpkg_from_github.md)
* [`vcpkg_from_gitlab`](vcpkg_from_gitlab.md)
* [`vcpkg_from_sourceforge`](vcpkg_from_sourceforge.md)

## Usage

```cmake
vcpkg_download_distfile(
    <OUT_VARIABLE>
    URLS <http://mainUrl> <http://mirror1>...
    FILENAME <output.zip>
    SHA512 <5981de...>
    [ALWAYS_REDOWNLOAD]
)
```

## Parameters

### OUT_VARIABLE

This variable will be set to the full path to the downloaded file. This can then immediately be passed to
[`vcpkg_extract_source_archive`](vcpkg_extract_source_archive.md) for sources.

### URLS

A list of URLs to be consulted. They will be tried in order until one of the downloaded files successfully matches the
SHA512 given.

### FILENAME

The local name for the file. Files are shared between ports, so the file may need to be renamed to make it clearly
attributed to this port and avoid conflicts.

### SHA512

The expected hash for the file.

If this doesn't match the downloaded version, the build will be terminated with a message describing the mismatch.

### QUIET

Suppress output on cache hit

### SKIP_SHA512

Skip SHA512 hash check for file.

This switch is only valid when building with the `--head` command line flag.

### ALWAYS_REDOWNLOAD

Avoid caching; this is a REST call or otherwise unstable.

Requires `SKIP_SHA512`.

### HEADERS

A list of headers to append to the download request. This can be used for authentication during a download.

Headers should be specified as `"<header-name>: <header-value>"`.

## Notes

The helper [`vcpkg_from_github`](vcpkg_from_github.md) should be used for downloading from GitHub projects.

## Examples

- [apr](https://github.com/Microsoft/vcpkg/blob/master/ports/apr/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_download\_distfile.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_download_distfile.cmake)
