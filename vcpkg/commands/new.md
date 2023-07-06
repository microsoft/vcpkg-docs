---
title: vcpkg new
description: Command line reference for the vcpkg new command. Creates a new manifest in the current directory.
ms.date: 07/05/2023
---
# vcpkg new

## Synopsis

```console
vcpkg new --application
```

```console
vcpkg new --name hello --version 1.0
```

## Description

Writes a new manifest into the current working directory. In `--application` mode, the manifest
contains no name or version, suitable for use in an end user application that ships in binary form.

Otherwise, `--name` and `--version` are required. `--version-date`, `--version-relaxed`, or
`--version-string` can force that a particular version format is selected. For more information
about the versioning formats, see [Versioning reference](../users/versioning.md).

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--application`

Indicates that the manifest will be used for an end user application shipped in binary forms.

### `--name`

The name to write into the manifest.

### `--version`

Indicates the version to write into the manifest. If none of the other version format switches are
passed, guesses the appropriate form to use based on the form of the input.

### `--version-relaxed`

Indicates that the version to write into the manifest is a 'relaxed' version. Relaxed versions
obey semantic versioning's format, but allow any number of numeric values.

### `--version-date`

Indicates that the version to write into the manifest is a 'date' version. Date versions are an
ISO 8601 date.

### `--version-string`

Indicates that the version to write into the manfest is a 'string' version with no ordering
semantics.

## Examples

### `--application`

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

### `--name` and `--version`

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

### Asserting particular version formats

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
