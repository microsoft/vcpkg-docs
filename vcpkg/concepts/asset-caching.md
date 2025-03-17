---
title: Asset caching with vcpkg
description: Use asset caching with vcpkg to mirror your asset's downloads to improve build reliability.
author: vicroms
ms.author: viromer
ms.topic: concept-article
ms.date: 10/03/2024
---
# Asset caching with vcpkg

[!INCLUDE [experimental](../../includes/experimental.md)]

A fundamental operation of vcpkg is downloading files from the Internet, things like: source code archives, tool executables,
source code patches, etc. We call anything that is downloaded from the network an **asset**.

Asset caching allows you to configure alternative download locations (caches) to acquire assets. When enabled, vcpkg first
attempst to acquire assets from any configured cache location, and falls back to the original download if no cached asset
is found. Optionally, vcpkg stores the asset in the cache for future use.

This feature helps mitigate these common development concerns:

* Reducing dependency from external networks. Possibly, also reducing download times.
* Mirroring assets in trusted locations for air-gapped environments.
* Maintaining continuity of business if assets hosted by third-parties are changed or unavailable.

## Configuration

Asset caching is configured via:

* The `X_VCPKG_ASSET_SOURCES` environment variable, or
* The `--x-asset-sources` command-line option.

The expected value is a semicolon-delimited list of asset caching sources. Read the [asset caching reference
documentation](../users/assetcaching.md) to learn the configuration syntax.

Example using Azure Blob Storage as an asset cache:

```PowerShell
vcpkg install zlib --x-asset-sources="x-azurl,mystorageaccount.blob.core.windows.net,${env:SAS_TOKEN},readwrite"
```

## Next step

Here are some tasks to try next:

* [Set up an asset cache](../consume/asset-caching.md)
* [Use a custom script as an asset caching source](../examples/asset-caching-source-nuget.md)
