---
title: Manifest mode
description: This article describes vcpkg's manifest mode concepts.
author: vicroms
ms.author: viromer
ms.date: 7/22/2024
ms.topic: concept-article

#CustomerIntent: As a vcpkg user, I want to learn more about manifest mode capabilities
---

# What is manifest mode?

vcpkg has two operation modes: [classic mode](classic-mode.md) and manifest
mode. For most users, we recommend manifest mode.

Manifest mode uses declarative JSON files to describe metadata about your
project or package. Manifest files are required to have the
[`vcpkg.json`](../reference/vcpkg-json.md) name.

Manifest mode is engaged by running the `vcpkg install` command while there's a
manifest file (`vcpkg.json`) in the working directory. Read ahead for details on
how to [install packages in manifest mode](#install-manifest-mode).

Manifest mode is also required to use advanced features like
[versioning](../users/versioning.md) and
[custom registries](../users/registries.md).

## Manifest files in ports

All vcpkg ports must include a `vcpkg.json` file that describes metadata about
the package they install.

vcpkg uses the metadata in the package manifest for various purposes, such as,
calculating dependency trees, searching for packages by name or description,
resolving features, etc.

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

## Manifest files in projects

The main purpose of using a manifest file in your project is to declare your
dependencies. When using a project manifest, you're able to specify version
constraints and overrides to lock specific versions of your dependencies. This 
feature is not available in classic mode.

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
      "repository": "https://github.com/microsoft/vcpkg-docs",
      "reference": "vcpkg-registry",
      "baseline": "768f6a3ad9f9b6c4c2ff390137690cf26e3c3453",
      "packages": [ "beicode", "beison" ]
    }
  ],
  "overlay-ports": [
    "C:\\dev\\my_vcpkg_ports"
  ]
}
```

## <a name="install-manifest-mode"></a> Installing packages in manifest mode

To install packages using a manifest file you use the
[`vcpkg install`](../commands/install.md) command without any package arguments.
The command must be executed from a directory containing a manifest
([`vcpkg.json`](../reference/vcpkg-json.md)) file, or the path to a manifest
file provided by using the `--x-manifest-root=<path>` option.

Packages installed in manifest mode will not be installed in the global
[`installed`](../reference/installation-tree-layout.md) directory as they do in
classic mode. Instead, each manifest gets its own installation directory named
`vcpkg_installed`; the `vcpkg_installed` directory is created in the same
directory that contains the manifest file.

Having independent installation trees per manifest allows separation of
dependencies between different projects. This circumvents a crucial limitation
of classic mode, which only allows one version of each port to be installed.
Manifest mode keeps versions of ports separated per project.

## Using features in project manifests

Manifest files can define additive sets of functionality, behavior, and
dependencies through the use of
["features"](../reference/vcpkg-json.md#features).

In your projects, you may define features to enable or disable dependencies that
apply to parts of your project. For example, if your project contains multiple
components, you may want to keep common dependencies in the `"dependencies"`
list but limit some others to their respective components.

To enable features of your project you can use one of the following methods:

- Pass the [`--x-feature`](../commands/install.md#feature) option to your `vpckg
  install` command.
- On CMake, set the
  [`VCPKG_MANIFEST_FEATURES`](../users/buildsystems/cmake-integration.md) before
  the first call to `project()`.
- On MSBuild, pass the `--x-feature` option via
  [`VcpkgAdditionalInstallOptions](../users/buildsystems/msbuild-integration.md#vcpkg-additional-install-options).

### Example: Features in project manifests

```json
{
  "name": "my-game",
  "dependencies": [ "grpc" ],
  "features": {
    "client": {
      "description": "client game executable",
      "dependencies": [ "sdl2", "bullet3" ]
    }, 
    "server": {
      "description": "multiplayer server executable",
      "dependencies": [ "proxygen" ]
    }, 
    "tests": {
      "description": "development tests",
      "dependencies": [ "gtest" ]
    }
  }
} 
```

To build only the "client" component's dependencies run:

```Console
vcpkg install --x-feature=client
```

## Next steps

Here are some tasks to try next:

- Complete the [manifest mode tutorial](../consume/manifest-mode.md)
- Read the [`vcpkg.json`](../reference/vcpkg-json.md) and
  [`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md)
  reference articles.
- Lock down your versions for repeatable builds using
  [versioning](../users/versioning.concepts.md)
