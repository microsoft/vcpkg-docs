---
title: "Tutorial: Set up a vcpkg asset cache"
description: Learn to set up a local asset cache to mirror download assets.
author: vicroms
ms.author: viromer
ms.date: 10/24/2023
ms.prod: vcpkg
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to set up a local asset cache to mirror downloaded artifacts
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---
# Tutorial: Set up a vcpkg asset cache

All asset caches are configured through the `X_VCPKG_ASSET_SOURCES` environment variable. The value
of `X_VCPKG_ASSET_SOURCES` follows a  specific [configuration
syntax](../users/assetcaching.md).

In this tutorial you'll learn how to:

> [!div class="checklist"]
> * [Create an asset cache directory](#1---create-an-asset-cache-directory)
> * [Configure `X_VCPKG_ASSET_SOURCES`](#2---configure-x_vcpkg_asset_sources)

## Prerequisites

* A terminal
* vcpkg

## 1 - Create an asset cache directory

When using the [`x-azurl`](../users/assetcaching.md#x-azurl) backend, you can
use an Azure DevOps Blob Storage to serve as the storage backend for your
downloaded assets. Alternatively, you can use a filesystem directories to store
your download assets, including network locations. To do so, you need to use the
`file://` protocol in your asset cache URL.

In this tutorial, you'll create a local asset cache location to store downloaded
artifacts. If you have an Azure DevOps Blob Storage you want to use instead,
replace the URL and provide a SAS token as the second parameter in the
configuration string.

1. Create a directory to serve as an asset cache location (substitute with any locations of your choosing):

::: zone pivot="shell-powershell, shell-cmd"

```console
mkdir D:\vcpkg\asset-cache
```

::: zone-end
::: zone pivot="shell-bash"

```bash
mkdir /home/vcpkg/asset-cache
```

::: zone-end

## 2 - Configure `X_VCPKG_ASSET_SOURCES`

Next set the value of `X_VCPKG_ASSET_SOURCES` as follows:

::: zone pivot="shell-powershell"

```PowerShell
$env:X_VCPKG_ASSET_SOURCES="clear;x-azurl,file://D:/vcpkg/asset-cache,,readwrite"
```

::: zone-end
::: zone pivot="shell-cmd"

```console
set X_VCPKG_ASSET_SOURCES="clear;x-azurl,file://D:/vcpkg/asset-cache,,readwrite"
```

::: zone-end

::: zone pivot="shell-cmd, shell-powershell"
This `X_VCPKG_ASSET_SOURCES` configuration adds the following source strings:

* `clear`, disables any previously configured asset cache
* `x-azurl,file://C:/vcpkg/asset-cache,,readwrite`, sets a filesystem asset cache, located in
  `D:\vcpkg\asset-cache`, with read-write permissions.
::: zone-end

::: zone pivot="shell-bash"

```bash
export X_VCPKG_ASSET_SOURCES="clear;x-azurl,file:///home/vcpkg/asset-cache,,readwrite"
```

This `X_VCPKG_ASSET_SOURCES` configuration adds the following source strings:

* `clear`, disables any previously configured asset cache
* `x-azurl,file:///home/vcpkg/asset-cache,,readwrite`, sets a filesystem asset cache, located in
  `home/vcpkg/asset-cache`, with read-write permissions.

::: zone-end

## Next steps

Here are other tasks to try next:

* [Use a custom script as an asset caching source](../examples/asset-caching-source-nuget.md)
* [Set up a binary cache to save time on rebuilds](../consume/binary-caching-local.md)
