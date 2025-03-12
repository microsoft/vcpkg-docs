---
title: Offline Usage of vcpkg
description: Use export, asset caching, and git to develop in offline or air-gapped networks.
author: roschuma
ms.author: roschuma
ms.topic: concept-article
ms.date: 03/12/2025
---
# Offline Usage of vcpkg

This document details two robust methods for deploying vcpkg in environments without live internet connectivity.

## Method 1: Prebuild and Export Packages

This method uses the export command to create a comprehensive package bundle on an internet-connected system. The process compiles binaries, headers, and all transitive dependencies into a self-contained SDK bundle suitable for offline deployment.

Get started with the [Export Tutorial – Creating a self-contained SDK bundle](../produce/export-compiled-dependencies.md).

### Pros and Cons

**Pros:**
- All necessary components are bundled together into a single archive using the export command.
- Easy to get started.
- Extremely reliable.

**Cons:**
- Updates and source development must be performed on a connected system.
- Involves additional maintenance due to frequent updates.
- Handling offline-only private libraries is not possible.
- Does not scale well to multiple diverse consumers -- each needs a separate bundle.

## Method 2: Redirect External Resource Access

This strategy enables controlled access to external resources by combining Git URL redirection with asset caching. It is an effective alternative where a complete offline bundle is not essential.

### Git Redirection

The Git configuration below demonstrates how to redirect requests from the standard repository to an internal mirror:

```ini
[url "https://internal/mirror/of/vcpkg"]
    insteadOf = https://github.com/microsoft/vcpkg
```

This configuration can also be applied via the command line:

```sh
git config --global url."https://internal/mirror/of/vcpkg".insteadOf "https://github.com/microsoft/vcpkg"
```

### Asset Caching

Asset caching stores downloads locally or on a remote storage solution, enhancing build performance and increasing network security. Detailed instructions are available in both the [asset caching tutorial](../consume/asset-caching.md) and the [asset caching configuration reference](../users/assetcaching.md).

### Pros and Cons

**Pros:**
- Accelerates builds by reusing cached assets.
- Removes dependency on continuous internet connectivity.
- Enables rebuilding offline, making toolchain or flag changes easier.
- Scales well to multiple diverse consumers with different subsets of libraries.

**Cons:**
- Involves additional setup.
- Requires maintenance of internal mirrors and caching infrastructure.

## Conclusion

Deploying vcpkg in an air-gapped environment can be achieved through:
- Prebuilding and exporting a complete package bundle.
- Configuring Git redirection combined with asset caching for controlled external access.

Choose the method that aligns best with your infrastructure and deployment requirements.

## Additional Resources

1. [Export Command](../commands/export.md)
2. [Asset Caching Configuration Reference](../users/assetcaching.md)
3. [Using Git URL Redirection (insteadOf)](https://git-scm.com/docs/git-config#Documentation/git-config.txt-url)
