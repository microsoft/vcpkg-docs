---
title: Understanding default features in vcpkg
description: This article explains the concept of default features, their capabilities, and their expected contents in detail.
author: JavierMatosD
ms.author: javiermat
ms.date: 02/01/2024
ms.topic: concept-article
---

# Concept: Default features

This article delves into the concept of default features, their implications, and strategies for effective management.

Default features in vcpkg are a set of features automatically included when a port is installed, unless explicitly disabled. These features ensure that a library maintains a baseline level of functionality, even as the complexity of its dependency graph increases.

## Role of default features

Default features are crucial for providing a user-friendly experience with libraries that offer modular functionalities through features. For instance, a library capable of extracting various archive formats might have each format as an optional feature. Without default features, the library might not perform any extraction out of the box, complicating initial use. Default features guarantee that essential functionalities are available immediately, enhancing usability.

## Specifying default features

When adding a library as a dependency in your `vcpkg.json` manifest, default features are included by default. To modify this behavior, you can use the `"default-features": false` attribute to disable them, allowing for a more customized setup based on your specific needs.

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

1. Installing a port without specifying default features:
    `vcpkg install portA` installs `portA` with its default features (`portA[i]`), due to the absence of specific feature requests.

2. Specifying a feature without disabling defaults:
    `vcpkg install portA[j]` leads to the installation of `portA` with both the specified (`j`) and default (`i`) features (`portA[i,j]`).

3. Explicitly disabling default features:
    `vcpkg install portA[core]` installs `portA` without any default features, as `[core]` explicitly excludes them.

4. Transitive dependencies and default features:
    `vcpkg install portA[j] portB` might result in `portA[i,j] portB` being installed if `portB` depends on `portA[j]`, but does not explicitly disable `portA`'s default features.

5. Combining ports with varied feature specifications:
    `vcpkg install portA[j] portB` installs `portA[i,j] portB`, including `portA`'s default features unless explicitly excluded.

6. Comprehensive disabling of default features:
    `vcpkg install portA[core] portB` ensures that `portA` is installed with only the explicitly requested features, excluding the defaults.

## Managing default features

To effectively manage default features within your projects:

- **Be Explicit**: Specify only the features you need. Use `[core]` to disable default features when necessary.
- **Inspect Dependencies**: Use `vcpkg depend-info <port>` to understand dependency resolutions and adjust your configurations accordingly.
- **Utilize Overrides and Baselines**: Leverage versioning and baseline features for more control over dependency resolutions.

Adopting a clear strategy for managing default features will help streamline your project's dependency management, ensuring you include only what's necessary for your application's functionality.