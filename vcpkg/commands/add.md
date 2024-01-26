---
title: vcpkg add
description: Reference for the vcpkg add command. Adds packages to the manifest.
author: JavierMatosD
ms.author: javiermat
ms.date: 01/26/2024
---
# vcpkg add

## Synopsis

```console
vcpkg add port [options] <port-name>...
```

## Description

The `vcpkg add port` command allows you to update the `vcpkg.json` manifest file by adding new package dependencies to your C++ project. 

You can specify one or multiple port names to add. It's also possible to define specific features of a port that you would like to include as a dependency. The manifest (`vcpkg.json`) will then be updated to reflect these changes.

### Examples

Sample manifest:

```json
{
  "name": "Example",
  "version": "1.0"
}
```

To add ports - `fmt`, `sqlitecpp`, `zlib`:

```console
vcpkg add port fmt sqlitecpp zlib
```

```json
{
  "name": "Example",
  "version": "1.0",
  "dependencies": [
    "fmt",
    "zlib",
    "sqlitecpp"
  ]
}
```

To specify port with feature - `sqlitecpp[sqlcipher]`

```console
vcpkg add port sqlitecpp[sqlcipher]
```

```json
{
  "name": "Example",
  "version": "1.0",
  "dependencies": [
    "fmt",
    {
      "name": "sqlitecpp",
      "features": [
        "sqlcipher"
      ]
    },
    "zlib"
  ]
}
```

> [!NOTE]
> The `vcpkg add port` command does not validate the existence of specified features for a port. Ensure that the feature exists before adding it to the manifest.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### port

`vcpkg add port <port-name>...` adds the specified ports to the manifest if it's not already present.
