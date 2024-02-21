---
title: Understanding features in vcpkg
description: This article explains the concept of features, their capabilities, and their expected contents in detail.
author: JavierMatosD
ms.author: javiermat
ms.date: 02/21/2024
ms.topic: concept-article
---

# Concept: features

## Features

Manifests can define additive sets of functionality, behavior, and dependencies called _features_.

Each feature should be additive with other features: if a project provides features `A` and `B`, then it should build successfully with both `A` and `B` enabled together. Features should not be used to define alternatives. For example, features cannot be used to choose between multiple, exclusive graphics APIs since it is not possible to choose both. Features can depend on other features within same project if the project's manifest has a [`"name"`](../reference/vcpkg-json.md#name).

The set of features available are defined by the [`"features"` field](../reference/vcpkg-json.md#features).

### Example 1: Multiple file formnats

An image manipulation library, for instance, might support several different image types by depending on different sets of other libraries.

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


## Default features

Default features are a set of features to be automatically activated if the top-level project does not explicitly request a build without them. Default features are intended to ensure a minimum level of functionality regardless of how complex and customizable the dependency graph of a project grows. 

![NOTE]
> They are not intended to model "curation" or "suggestions".

For example, consider a library `"extract-any"` that supports over 10 different archive formats, including several that are quite obscure. Because they are all optional, if none are selected the library is not functional: it cannot extract any files.

Default features ensure that a user who simply adds `"extract-any"` to the list of dependencies in their `vcpkg.json` will get a baseline level of functionality, for example automatically selecting `.zip` and `.tar.gz` decompressors.

### Example 2: Default features in action

When a user adds `"extract-any"` to their `vcpkg.json` without specifying features, the default features (e.g., support for `.zip` and `.tar.gz` formats) are automatically included, ensuring basic functionality.

```
{
  "name": "my-application",
  "version": "0.15.2",
  "dependencies": [
    "extract-any"
  ]
}
```

If the user wants to explicitly disable the default features, they can do so by adding `"default-features": false` to the dependency:

```json
{
  "name": "my-application",
  "version": "0.15.2",
  "dependencies": [
    {
      "name": "extract-any",
      "default-features": false
    }
  ]
}
```

Alternatively, if using vcpkg in [classic mode](../consume/classic-mode.md), you can disabled default features via the `core` feature. For example, `vcpkg install extract-any[core]` installs `extract-any` without any default features, as `[core]` explicitly excludes them.

For more information, check out the [default features article](default-features.md).

## Dependency Resolution

When using vcpkg, dependency resolution plays a crucial role, especially when dealing with features that have interdependencies. To illustrate, consider the following scenario involving an image manipulation library:


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

In scenarios where different libraries depend on various features of a common library, vcpkg ensures that all required features and dependencies are considered. For instance, if `library-a` requires the `png` feature and `library-b` requires the `jpeg` feature from `my-image-lib`, the dependency graph would look like this:

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

When these dependencies are resolved, vcpkg combines all necessary features and dependencies to form a comprehensive installation plan. In this example, a project depending on both `library-a` and `library-b` would result in an installation plan that includes both `PNG` and `JPEG` support from `my-image-lib`, but not `TIFF`:

```
libjpeg-turbo[core]
libpng[core]
library-a[core]
library-b[core]
my-image-lib[core,png,jpeg]
```

This mechanism ensures that the build of `my-image-lib` is optimized for the required features, providing support for `PNG` and `JPEG` while excluding unnecessary `TIFF` support.

## Advanced Usage

### Example 3: Multiple Related Projects in a Single Repository

When a single repository contains several separate buildable components, such as client and server applications with some shared code, developers of each piece may want to avoid installing expensive dependencies required by other pieces.

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
- When using [CMake Integration](../users/buildsystems/cmake-integration.md), set [`VCPKG_MANIFEST_FEATURES`](../users/buildsystems/cmake-integration.md#vcpkg_manifest_features) before the `project()` command.
- When using [MSBuild Integration](../users/buildsystems/msbuild-integration.md), pass `--x-feature=` via [`VcpkgAdditionalInstallOptions`](../users/buildsystems/msbuild-integration.md#vcpkg-additional-install-options)

For more information, see the following:

* [depend-info](../commands/depend-info.md)
* [overrides](../reference/vcpkg-json.md#overrides)
* [baselines](../users/versioning.md#baselines)
* [versioning](../users/versioning.md)

Adopting a clear strategy for managing default features will help streamline your project's dependency management, ensuring you include only what's necessary for your application's functionality.