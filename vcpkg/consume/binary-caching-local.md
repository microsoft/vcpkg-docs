---
title: "Tutorial: Set up a vcpkg binary cache using filesystem directories"
description: Learn to set up a local binary cache to reduce rebuild times.
author: vicroms
ms.author: viromer
ms.date: 9/25/2023
ms.prod: vcpkg
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to set up a local binary cache so that I save time on package rebuilds
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
---
# Tutorial: Set up a vcpkg binary cache using filesystem directories

You are not limited to the [default binary cache](binary-caching-default.md) location as your only
local binary cache, it is possible to configure multiple filesystem locations as binary caches. You
can even set up specific caches with different permissions, such as read-only, write-only, or read
and write permissions.

All binary caches are configured through the `VCPKG_BINARY_SOURCES` environment variable. The value
of `VCPKG_BINARY_SOURCES` follows a  specific [configuration
syntax](../users/binarycaching.md#configuration-syntax).

In this tutorial you'll learn how to:

> [!div class="checklist"]
> * [Create a binary cache directory](#1---create-a-binary-cache-directory)
> * [Configure `VCPKG_BINARY_SOURCES`](#2---configure-vcpkg_binary_sources)

## Prerequisites

* A terminal
* vcpkg

## 1 - Create a binary cache directory

When using the [`files`](../users/binarycaching.md#files) backend, you can use filesystem
directories to store your binary packages, including network locations.

::: zone pivot="os-windows"

```PowerShell
mkdir D:\vcpkg\binary-cache
mkdir \\remote\shared\vcpkg\binary-cache
```

::: zone-end

::: zone pivot="os-windows"

```bash
mkdir /home/vcpkg/binary-cache
mkdir /mnt/remote/shared/vcpkg/binary-cache
```

::: zone-end

Substitute these locations with any locations of your choosing.

## 2 - Configure `VCPKG_BINARY_SOURCES`

Next set the value of `VCPKG_BINARY_SOURCES` as follows:

::: zone pivot="os-windows"

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear;files,\\remote\shared\vcpkg\binary-cache,read;files,D:\vcpkg\binary-cache"
```

This `VCPKG_BINARY_SOURCES` configuration adds the following source strings:

* `clear`, disables any previously configured binary cache, in this case, it disables the [default
  binary cache](binary-caching-local.md).
* `files,\\remote\shared\vcpkg\binary-cache,read`, sets a binary cache using the filesystem backend,
  `files`, located in `\\remote\shared\vcpkg\binary-cache`, and gives it read-only permissions
  (`read`).
* `files,D:\vcpkg\binary-cache`, sets a second filesystem binary cache, located in
  `D:\vcpkg\binary-cache`, with read-write permissions (`readwrite` is omitted since it is the
  default permission).

::: zone-end

::: zone pivot="os-linux, os-macos"

```bash
export VCPKG_BINARY_SOURCES="clear;files,/mnt/remote/shared/vcpkg/binary-cache,read;files,/home/vcpkg/binary-cache"
```

This `VCPKG_BINARY_SOURCES` configuration adds the following source strings:

* `clear`, disables any previously configured binary cache, in this case, it disables the [default
  binary cache](binary-caching-local.md).
* `files,/mnt/remote/shared/vcpkg/binary-cache,read`, sets a binary cache using the filesystem backend,
  `files`, located in `/mnt/remote/shared/vcpkg/binary-cache`, and gives it read-only permissions
  (`read`).
* `files,/home/vcpkg/binary-cache`, sets a second filesystem binary cache, located in
  `/home/vcpkg/binary-cache`, with read-write permissions (`readwrite` is omitted since it is the
  default permission).

::: zone-end

## Next steps

> [!div class="nextstepaction"]
> [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)

Here are other tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Packages](binary-caching-github-packages.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Actions Cache](binary-caching-github-actions-cache.md)
