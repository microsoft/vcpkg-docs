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

Default features in vcpkg are a set of features automatically included when a port is installed, unless explicitly disabled. These features ensure that a library maintains a baseline level of functionality, even as the complexity of its dependency graph increases.

>![NOTE]
> Not all ports contain default features.

## Role of default features

Default features are crucial for providing a user-friendly experience with libraries that offer modular functionalities through features. For instance, a library capable of extracting various archive formats might have each format as an optional feature. Without default features, the library might not perform any extraction out of the box, complicating initial use. Default features guarantee that essential functionalities are available immediately, enhancing usability.

## Specifying default features

When adding a library as a dependency in your `vcpkg.json` manifest, default features are included by default. To modify this behavior, you can use the `"default-features": false` attribute to disable them, allowing for a more customized setup based on your specific needs.

>![NOTE]
> Adding the `"default-features": false"` attribute to disable default features only works if specified in the top level manifest.

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


### Example 2: Disabling default features

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

## Default features in action

1. **Installing a port without specifying default features**:
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
- **Utilize Overrides and Baselines**: Leverage versioning and baseline features for more control over dependency resolutions.

For more information, see the following:

* [depend-info](../commands/depend-info.md)
* [overrides](../reference/vcpkg-json.md#overrides)
* [baselines](../users/versioning.md#baselines)
* [versioning](../users/versioning.md)

Adopting a clear strategy for managing default features will help streamline your project's dependency management, ensuring you include only what's necessary for your application's functionality.