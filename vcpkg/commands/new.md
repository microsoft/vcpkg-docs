---
title: vcpkg new
description: Command line reference for the vcpkg new command. Creates a new manifest in the current directory.
ms.author: bion
author: BillyONeal
ms.date: 01/10/2024
ms.topic: reference
---
# vcpkg new

## Synopsis

```console
vcpkg new --application
```

```console
vcpkg new --name hello --version 1.0 [--version-relaxed | --version-date | --version-string]
```

Creates a manifest file ([`vcpkg.json`](../reference/vcpkg-json.md)) and a default configuration file
([`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md)) ready to use in your project in the current
working directory.

Use the `--application` flag if your project is an end-user application.

If not using `--application`, `--name` and `--version` are required. `--version-date`,
`--version-relaxed`, or `--version-string` can force that a particular version format is selected.
For more information about the versioning formats, see
[Versioning reference](../users/versioning.md).

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--application`

Creates a manifest suitable for use in applications, removing the requirement to supply name and
version.

### `--name`

The name to write into the manifest.

### `--version`

Indicates the version to write into the manifest. If none of the other version format switches are
passed, infers the appropriate form to use based on the form of the input.

### `--version-relaxed`

Indicates that the version to write into the manifest is a 'relaxed' version. Relaxed versions
obey semantic versioning's format, but allow any number of numeric values. See [versioning reference](../users/versioning.md#version) for more information.

### `--version-date`

Indicates that the version to write into the manifest is a 'date' version. Date versions are an
ISO 8601 date (e.g. "YYYY-MM-DD"). See [versioning reference](../users/versioning.md#version-date) for more information.

### `--version-string`

Indicates that the version to write into the manifest is a 'string' version with no ordering

semantics. See [versioning reference](../users/versioning.md#version-string) for more information.

## Examples

> [!NOTE]
> `vcpkg new` generates metadata in `vcpkg.json` and `vcpkg-configuration.json` files. This includes
> integration with the experimental artifacts experience. In particular, the registries with
> `"kind": "artifact"` are for the experimental vcpkg-artifacts feature.

### With `--application`

```console
$ vcpkg new --application
$ type vcpkg.json
{}
$ type .\vcpkg-configuration.json
{
  "default-registry": {
    "kind": "git",
    "baseline": "64adda19c86e89526b5e27703a193c14477cce07",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "artifact",
      "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
      "name": "microsoft"
    }
  ]
}
```

### With `--name` and `--version`

```console
$ vcpkg new --name hello --version 2023-07-05
$ type vcpkg.json
{
  "name": "hello",
  "version-date": "2023-07-05"
}
$ type .\vcpkg-configuration.json
{
  "default-registry": {
    "kind": "git",
    "baseline": "64adda19c86e89526b5e27703a193c14477cce07",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "artifact",
      "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
      "name": "microsoft"
    }
  ]
}
```

### With explicit version format selection

```console
$ vcpkg new --name hello --version 1.0 --version-date
error: `1.0` is not a valid date version. Dates must follow the format YYYY-MM-DD and disambiguators must be dot-separated positive integer values without leading zeroes.
$ vcpkg new --name hello --version 1.0 --version-string
$ type vcpkg.json
{
  "name": "hello",
  "version-string": "1.0"
}
$ type .\vcpkg-configuration.json
{
  "default-registry": {
    "kind": "git",
    "baseline": "64adda19c86e89526b5e27703a193c14477cce07",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "artifact",
      "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
      "name": "microsoft"
    }
  ]
}
```
