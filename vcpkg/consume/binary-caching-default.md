---
title: "Default local vcpkg binary cache"
description: Learn about the default vcpkg binary cache
author: vicroms
ms.author: viromer
ms.date: 9/25/2023
ms.prod: vcpkg
ms.topic: concept-article
---
# Default local vcpkg binary cache

By default, vcpkg creates a local binary cache. This saves time when installing a package that has been previously installed in the same machine.

The location of the default binary cache depends on your operating system.

## [Windows](#tab/default-windows)

By default, binary caching is enabled at the first valid location among the following:

* The location specified by the `VCPKG_DEFAULT_BINARY_CACHE` environment variable
* `%LOCALAPPDATA%\vcpkg\archives`
* `%APPDATA%\vcpkg\archives`

## [Unix](#tab/unix)

By default, binary caching is enabled at the first valid location among the following:

* The location specified by the `VCPKG_DEFAULT_BINARY_CACHE` environment variable
* `$XDG_CACHE_HOME/vcpkg/archives`
* `$HOME/.cache/vcpkg/archives`

---

Use the `VCPKG_DEFAULT_BINARY_CACHE` environment variable to change the default binary cache
location.

The default binary cache uses a [`files` provider](../users/binarycaching.md#files). This
type of binary cache provider stores binary packages as compressed archives in a filesystem
directory.

## Disabling the default binary cache

To disable the default binary cache, set the [`VCPKG_BINARY_SOURCES` environment variable](../users/binarycaching.md#configuration-syntax) to `clear`.

### [Windows](#tab/disable-windows)

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear"
```

### [Unix](#tab/disable-unix)

```bash
VCPKG_BINARY_SOURCES=clear
```

---


## Next steps

> [!div class="nextstepaction"]
> [Set up a local binary cache](binary-caching-local.md)

Here are other tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a local binary cache](binary-caching-localmd)
* [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Packages](binary-caching-github-packages.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Actions Cache](binary-caching-github-actions-cache.md)
