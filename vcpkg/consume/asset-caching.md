---
title: "Tutorial: Set up a vcpkg asset cache"
description: Learn to set up a local asset cache to mirror download assets using local storage and the x-azurl provider.
author: vicroms
ms.author: viromer
ms.date: 03/20/2025
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to set up a local asset cache to mirror downloaded artifacts
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---
# Tutorial: Set up a vcpkg asset cache

All asset caches are configured through the `X_VCPKG_ASSET_SOURCES` environment variable. The value
of `X_VCPKG_ASSET_SOURCES` follows a specific [configuration syntax](../users/assetcaching.md).

> [!div class="checklist"]
> * [Create an asset cache directory](#1---create-an-asset-cache-directory)
> * [Configure `X_VCPKG_ASSET_SOURCES`](#2---configure-x_vcpkg_asset_sources)

## Prerequisites

* A terminal
* vcpkg

## 1 - Create an asset cache directory

vcpkg offers the following asset cache providers:

* `x-azurl`: which is intended to work with Azure Blob Storage containers, but works just as well with other service providers
  and even local or network filesystems.
* `x-script`: which lets you add your own business logic to handle asset caching through a customizable script or executable.

This tutorial uses, `x-azurl` as the provider to store assets in a local directory in your filesystem using a URL with
the `file://` protocol.

> [!NOTE]
> You can use these same steps to create an asset cache with Azure Blob Storage. Replace any URL in the tutorial with the
> URL for your storage and provide a SAS Token for authentication.

1. Create a directory to serve as an asset cache location (substitute with any locations of your choosing):

::: zone pivot="shell-powershell, shell-cmd"

```console
mkdir Z:\vcpkg\asset-cache
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
$env:X_VCPKG_ASSET_SOURCES="clear;x-azurl,file:///Z:/vcpkg/asset-cache,,readwrite"
```

[!INCLUDE [env-vars](../../includes/env-vars.md)]

::: zone-end
::: zone pivot="shell-cmd"

```console
set "X_VCPKG_ASSET_SOURCES=clear;x-azurl,file:///Z:/vcpkg/asset-cache,,readwrite"
```

[!INCLUDE [env-vars](../../includes/env-vars.md)]

::: zone-end

::: zone pivot="shell-cmd, shell-powershell"
This `X_VCPKG_ASSET_SOURCES` configuration adds the following source strings:

* `clear`, disables any previously configured asset cache
* `x-azurl,file:///Z:/vcpkg/asset-cache,,readwrite`, sets a filesystem asset cache, located in
  `Z:\vcpkg\asset-cache`, with read-write permissions.
::: zone-end

::: zone pivot="shell-bash"

```bash
export X_VCPKG_ASSET_SOURCES="clear;x-azurl,file:///home/vcpkg/asset-cache,,readwrite"
```

[!INCLUDE [env-vars](../../includes/env-vars-bash.md)]

This `X_VCPKG_ASSET_SOURCES` configuration adds the following source strings:

* `clear`, disables any previously configured asset cache
* `x-azurl,file:///home/vcpkg/asset-cache,,readwrite`, sets a filesystem asset cache, located in
  `home/vcpkg/asset-cache`, with read-write permissions.

::: zone-end

## Next steps

Here are other tasks to try next:

* [Use a custom script as an asset caching source](../examples/asset-caching-source-nuget.md)
* [Set up a binary cache to save time on rebuilds](../consume/binary-caching-local.md)
