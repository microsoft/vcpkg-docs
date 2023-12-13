---
title: vcpkg_extract_archive
description: Learn how to use vcpkg_extract_archive.
author: vicroms
ms.author: viromer
ms.date: 11/10/2023
ms.prod: vcpkg
ms.topic: reference
---
# vcpkg_extract_archive

Extract an archive.

Archives can be MSIs, 7zip self extracting executables, or any archive format accepted by CMake's
built-in `tar`.

## Usage

```cmake
vcpkg_extract_archive(
    ARCHIVE <path>
    DESTINATION <path>
)
```

## Parameters

### ARCHIVE

Full path to the archive to extract.

### DESTINATION

Full path to a nonexistent path which will be created as a directory, into which the contents of
`ARCHIVE` are extracted.

## Source

[scripts/cmake/vcpkg\_extract\_archive.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_extract_archive.cmake)
