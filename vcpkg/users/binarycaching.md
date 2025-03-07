---
# Concepts/Binary Caching
title: Binary Caching
description: Reuse binaries built with vcpkg across different projects and machines.
author: vicroms
ms.author: viromer
ms.date: 5/1/2024
ms.topic: concept-article
---

# Binary Caching

Most ports in the [vcpkg public registry](<https://github.com/Microsoft/vcpkg>) are built from
source. By building from source, vcpkg can ensure maximum compatibility by using
the same environment, build-tools, compiler flags, linker flags, and other configurations that you
use in your project to build your dependencies.

When binary caching is enabled, after each package is built from source, vcpkg creates a **binary
package**. Binary packages hold the build-output of a package: binaries, build system integration files,
usage documentation, license, and other files. If a later run requires a cached package to be installed,
vcpkg determines whether to restore the cached binary package or trigger a build from source.

This way, binary caching reduces the impact of the following downsides of building from source:

* **Duplicated effort:** By reducing the number of times a package needs to be built from source.
* **Long build times:** Restoring a binary package is usually a very fast operation that takes
  seconds to complete.

Binary caching is especially effective in CI scenarios where ephemeral containers or build agents
force vcpkg to work with a clean-slate each time. By using a cloud-based binary cache (such as
[GitHub Packages](<https://docs.github.com/packages>) or [Azure DevOps
Artifacts](/azure/devops/artifacts/start-using-azure-artifacts?view=azure-devops&tabs=nuget&preserve-view=true)) you can persist binary packages between runs to ensure maximum speed since rebuilds only happen
when you make changes to your dependencies or configuration.

> [!TIP]
> It is recommended to create a binary cache with read and write permissions for every continuous
> integration pipeline or workflow. Individual developers should have read-only access to the
> CI-produced binary cache.

Binary caches can be hosted in a variety of environments. The most basic form of a binary cache is a
folder on the local machine or a network file share. Caches can also be stored in any NuGet feed
(such as GitHub Packages or Azure DevOps Artifacts), Azure Blob Storage, Google Cloud Storage, and
many other services.

If your CI provider offers a native "caching" function, it is recommended to use both vcpkg binary
caching and the native method for the most performant results.

## Reusing a binary cache for development

While not recommended as a binary distribution mechanism, binary caching can be used to reuse build
output from multiple systems. For example, developers can use the binary packages produced by a CI run on
their local machines. For other methods to reuse and integrate vcpkg-produced binaries, look at
[`vcpkg export`](../commands/export.md).

For example, you can configure a [NuGet feed hosted on Azure
Artifacts](../consume/binary-caching-nuget.md) to accept binary packages built
from your CI pipeline and reuse those in a development environment.

You will need to configure your Azure Artifacts feed to [grant read and write
permissions to your Azure DevOps
pipeline](/azure/devops/artifacts/feeds/feed-permissions) as well as granting
read-only permissions for hosts in your development team.

## Default binary cache

Binary caching is enabled by default with a [`files` provider](../reference/binarycaching.md#files) at the first valid location of:

- **Windows**

1. `%VCPKG_DEFAULT_BINARY_CACHE%`
1. `%LOCALAPPDATA%\vcpkg\archives`
1. `%APPDATA%\vcpkg\archives`

- **Non-Windows**

1. `$VCPKG_DEFAULT_BINARY_CACHE`
1. `$XDG_CACHE_HOME/vcpkg/archives`
1. `$HOME/.cache/vcpkg/archives`

Reduced in-tool help is available via `vcpkg help binarycaching`.

Binary Caching only covers binaries you build. To cache source files and
prebuilt tools, see [Asset Caching](assetcaching.md).

## Next steps

Read the tutorials to learn how to set up a binary cache and the [configuration
syntax reference](../reference/binarycaching.md):

* [Set up a local binary cache](../consume/binary-caching-local.md)
* [Set up a binary cache using a NuGet feed](../consume/binary-caching-nuget.md)
* [Set up a binary cache with GitHub Actions
  cache](../consume/binary-caching-github-actions-cache.md)
* [Set up a binary cache with GitHub
  Packages](../consume/binary-caching-github-packages.md)
