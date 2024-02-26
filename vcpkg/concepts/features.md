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

Features represent sets of functionality, behavior, and dependencies that can be selectively added to a package or project upon installation. 

By design, features should follow these principles:

- Additive: enabling a feature should provide new functionality otherwise missing from the package without disabling any other functionality.
- Non-exclusive: enabling a feature should not prevent other features from being installed.

Features should not be used to define alternative sets of functionalities. For example, a graphics library should not use features to choose between exclusive graphics backends, since it's not possible to install all of them at the same time.

Features can have the following effects on a package's dependencies:

- Add new dependencies, including dependencies on other features from the same package.
- Enable new features on existing dependencies.

The set of features available are defined by the [`"features"` field](../reference/vcpkg-json.md#features).

### Example 1: Multiple file formats

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

>![NOTE]
> Default features are not intended to model "curation" or "suggestions".

For example, consider a library `"extract-any"` that supports over 10 different archive formats, including several that are quite obscure. Because they are all optional, if none are selected the library is not functional: it cannot extract any files.

Default features ensure that a user who simply adds `"extract-any"` to the list of dependencies in their `vcpkg.json` will get a baseline level of functionality; for example, automatically selecting `.zip` and `.tar.gz` decompressors.

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

## Dependency resolution

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

```json
{
  "name": "project-using-a-and-b",
  "version": "1",
  "dependencies": [
    "library-a",
    "library-b"
  ]
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

## Advanced usage

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
