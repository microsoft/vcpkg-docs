	---
title: vcpkg_download_sourceforge
description: Learn how to use vcpkg_download_sourceforge.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: reference
---
# vcpkg_download_sourceforge

Download a file from a project in SourceForge.

This function automatically checks a set of SourceForge mirrors.
Additional mirrors can be injected through the `VCPKG_SOURCEFORGE_EXTRA_MIRRORS`
list variable in the triplet.

To use the file as source code, use [`vcpkg_from_sourceforge`](vcpkg_from_sourceforge.md) rather than this function plus
`vcpkg_extract_source_archive`.

## Usage

```cmake
vcpkg_download_sourceforge(
    <OUT_VARIABLE>
    REPO <cunit/CUnit>
    [REF <2.1-3>]
    SHA512 <547b417109332...>
    FILENAME <CUnit-2.1-3.tar.bz2>
)
```

## Parameters

### OUT_VARIABLE

This variable will be set to the full path to the downloaded file.

### REPO

The organization or user and repository (optional) on SourceForge.

### REF

A stable version number that will not change contents.

### FILENAME

The local name for the file. Files are shared between ports, so the file may need to be renamed to make it clearly
attributed to this port and avoid conflicts.

For example, we can get the download link:
https://sourceforge.net/settings/mirror_choices?projectname=mad&filename=libmad/0.15.1b/libmad-0.15.1b.tar.gz&selected=nchc
So the REPO is `mad/libmad`, the REF is `0.15.1b`, and the FILENAME is `libmad-0.15.1b.tar.gz`

For some special links:
https://sourceforge.net/settings/mirror_choices?projectname=soxr&filename=soxr-0.1.3-Source.tar.xz&selected=nchc
The REPO is `soxr`, REF is empty, and the FILENAME is `soxr-0.1.3-Source.tar.xz`

### SHA512

The SHA512 hash that should match the archive.

This is most easily determined by first setting it to `0`, then trying to build the port. The error message will
contain the full hash, which can be copied back into the portfile.

## Source

[scripts/cmake/vcpkg\_download\_sourceforge.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_download_sourceforge.cmake)
