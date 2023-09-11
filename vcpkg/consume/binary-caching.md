---
title: Reduce package build times with binary caching
description: Learn to set up a binary cache to reduce rebuild times.
author: vicroms
ms.author: viromer
ms.date: 8/13/2023
ms.prod: vcpkg
---
# Reduce package build times with binary caching

Most ports in the [vcpkg public registry](<https://github.com/Microsoft/vcpkg>) are built from
source. Building from source ensures maximum compatibility by using the same environment, build
tools, and configurations you use for your project in your dependencies. However, this  can also
result in duplicated builds when multiple projects, developers, or machines require the same
dependencies.

Setting up a binary cache allows vcpkg to save the compiled files of a package in a shared location
and reuse it when asked to build the same package with the same configuration, saving time on
rebuilds. We call the output produced by each package build a "binary package".

Binary caches can be hosted in a variety of environments, the most basic form of binary caching is a
directory containing zipped archives of a package's build output. More advanced scenarios include
NuGet package feeds hosted in [GitHub Packages](<https://docs.github.com/en/packages>), [Azure
DevOps Artifacts](<https://learn.microsoft.com/azure/devops/artifacts/start-using-azure-artifacts?view=azure-devops&tabs=nuget>),
or other such services.

Binary caching is especially effective in scenarios where dependencies and configuration
don't change often, such as in CI (Continuous Integration) or build servers. Binary packages are particularly susceptible to
configuration changes (compiler version, compilation flags, installed tools, vcpkg-specific script changes,
etc) which makes sharing them between developer machines difficult. See the [`vcpkg
export`](../commands/export.md) for a different option to share built packages.

See the [Binary Caching documentation](../users/binarycaching.md) to learn about the different [storage backends](../users/binarycaching.md#providers).

## Set up a local binary cache

### Default local binary cache

By default, vcpkg creates a local binary cache. This helps save time when installing the same
package on the same machine.

The location of the default binary cache depends on your operating system.

### [Windows](#tab/default-windows)

By default, binary caching is enabled at the first valid location among the following:

* The location specified by the `VCPKG_DEFAULT_BINARY_CACHE` environment variable
* `%LOCALAPPDATA%\vcpkg\archives`
* `%APPDATA%\vcpkg\archives`

### [Unix](#tab/unix)

By default, binary caching is enabled at the first valid location among the following:

* The location specified by the `VCPKG_DEFAULT_BINARY_CACHE` environment variable
* `$XDG_CACHE_HOME/vcpkg/archives`
* `$HOME/.cache/vcpkg/archives`

---

Use the `VCPKG_DEFAULT_BINARY_CACHE` environment variable to change the default binary cache
location.

The default binary cache uses a [`files` provider](../users/binarycaching.md#files-provider). This
type of binary cache provider stores binary packages as compressed archives in a filesystem
directory.

### Disabling the default binary cache

To disable the default binary cache, set the [`VCPKG_BINARY_SOURCES` environment variable](../users/binarycaching.md#configuration-syntax) to `clear`.

### [Windows](#tab/disable-windows)

```PowerShell
PS D:\vcpkg> $env:VCPKG_BINARY_SOURCES="clear"
```

### [Unix](#tab/disable-unix)

```bash
$ VCPKG_BINARY_SOURCES=clear
```

---

### Set up local binary cache locations

You are not limited to the default binary cache location as your only local binary cache, it is
possible to configure multiple filesystem locations as binary caches. You can even set up specific
caches with different permissions, such as read-only, write-only, or read and write.

All binary caches are configured through the `VCPKG_BINARY_SOURCES` environment variable. The value
of `VCPKG_BINARY_SOURCES` follows a  specific [configuration
syntax](../users/binarycaching.md#configuration-syntax).


### Example: configuring multiple binary cache locations

In this example we'll set up two binary cache locations:

* `\\remote\shared\vcpkg-cache` which is a network location, and
* `D:\vcpkg-cache\archives`

We want the remote location to be read-only, meaning we will use this location to restore packages
but won't upload any locally-built binary packages to it. The second location will have both read
and write permissions and we will use as a backup binary cache.

Let's set the value of `VCPKG_BINARY_SOURCES` as follows `clear;files,\\remote\shared\vcpkg-cache,read;files,D:\vcpkg-cache\archive`.

Breaking down this configuration:

* `clear`, removes any previously configured binary cache. In this case, this disables the default
  binary cache.
* `files,\\remote\shared\vcpkg-cache,read`: configures a read-only `files` binary cache provider using the
  network-location `\\remote\shared\vcpkg-cache` the last section (`read`) sets this binary cache
  provider as read-only.
* `files,D:\vcpkg-cache\archive`: configures a second binary cache provider location, it uses the
  same `files` backend but uses a local path instead (`D:\vcpkg-cache\archives`). By default, cache
  locations have `readwrite` permissions, so we don't need to specify permissions explicitly.
  
## Next step

> [!div class="nextstepaction"]
> [Set up a NuGet-backed binary cache](binary-caching-nuget.md)
