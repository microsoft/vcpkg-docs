---
title: Registries concepts
description: Learn about concepts relating to vcpkg registries.
author: vicroms
ms.author: viromer
ms.date: 8/21/2023
# TODO: Split this article apart
ms.prod: vcpkg
ms.topic: concept-article
---
# Registries concepts


## Package name resolution

vcpkg determines the responsible registry (or overlay) for each package before reaching out to the network. This prevents Package Dependency Confusion Attacks because name resolution does not depend on any external state.

The name resolution algorithm steps are:

- If the name is found in an [overlay](#overlays), use that overlay; otherwise
- If there is a [`"packages"`](../reference/vcpkg-configuration-json.md#registry-packages) pattern that matches the port name, use that registry; otherwise
- If the [default registry](../reference/vcpkg-configuration-json.md#default-registry) is not `null`, use that registry; otherwise
- Fail to resolve the port to a registry.

When there are multiple `"packages"` patterns that match the name, we break ties as follows:
1. **Exact match** -- `boost` is chosen over `boost*`
2. **Longest pattern** -- `boost*` is chosen over `b*`
3. **First match** -- The first registry declaring the best pattern is chosen

### Example #1: Package name resolution

`vcpkg-configuration.json`
```json
{
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/northwindtraders/vcpkg-registry",
      "baseline": "dacf4de488094a384ca2c202b923ccc097956e0c",
      "packages": ["bei*"]
    },
    {
      "kind": "git",
      "repository": "https://github.com/vicroms/vcpkg-registry",
      "baseline": "dacf4de488094a384ca2c202b923ccc097956e0c",
      "packages": ["beicode", "bei*"]
    }
  ]
}
```

`vcpkg.json`
```json
{
  "dependencies": [ 
    "beicode", 
    "beison",
    "fmt"
  ],
  "builtin-baseline": "7e7c62d863b1bf599c1d104b76cd8b74475844d4"
}
```

Given this configuration, each package name resolves to:

* `beicode`: from registry `https://github.com/vicroms/vcpkg-registry` (exact match on `beicode`)
* `beison`: from registry `https://github.com/northwindtraders/vcpkg-registry` (pattern match on `beison` and declared first in `"registries"` array)
* `fmt`: from default registry (no matches)

Because multiple registries declare `bei*`, vcpkg will also emit a warning:

```no-highlight
Found the following problems in configuration (path/to/vcpkg-configuration.json):
$ (a configuration object): warning: Package "bei*" is duplicated.
    First declared in:
        location: $.registries[0].packages[0]
        registry: https://github.com/northwindtraders/vcpkg-registry
    The following redeclarations will be ignored:
        location: $.registries[1].packages[1]
        registry: https://github.com/vicroms/vcpkg-registry
```

### Example #2: Assign multiple patterns to the default registry

You can change the default registry in two ways. First, by defining the [`"default-registry"`](../reference/vcpkg-configuration-json.md#default-registry):
```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/Microsoft/vcpkg",
    "baseline": "e79c0d2b5d72eb3063cf32a1f7de1a9cf19930f3"
  }
}
```

Second, by setting the `"default-registry"` to `null` and using the `"*"` pattern in the `"registries"` array:
```json
{
  "default-registry": null,
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "e79c0d2b5d72eb3063cf32a1f7de1a9cf19930f3",
      "packages": ["*"]
    }
  ]
}
```

An advantage of the second form is that you can add more entries to the packages array, while the `"default-registry"` object doesn't allow you to define a packages array at all. This difference becomes important in cases where you need to ensure that a package comes from the default registry.

Let's consider a registry that provides the Qt Framework libraries.

`vcpkg-configuration.json`
```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/Microsoft/vcpkg",
    "baseline": "7e7c62d863b1bf599c1d104b76cd8b74475844d4"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/custom-qt/custom-qt-registry",
      "baseline": "adfc4de488094a384ca2c202b923ccc097956e0c",
      "packages": ["qt*"]
    }
  ]
}
```

And the following project dependencies:

`vcpkg.json`
```json
{
  "dependencies": [ 
    "qt5", 
    "qt-advanced-docking-system", 
    "qtkeychain" 
  ]
}
```

The `"qt*"` pattern matches all port names in `vcpkg.json`. But there is a problem! The ports `qt-advanced-docking-system` and `qtkeychain` are not part of the official Qt Framework libraries and since vcpkg won't be able to find the ports in the custom registry the installation will fail.

This can be fixed by assigning these packages to the default registry instead. We accomplish that by changing the way we declare the default registry and adding `qt-advanced-docking-system` and `qtkeychain` to its `"packages"` array:

`vcpkg-configuration.json`
```json
{
  "default-registry": null,
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "e79c0d2b5d72eb3063cf32a1f7de1a9cf19930f3",
      "packages": ["*", "qt-advanced-docking-system", "qtkeychain"]
    },
    {
      "kind": "git",
      "repository": "https://github.com/custom-qt/custom-qt-registry",
      "baseline": "adfc4de488094a384ca2c202b923ccc097956e0c",
      "packages": ["qt*"]
    }
  ]
}
```

Because exact matches are preferred over pattern matches, `qt-advanced-docking-system` and `qtkeychain` will resolve to the default registry.

## <a name="overlays"></a> Overlays

Overlays are a way to extend vcpkg with additional ports and additional triplets, without creating a full registry. Overlays are considered before performing any registry lookups or versioning and will replace any builtin triplets or ports.

Overlay settings are evaluated in this order:

1. Overlays from the [command line](../commands/common-options.md#overlay-ports) in the order passed; then
2. Overlays from [`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md#overlay-ports) in order; then
3. Overlays from the `VCPKG_OVERLAY_[PORTS|TRIPLETS]` [environment variables](config-environment.md#vcpkg_overlay_ports) in order

