---
title: Manifest mode
description: Use a manifest with vcpkg to configure libraries on a per project basis.
ms.date: 2/10/2023
---
# Manifest mode

vcpkg has two modes for consuming packages - [Classic mode](classic-mode.md) and [Manifest mode](manifests.md). For most users, we recommend Manifest mode.

In Manifest mode, vcpkg creates separate *installed trees* for each project and configuration. This allows separate projects to use different versions of libraries. The [`vcpkg.json` file](../reference/vcpkg-json.md) and optional [`vcpkg-configuration.json` file](../reference/vcpkg-configuration-json.md) form a project's _manifest_. The manifest declares the project's direct dependencies, version constraints, and registries used.

Check out the [manifest CMake example](../examples/manifest-mode-cmake.md) for an example project using CMake and manifest mode.

Check out the [`vcpkg.json` Syntax Reference](../reference/vcpkg-json.md) for a full list of fields and functionality.

## How does it work?

In manifest mode, vcpkg is integrated with CMake via a [toolchain](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html), which must be [specified](https://github.com/microsoft/vcpkg#using-vcpkg-with-cmake) when the project is configured. The toolchain file is evaluated by CMake via [`include()`](https://cmake.org/cmake/help/latest/command/include.html) the first time CMake's [`project()`](https://cmake.org/cmake/help/latest/command/project.html) command is called. This evaluation sequence means that any vcpkg CMake variables must be set before the first call to `project()`. Note that changing the triplet requires a clean reconfigure since the results of `find_*` calls are stored in cache variables which won't get refreshed. During configuration, the toolchain CMake file effectively runs `vcpkg install`, with a project-local build tree, thus executing full builds of all manifest dependencies _during CMake configuration_.

## Example

```json
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg/master/scripts/vcpkg.schema.json",
  "name": "my-application",
  "version": "0.15.2",
  "dependencies": [
    "boost-system",
    {
      "name": "cpprestsdk",
      "default-features": false
    },
    "libxml2",
    "yajl"
  ]
}
```

## Features

Manifests can define additive sets of functionality, behavior, and dependencies called _features_.

Each feature should be additive with other features: if a project provides features `A` and `B`, then it should build successfully with both `A` and `B` enabled together. Features should not be used to define alternatives. For example, features cannot be used to choose between multiple, exclusive graphics APIs since it is not possible to choose both. Features can depend on other features within same project if the project's manifest has a [`"name"`](../reference/vcpkg-json.md#name).

The set of features available are defined by the [`"features"` field](../reference/vcpkg-json.md#features).

### Example 1: Multiple File Formats

For example, an image manipulation library might optionally support several different image types by depending on different sets of other libraries.

```json
{
  "name": "my-image-lib",
  "version": "0.1",
  "features": {
    "png": { "description": "Support PNG files", "dependencies": ["libpng"]},
    "jpeg": { "description": "Support JPEG files", "dependencies": ["libjpeg-turbo"]},
    "tiff": { "description": "Support TIFF files", "dependencies": ["libtiff"]},
  }
}
```

In a single dependency graph, multiple libraries can depend on features from a common core library.

```json
{
  "name": "library-a",
  "version": "1",
  "dependencies": [{"name": "my-image-lib", "features": ["png"]}]
}
```
```json
{
  "name": "library-b",
  "version": "1",
  "dependencies": [{"name": "my-image-lib", "features": ["jpeg"]}]
}
```

When resolving these dependencies, all required features and dependencies will be unioned together. In this example, if a top-level project used `library-a` and `library-b`, they would see an install plan similar to:

```
libjpeg-turbo[core]
libpng[core]
library-a[core]
library-b[core]
my-image-lib[core,png,jpeg]
```

The build of `my-image-lib` would activate PNG and JPEG support but deactivate TIFF support.

### Example 2: Multiple Related Projects in a Single Repository

When a single repository contains several separate buildable components, for example client and server applications with some shared code, developers of each piece may want to avoid installing expensive dependencies required by other pieces.

```json
{
  "name": "my-game",
  "dependencies": ["grpc"],
  "features": {
    "client": { "description": "Client Game Executable", "dependencies": ["sdl2", "bullet3"]},
    "server": { "description": "Multiplayer Server Executable", "dependencies": ["proxygen"]},
    "tests": { "description": "Build tests", "dependencies": ["gtest"] }
  }
}
```

Individual developers can then select which features to install:

- On the [install command line](../commands/install.md), pass [`--x-feature=`](../commands/install.md#feature)
- When using [CMake Integration](buildsystems/cmake-integration.md), set [`VCPKG_MANIFEST_FEATURES`](buildsystems/cmake-integration.md#vcpkg_manifest_features) before the `project()` command.
- When using [MSBuild Integration](buildsystems/msbuild-integration.md), pass `--x-feature=` via [`VcpkgAdditionalInstallOptions`](buildsystems/msbuild-integration.md#vcpkg-additional-install-options)

### Default Features

Default features are a set of features to be automatically activated if the top-level project does not explicitly request a build without them. Default features are intended to ensure a minimum level of functionality regardless of how complex and customizable the dependency graph of a project grows. They are not intended to model "curation" or "suggestions".

For example, consider a library `"extract-any"` that supports over 10 different archive formats, including several that are quite obscure. Because they are all optional, if none are selected the library is not functional: it cannot extract any files.

Default features ensure that a user simply running:
```
vcpkg install extract-any
```
will get a baseline level of functionality, for example automatically selecting `.zip` and `.tar.gz` decompressors.
