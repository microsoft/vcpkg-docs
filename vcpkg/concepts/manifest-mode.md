---
title: Manifest mode
description: This article describes vcpkg's manifest mode concepts.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: concept-article

#CustomerIntent: As a vcpkg user, I want to learn more about manifest mode capabilities
---

# What is manifest mode?

vcpkg has two operation modes: classic mode and manifest mode. This article
describes the capabilities of manifest mode, which is the recommended
mode for most users.

Manifest mode uses declarative JSON files to describe metadata about your
project or package. In any case, the name of this file is 
[`vcpkg.json`](../reference/vcpkg-json.md).

Manifest mode is also required to use advanced features like
[versioning](../users/versioning.md) and 
[custom registries](../users/registries.md).

## Manifest files in your packages

All vcpkg ports must include a `vcpkg.json` file that describes metadata about
the package they install.

vcpkg uses the metadata in the package manifest for various purposes, such as,
calculating dependency trees, searching for packages by name or description,
resolving version constraints, etc.

### Package manifest example

```json
{
  "name": "fmt",
  "version": "10.1.1",
  "description": "Formatting library for C++. It can be used as a safe alternative to printf or as a fast alternative to IOStreams.",
  "homepage": "https://github.com/fmtlib/fmt",
  "license": "MIT",
  "dependencies": [
    {
      "name": "vcpkg-cmake",
      "host": true
    },
    {
      "name": "vcpkg-cmake-config",
      "host": true
    }
  ]
}
```

## Manifest files in your projects
The main purpose of using a manifest file in your project is to declare your
dependencies. When used on a project, you're also able to specify version 
constraints and overrides to lock specific versions of your dependencies.

### Project manifest example

```json
{
  "dependencies": [ "fmt", "zlib" ],
  "builtin-baseline": "3426db05b996481ca31e95fff3734cf23e0f51bc",
  "overrides": [
    { "name": "zlib", "version": "1.2.8" }
  ]
}
```

## Configuration file
vcpkg can be configured through a `vcpkg-configuration.json` file to add more
[package registries](../users/registries.md) or 
[overlay ports and triplets](../concepts/overlay-ports.md) locations.

### Configuration file example

```json
{
  "default-registry": {
    "kind": "git",
    "baseline": "7476f0d4e77d3333fbb249657df8251c28c4faae",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/northwindtraders/vcpkg-registry",
      "baseline": "dacf4de488094a384ca2c202b923ccc097956e0c",
      "packages": [ "beicode", "beison" ]
    }
  ],
  "overlay-ports": [
    "C:\\dev\\my_vcpkg_ports"
  ]
}
```

## Next steps

Here are some tasks to try next:

* Lock down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reuse binaries across local or continuous integration runs using [binary caching](../users/binarycaching.md)
* Manage your private libraries using custom [registries](../maintainers/registries.md)
