---
title: vcpkg license-report
description: Reference for the vcpkg license-report command. Prints licenses of all installed packages.
author: BillyONeal
ms.author: bion
ms.date: 06/04/2025
---
# vcpkg license-report

## Synopsis

```console
vcpkg license-report
```

## Description

The `vcpkg license-report` command prints the licenses of all installed packages.

### Examples

```console
> vcpkg install zlib
[...]
> vcpkg license-report
Installed contents are licensed to you by owners. Microsoft is not responsible for, nor does it grant any licenses to, third-party packages.
Installed packages declare the following licenses:
MIT
Zlib
```

## Options

All vcpkg commands support a set of [common options](common-options.md).
