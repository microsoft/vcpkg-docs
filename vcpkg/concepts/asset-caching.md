---
title: Asset caching with vcpkg
description: Use asset caching with vcpkg to mirror your asset's downloads to improve build reliability.
author: vicroms
ms.author: viromer
ms.topic: concept-article
ms.date: 01/10/2024
---
# Asset caching with vcpkg

[!INCLUDE [experimental](../../includes/experimental.md)]

Asset caching is a feature that allows vcpkg to use alternative download
endpoints to acquire a package's assets. When asset caching is enabled vcpkg
attempts to download assets (such as source code and build tools) from a
configured mirror location. If the asset is not available in the mirror location, vcpkg will download it from its original source and then store a copy in the mirror for future use.

Asset caching can help with these common development scenarios:

* Improving reliability of continuous integration runs.
* Mirroring download assets in trusted locations for air-gapped environments.
* Maintaining continuity of business if third party sources are changed or become unavailable.

## Configuration

Asset caching is configured via:

* The `X_VCPKG_ASSET_SOURCES` environment variable, or
* The `--x-asset-sources` command-line option.

In both cases, the expected value is a semicolon-delimited list of sources. Each
source has a specific syntax depending on its storage backend (see the [asset
caching reference documentation](../users/assetcaching.md) to learn how to
configure asset caching sources).

## Next step

Here are some tasks to try next:

* [Set up an asset cache](../consume/asset-caching.md)
* [Use a custom script as an asset caching source for
  NuGet](../examples/asset-caching-source-nuget.md)
