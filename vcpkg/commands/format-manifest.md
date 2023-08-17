---
title: vcpkg format-manifest
description: Reference for the vcpkg format-manifest command. Formats vcpkg.json files and converts CONTROL files to vcpkg.json format.
author: JavierMatosD
ms.author: javiermat
ms.date: 08/17/2023
ms.prod: vcpkg
---

# vcpkg format-manifest

## Synopsis

```console
vcpkg format-manifest [--all] [--convert-control] [<path>...]
```

## Description
Formats manifest (`vcpkg.json`) files and converts control (`CONTROL`) files into the manifest format.

[!NOTE]
If both `CONTROL` and `vcpkg.json` file are present in the same directory, the command will exit with an error.

## Examples

To format all manifest files in the ports directory:
```console
vcpkg format-manifest --all
```

To convert all `CONTROL` files in the ports directory to `vcpkg.json` format:
```console
vcpkg format-manifest --all --convert-control
```

To format a specific manifest or control file:
```console
vcpkg format-manifest /path/to/vcpkg.json
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--all`

Formats all manifest files in the ports directory.

### `--convert-control`

Converts `CONTROL` files into `vcpkg.json` format when found in the ports directory.

[!NOTE]
Using `--convert-control` option with the `--all` option doesn't do anything: files passed explicitly are converted automatically.