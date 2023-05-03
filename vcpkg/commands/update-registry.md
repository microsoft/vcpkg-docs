---
title: vcpkg x-update-registry
description: Command line reference for the vcpkg x-update-registry command. Updates dowloaded content for the supplied artifact registries.
ms.date: 05/03/2023
---
# vcpkg x-update-registry

[!INCLUDE [experimental](../../includes/experimental.md)]

## Synopsis

```console
vcpkg x-update-registry [--all] artifact-registry-name-or-uri...
```

## Description

Downloads the registry contents of the supplied artifact registry.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--all`

Updates all registries declared in currently active vcpkg-configurations.

## Example

```console
$ type vcpkg-configuration.json
{
  "default-registry": {
    "kind": "git",
    "baseline": "a871f0925e5def15672a1d3558c48a3320ee6f2c",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "artifact",
      "location": "https://aka.ms/vcpkg-ce-default",
      "name": "microsoft"
    },
    {
      "kind": "artifact",
      "location": "https://aka.ms/vcpkg-artifacts-cmsis",
      "name": "cmsis"
    },
    {
      "kind": "artifact",
      "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/msvc-experiments.zip",
      "name": "hello"
    }
  ]
}
$ .\vcpkg.exe x-update-registry microsoft
warning: vcpkg-artifacts is experimental and may change at any time.
Updating registry data from microsoft
Updated microsoft. It contains 22 metadata files.
$ .\vcpkg.exe x-update-registry https://aka.ms/vcpkg-artifacts-cmsis
warning: vcpkg-artifacts is experimental and may change at any time.
Updating registry data from cmsis
Updated cmsis. It contains 9 metadata files.
$ .\vcpkg.exe x-update-registry microsoft hello
warning: vcpkg-artifacts is experimental and may change at any time.
Updating registry data from microsoft
Updated microsoft. It contains 22 metadata files.
Updating registry data from hello
Updated hello. It contains 480 metadata files.
$ .\vcpkg.exe x-update-registry --all
warning: vcpkg-artifacts is experimental and may change at any time.
Updating registry data from microsoft
Updated microsoft. It contains 22 metadata files.
Updating registry data from cmsis
Updated cmsis. It contains 9 metadata files.
Updating registry data from hello
Updated hello. It contains 480 metadata files.
```
