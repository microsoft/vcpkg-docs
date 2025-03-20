---
title: Asset caching with vcpkg
description: Use asset caching with vcpkg to mirror your downloaded artifacts, reduce external network reliance and improve build reliability.
author: vicroms
ms.author: viromer
ms.topic: concept-article
ms.date: 03/20/2025
---
# Asset caching with vcpkg

[!INCLUDE [experimental](../../includes/experimental.md)]

A fundamental operation of vcpkg is downloading files from the Internet, such as source code archives, tool executables,
and source code patches. These files, referred to as **assets**, are obtained through simple file transfers over a network.
Each asset has a known SHA512 hash, to check for integrity, before the download begins.

Asset caching enables you to set up mirror locations, known as caches, to retrieve these assets. If enabled, vcpkg first
tries to fetch assets from known caches. If the asset is unavailable, vcpkg falls back to the original download source.
Additionally, vcpkg can save the asset to the cache for future use.

The primary goal of asset caching is to minimize reliance on external networks, offering several advantages:

* **Reduced download times and bandwidth usage**: By retrieving assets from a local or nearby cache, builds complete
  faster and consume less network bandwidth.
* **Support for air-gapped or offline environments**: Assets can be mirrored in trusted locations, enabling builds in
  environments without internet access.
* **Improved reliability and continuity**: Ensures access to critical assets even if third-party hosts modify or remove
  them, reducing the risk of disruptions.

## Configuration

Asset caching is configured via:

* The `X_VCPKG_ASSET_SOURCES` environment variable, or
* The `--x-asset-sources` command-line option.

In both cases, the expected value is a semicolon-delimited list of sources. Each source has a specific syntax depending
on its storage backend. See the [asset caching reference documentation](../users/assetcaching.md) to learn how to
configure asset caching sources.

### Example using Azure Blob Storage

```PowerShell
vcpkg install zlib --x-asset-sources="clear;x-azurl,mystorageaccount.blob.core.windows.net,${SAS_TOKEN},readwrite;x-azurl,file:///Z:/vcpkg-cache/assets,,readwrite"
```

## Next step

Here are some tasks to try next:

* [Set up an asset cache](../consume/asset-caching.md)
* [Use a custom script as an asset caching source](../examples/asset-caching-source-nuget.md)
