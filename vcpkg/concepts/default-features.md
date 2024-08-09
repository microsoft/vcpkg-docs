---
title: Understanding default features in vcpkg
description: This article explains the concept of default features, their capabilities, and their expected contents in detail.
author: JavierMatosD
ms.author: javiermat
ms.date: 02/01/2024
ms.topic: concept-article
---

# Concept: Default features

This article describes the concept of default features, their implications, and strategies for effective management.

Default features are a set of [features](features.md) automatically included when a port is installed, unless explicitly disabled. These features ensure that a library maintains a baseline level of functionality, even as the complexity of its dependency graph increases.

> [!NOTE]
> Not all ports contain default features.

## Role of default features

Default features provide a user-friendly experience by ensuring essential functionalities are available in libraries with modular features.

For instance, a library capable of extracting various archive formats might have each format as an optional feature. Without default features, the library might not include any extraction algorithm out of the box, complicating initial use.

Packages installed by vcpkg will always include their default features, unless explicitly requested otherwise. This behavior applies to packages required directly by the user through a manifest file or vcpkg install invocation, and to packages required as dependencies.

### Disabling default features in manifest mode

Add the `"default-features": false` attribute to a dependency declaration to disable installation of its default features.

Disabling the automatic installation of default features is only possible for user requested dependencies.

If you want to disable default features for a transitive dependency, you need to promote it to a direct dependency on your project's manifest.

### Disabling default features in classic mode

Add the special `core` feature as part of the requested features of the packages you want to install.

> [!NOTE]
> Similar to how manifest mode works, if you want to disable default features for a transitive dependency; you need to promote it to a direct installation request in your command line.

### Example 1: Including a library with default features

```json
{
  "name": "my-application",
  "version": "0.15.2",
  "dependencies": [
    "extract-any"
  ]
}
```

In this scenario, `extract-any` is installed with its default features, ensuring immediate functionality.

### Example 2: Disabling default features (manifest mode)

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

Here, `extract-any` is installed without its default features, allowing for a tailored setup.

### Example 3: Disabling default features (classic mode)

```console
vcpkg install extract-any[core]
```

Here, `extract-any` is installed without its default features by using the special `core` feature.

## Default features interactions

1. **Installing a port with default features (default behavior)**:
    `vcpkg install extract-any` installs `extract-any` with its default features (`extract-any[format-a]`), due to the absence of specific feature requests.

1. **Specifying a feature without disabling defaults**:
    `vcpkg install extract-any[format-b]` leads to the installation of `extract-any` with both the specified (`format-b`) and default (`format-a`) features (`extract-any[format-a,format-b]`).

1. **Explicitly disabling default features**:
    `vcpkg install extract-any[core]` installs `extract-any` without any default features, as `[core]` explicitly excludes them.

1. **Transitive dependencies and default features**:
    `vcpkg install extract-any[format-b] portB` might result in `extract-any[format-a,format-b] portB` being installed if `portB` depends on `extract-any[format-a]`, but does not explicitly disable `extract-any`'s default features.

1. **Combining ports with varied feature specifications**:
    `vcpkg install extract-any[format-b] portB` installs `extract-any[format-a, format-b] portB`, including `extract-any`'s default features unless explicitly excluded.

1. **Comprehensive disabling of default features**:
    `vcpkg install extract-any[core] portB` ensures that `extract-any` is installed with only the explicitly requested features, excluding the defaults.

## Managing default features

To effectively manage default features within your projects:

- **Be Explicit**: Specify only the features you need. Use `[core]` to disable default features when necessary.
- **Inspect Dependencies**: Use `vcpkg depend-info <port>` to understand dependency resolutions and adjust your configurations accordingly.
- **Utilize Overrides and Baselines**: Leverage [versioning and baseline features](../users/versioning.concepts.md) for more control over dependency resolutions.

For more information, see the following:

* [depend-info](../commands/depend-info.md)
* [overrides](../reference/vcpkg-json.md#overrides)
* [baselines](../users/versioning.md#baselines)
* [versioning](../users/versioning.md)

Adopting a clear strategy for managing default features will help streamline your project's dependency management, ensuring you include only what's necessary for your application's functionality.