---
title: vcpkg format-manifest
description: Reference for the vcpkg format-manifest command. Formats vcpkg.json files and converts CONTROL files to vcpkg.json format.
author: JavierMatosD
ms.author: javiermat
ms.date: 01/10/2024
---

# vcpkg format-manifest

## Synopsis

```console
vcpkg format-manifest [--all] [--convert-control] [<path>...]
```

## Description

Formats [manifest (`vcpkg.json`)](../reference/vcpkg-json.md) files
and converts control (`CONTROL`) files into the manifest format.
If both `CONTROL` and `vcpkg.json` file are present in the same directory, the command will exit with an error.

## Examples

To format a specific manifest or control file:

```console
vcpkg format-manifest /path/to/vcpkg.json
```

To format all manifest files in the `${VCPKG_ROOT}/ports` directory:

```console
vcpkg format-manifest --all
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--all`

Formats all manifest files in the `${VCPKG_ROOT}/ports` directory.

### `--convert-control`

Converts `CONTROL` files into `vcpkg.json` format when found in the ports
directory.
