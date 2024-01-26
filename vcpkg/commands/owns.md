---
title: vcpkg owns
description: Reference for the vcpkg owns command. Determines which installed package owns a particular file or files matching a substring.
author: JavierMatosD
ms.author: javiermat
ms.date: 01/10/2024
---
# vcpkg owns

## Synopsis

```console
vcpkg owns <file-substr>
```

## Description

The `owns` command is used to search for installed packages that own a particular file or files that match a substring (`<file-substr>`).

The command outputs a list of packages that have files matching the given substring. Each entry is in the format:

```console
<package-name>: <file-path>
```

## Examples

To search for installed packages that own a file matching the substring `zlib.h`.

```console
vcpkg owns zlib.h

-- zlib:x86-windows: x86-windows/include/zlib.h
-- zlib:x64-windows: x64-windows/include/zlib.h
-- bzip2:x64-windows: x64-windows/include/bzlib.h
```

## Options

All vcpkg commands support a set of [common options](common-options.md).
