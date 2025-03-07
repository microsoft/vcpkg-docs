---
title: vcpkg update
description: Command line reference for the vcpkg update command.
ms.date: 01/10/2023
---
# vcpkg update

## Synopsis

```console
vcpkg update [options]
```

## Description

Compares the set of [Classic mode] installed packages against the current available versions.

To remove outdated versions without installing replacements, use [`vcpkg remove --outdated`](remove.md). To remove and install the newer versions, use [`vcpkg upgrade`](upgrade.md).

## Example

```console
$ vcpkg update
Using local portfile versions. To update the local portfiles, use `git pull`.
The following packages differ from their port versions:
        corrade:x64-windows              2020.06#4 -> 2020.06#5
        magnum:x64-windows               2020.06#11 -> 2020.06#12
        openal-soft:x64-windows          1.22.2#5 -> 1.23.0
        ragel:x64-windows                6.10#4 -> 6.10#5
        sdl2:x64-windows                 2.26.3#1 -> 2.26.4
To update these packages and all dependencies, run
.\vcpkg upgrade'
To only remove outdated packages, run
.\vcpkg remove --outdated
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

[Classic mode]: ../concepts/classic-mode.md
