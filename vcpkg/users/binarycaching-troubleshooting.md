---
title: Troubleshoot binary caching issues
description: Troubleshooting guide for common binary caching issues
author: data-queue
ms.author: danshaw2
ms.date: 01/10/2024
ms.topic: troubleshooting-general
---

# Binary caching troubleshooting guide

This guide is intended for users experiencing issues with [binary caching](binarycaching.md).

## <a name="debug-output"></a> Enable vcpkg debugging information
It is highly recommended that you enable debug output when following this guide.

* [Classic mode](../concepts/classic-mode.md): add `--debug` to your command invocation.
* CMake toolchain: add `-DVCPKG_INSTALL_OPTIONS="--debug"` in your CMake
  configure call or in your `CMakePresets.json` file.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to `--debug`.
* Set the `VCPKG_INSTALL_OPTIONS` environment variable to `--debug`.

## <a name="push-failure"></a> NuGet push to {url} fails 

[!INCLUDE [debug](../../includes/debug.md)]

When using a NuGet binary source the following error appears:

```
Pushing NuGet to {url} failed. Use --debug for more information.
```

When using a NuGet config file binary source the following error appears:

```
Pushing NuGet config to {url} failed. Use --debug for more information.
```

This error arises when vcpkg attempts and fails to use the NuGet command line to upload packages to a NuGet feed.

### Cause 1: Insufficient user write permissions
<!-- 
Steps to reproduce:
1. Create a NuGet Feed
2. Set your role to a Reader and remove any write permissions you have
3. `vcpkg` install with variables set `$env:VCPKG_BINARY_SOURCES="clear;nuget,<feed url>,readwrite"`
-->

You encounter the following error message:

```
System.Net.Http.HttpRequestException: Response status code does not indicate success: 403 (Forbidden - User <user> lacks permission to complete this action. You need to have 'AddPackage'.
```

Push was rejected by remote source because the user does not have sufficient write permissions.

* Confirm that your user or user group has write permissions. In NuGet, the user must be at least a [Contributor role](/azure/devops/artifacts/feeds/feed-permissions#permissions-table) to the feed.

### Cause 2: Misconfigured NuGet Feed url
<!--
Steps to reproduce:
1. Push a binary package to an existing NuGet feed with an incorrect URI.
2. Set $env:VCPKG_BINARY_SOURCES="clear;nuget,<incorrect url>,readwrite"
-->
You might see the error:

```
System.Net.Http.HttpRequestException: Response status code does not indicate success: 405 (Method Not Allowed).
```

The server rejected NuGet's push request because it did not recognize the request method.

* Verify that the URI in your binary source is correct and that it directs to the service index of the feed, typically `<feed base url>/nuget/v3/index.json`.

### Additional NuGet Resources
Consult the [NuGet documentation](/azure/devops/artifacts/nuget/publish) for guidelines on connecting to and publishing on a NuGet feed.

## <a name="cache-upload"></a> Cache upload errors

[!INCLUDE [debug](../../includes/debug.md)]

You encounter errors uploading a binary package to your cache.

### Cause 1: Binary cache provider failed to upload

Uploads can fail for a variety of reasons, and error messages are usually provider specific.
* Ensure you are authenticated to your cache.  Different providers authenticate differently.
* Check if you've specified the correct URI for your cache.
* Refer to [push troubleshooting](#push-failure) if you are using NuGet as a binary source.
* Review documentation or the troubleshooting guide of your specific provider.

## <a name="empty-cache"></a> Empty binary cache

[!INCLUDE [debug](../../includes/debug.md)]

Although you encountered no errors and vcpkg installation was successful, the binary cache remains empty. If you observed errors, consult [push troubleshooting for NuGet](#push-failure) and [upload troubleshooting for other providers](#cache-upload).

### Cause 1: vcpkg lacks write permissions to the binary cache
<!--
Steps to reproduce:
1. Create a NuGet feed
2. $env:VCPKG_BINARY_SOURCES="clear;nuget,C:/nugetfeed,read"
3. Run ./vcpkg install rapidjson
-->
The following message is missing in your output.

```
Uploading binaries for 'rapidjson:x64-windows' to <binary source> source <url>.
Stored binaries in 1 destinations in 1.5 s.
```

vcpkg skipped uploading the binary package to your binary cache.

* Ensure your [binary cache configuration](../reference/binarycaching.md#configuration-syntax) is set to `write` or `readwrite`

## Libraries rebuild instead of using remote binary cache

[!INCLUDE [debug](../../includes/debug.md)]

Your system rebuilds libraries locally, even when the required binary package is available in the remote binary cache.

The following message is missing in your output.

```
Restored 1 package(s) from <remote binary cache> in 1.1 s. Use --debug to see more details.
```

### Cause 1: vcpkg lacks read permissions from the remote binary cache

vcpkg choose to read your default binary cache over the remote one.

* Ensure your [binary cache configuration](../reference/binarycaching.md#configuration-syntax) is set to `read` or `readwrite`

### Cause 2: The remote binary cache is empty

The remote cache should contain a list of binary packages you've pushed.

* Refer to the [empty binary cache](#empty-cache) section.

### Cause 3: Differences between local and remote build environments

Each package in binary cache is labeled with an [ABI hash](../reference/binarycaching.md#abi-hash) which contains compiler versions, sources, and other information to distinguish between binary packages. If the locally computed ABI hash doesn't match the one stored remotely, the package isn't retrieved.

* Consult the [ABI Hash mismatch troubleshooting guide](#abi-mismatch) to determine the root cause.

## Unexpected or frequent library rebuilds

[!INCLUDE [debug](../../includes/debug.md)]

In an unchanged environment and without updating vcpkg, you still find yourself rebuilding libraries occassionally.

### Cause 1: Undetected changes in the build environment

Each package in binary cache is labeled with an [ABI hash](../reference/binarycaching.md#abi-hash) which contains compiler versions, sources, and other information to distinguish between binary packages. If the locally computed ABI hash doesn't match the one stored remotely, the package isn't retrieved.

* Consult the [ABI Hash mismatch troubleshooting guide](#abi-mismatch) to determine the root cause.

## <a name="abi-mismatch"></a> Troubleshooting ABI hash mismatch

[!INCLUDE [debug](../../includes/debug.md)]

This guide is intended for users to diagnose why they have different ABI hashes for two identically named binary packages.

### Comparing two binary packages

Determining the difference between two identically named packages requires comparing various data: sources, tool versions, compilers, and target platforms. The [ABI hash](../reference/binarycaching.md#abi-hash) provides a concise representation of this data. When calculating an ABI hash, vcpkg considers all relevant data, including file content, tool versions, and system details. It creates a hash for each data point and then combines these hashes into a single value for the binary package.

#### Binary package ABI hash comparison

The [ABI hash](../reference/binarycaching.md#abi-hash) of the library zlib is `bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87`:

```
[DEBUG] Trying to hash <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt
[DEBUG] <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt has hash bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87
```

If the hash changes between runs for the same library, this indicates that the two packages are distinct.

#### Compiler version ABI hash comparison

Verify if the version of your compiler changed between runs.

```
[DEBUG] -- The C compiler identification is MSVC 19.36.32538.0
[DEBUG] -- The CXX compiler identification is MSVC 19.36.32538.0
[DEBUG] #COMPILER_HASH#f5d02a6542664cfbd4a38db478133cbb1a18f315
```

The compiler hash is `f5d02a6542664cfbd4a38db478133cbb1a18f315`.

#### ABI Hash entry comparison

Compare ABI entries for each package. A entry represents a piece of information that contributes to the final hash.

```
[DEBUG] <abientries for zlib:x86-windows>
[DEBUG]   0001-Prevent-invalid-inclusions-when-HAVE_-is-set-to-0.patch|750b9542cb55e6328cca01d3ca997f1373b9530afa95e04213168676936e7bfa
[DEBUG]   0002-skip-building-examples.patch|835ddecfed752e0f49be9b0f8ff7ba76541cb0a150044327316e22ca84f8d0c2
[DEBUG]   0003-build-static-or-shared-not-both.patch|d6026271dcb3d8fc74b41e235620ae31576a798e77aa411c3af8cd9e948c02b1
[DEBUG]   0004-android-and-mingw-fixes.patch|37a43eddbcb1b7dde49e7659ae895dfd0ff1df66666c1371ba7d5bfc49d8b438
[DEBUG]   cmake|3.26.2
[DEBUG]   features|core
[DEBUG]   portfile.cmake|ac63047b644fa758860dd7ba48ff9a13b058c6f240b8e8d675b8fbba035976be
[DEBUG]   ports.cmake|5a8e00cedff0c898b1f90f7d129329d0288801bc9056562b039698caf31ff3f3
[DEBUG]   post_build_checks|2
[DEBUG]   powershell|7.3.6
[DEBUG]   triplet|x86-windows
[DEBUG]   triplet_abi|3e71dd1d4afa622894ae367adbbb1ecbd42c57c51428a86b675fa1c8cad3a581-36b818778ba6f2c16962495caedb9a7b221d5be4c60de1cd3060f549319a9931-f5d02a6542664cfbd4a38db478133cbb1a18f315
[DEBUG]   usage|be22662327df993eebc437495add75acb365ab18d37c7e5de735d4ea4f5d3083
[DEBUG]   vcpkg-cmake|1b3dac4b9b0bcbef227c954b495174863feebe3900b2a6bdef0cd1cf04ca1213
[DEBUG]   vcpkg-cmake-wrapper.cmake|5d49ef2ee6448479c2aad0e5f732e2676eaba0411860f9bebabe6002d66f57d1
[DEBUG]   vcpkg.json|bc94e2540efabe36130a806381a001c57194e7de67454ab7ff1e30aa15e6ce23
[DEBUG]   vcpkg_copy_pdbs|d57e4f196c82dc562a9968c6155073094513c31e2de475694143d3aa47954b1c
[DEBUG]   vcpkg_fixup_pkgconfig|588d833ff057d3ca99c14616c7ecfb5948b5e2a9e4fc02517dceb8b803473457
[DEBUG]   vcpkg_from_git|8f27bff0d01c6d15a3e691758df52bfbb0b1b929da45c4ebba02ef76b54b1881
[DEBUG]   vcpkg_from_github|b743742296a114ea1b18ae99672e02f142c4eb2bef7f57d36c038bedbfb0502f
[DEBUG]   vcpkg_replace_string|d43c8699ce27e25d47367c970d1c546f6bc36b6df8fb0be0c3986eb5830bd4f1
[DEBUG] </abientries>
```

> [!NOTE]
> The `triplet_abi` entry contains three hashes: the hash of the file content of the `x86-windows` triplet, the `windows.cmake` toolchain, and the compiler hash. These hashes would change if you decided to target a different platform.

### Mismatch 1: Port files

Port files include port scripts (`portfile.cmake`, `vcpkg.json`), patch files (`*.patch`), or any other file in the ports directory: `ports/<library>/*`.

#### Cause 1: CI or pipeline updated the port catalog

Before vcpkg ran in your CI, it cloned the latest vcpkg repository.

* When using `git clone https://github.com/microsoft/vcpkg` followed by the `bootstrap` script, ensure you checkout to a specific commit.
* Consider adding vcpkg as a git submodule to your project.

#### Cause 2: GitHub Actions updated vcpkg

You are using the system copy of vcpkg provided by GitHub Actions, which was updated.

* Clone your own copy of vcpkg.
* Consider making vcpkg a git submodule in your project.

### Mismatch 2: vcpkg CMake helper functions

CMake helper functions reside in the scripts directory: `scripts/*`, and typically start with `vcpkg_`.

#### Cause 1: CI or pipeline updated helper scripts

Before vcpkg ran in your CI, it cloned the latest vcpkg repository.

* When using `git clone https://github.com/microsoft/vcpkg` followed by the `bootstrap` script, ensure you checkout to a specific commit.
* Consider adding vcpkg as a git submodule to your project.

#### Cause 2: GitHub Actions updated vcpkg

You are using the system copy of vcpkg provided by GitHub Actions, which was updated.

* Clone your own copy of vcpkg.
* Consider making vcpkg a git submodule in your project.

### Mismatch 3: Compiler version

vcpkg rebuilt your dependencies with a different version of the compiler.

#### Cause 1: The Visual Studio C++ compiler automatically updated.

Visual Studio automatically updated the C++ workload, including the compiler, between runs. Even minor version updates will result in vcpkg rebuilding the set of libraries.

* [Disable automatic compiler updates](/visualstudio/install/update-visual-studio#always-update-on-close-and-other-download-behaviors).

#### Cause 2: The library was built on a different machine than the machine used to consume it.

One machine created and published the binary package to a remote cache. Another machine typically used for development consumed the cached library. 

* Use the same C++ compiler version locally as on your remote machine. For Visual Studio, consider a [fixed version bootstrapper](/visualstudio/releases/2022/release-history#fixed-version-bootstrappers).
* Rebuild your dependencies locally for development purposes. Test and address issues later during continuous integration.

#### Cause 3: The self-hosted image updated the compiler.

The underlying image you used to build vcpkg dependencies changed, which updated the compiler version.

* Pin to a stable and versioned image. Ensure you are not fetching the latest image so it does not automatically update underlying tools or compilers between runs.
* If you need to update the image frequently, pin the C++ compiler tools to a specific version when creating your image.

#### Cause 4: GitHub Hosted Runners updated the underlying compiler.

Hosted GitHub runners update compilers and tools weekly.

* Currently, there is no way to fix your image and prevent the tools and compiler version from updating periodically. See the [other options](#other-options) section for alternative solutions.

### Mismatch 4: The version of the tools changed between runs.

The version of tools used to build your libraries, CMake or PowerShell, changed between runs.

#### Cause 1: Visual Studio automatically updated.

Visual Studio automatically updated, including any tools, between runs. Even minor version updates will result in vcpkg rebuilding the set of libraries.

* [Disable automatic Visual Studio updates](/visualstudio/install/update-visual-studio#always-update-on-close-and-other-download-behaviors).
* Add `--x-abi-tools-use-exact-versions` to your vcpkg invocation. This fixes the ABI of your tools based on the version in `vcpkgTools.xml`; vcpkg fetches its own copy if necessary.

#### Cause 2: The library was built on a different machine than the machine used to consume it.

One machine created and published the binary package to a remote cache. Another machine typically used for development consumed the cached library. 

* Use the same tool versions locally as on your remote machine.
* Rebuild your dependencies locally for development purposes. Test and address issues later during continuous integration.
* Add `--x-abi-tools-use-exact-versions` to your vcpkg invocation. This fixes the ABI of your tools based on the version in `vcpkgTools.xml`; vcpkg fetches its own copy if necessary.

#### Cause 3: Self-hosted image updated the tools.

The underlying image you used to build vcpkg dependencies changed, which the versions of any tools used.

* Pin to a stable and versioned image. Ensure you are not fetching the latest image so it does not automatically update underlying tools between runs.
* If you need to update the image frequently, pin any relevant tools to a specific version when creating your image.
* Add `--x-abi-tools-use-exact-versions` to your vcpkg invocation. This fixes the ABI of your tools based on the version in `vcpkgTools.xml`; vcpkg fetches its own copy if necessary.

#### Cause 4: GitHub Hosted Runners updated underlying tools.

Hosted GitHub runners update compilers and tools weekly.

* Add `--x-abi-tools-use-exact-versions` to your vcpkg invocation. This fixes the ABI of your tools based on the version in `vcpkgTools.xml`; vcpkg fetches its own copy if necessary.

### <a name="other-options"></a> Other Options

If the above options do not work, consider the following workarounds:

* Use [`vcpkg export`](../commands/export.md) to produce a standalone archive of your dependencies instead of restoring them from a manifest.
* Consider using a Docker self-hosted image to build your libraries
* Have an auxiliary continuous integration run that builds vcpkg libraries on a regular cadence (e.g. daily or weekly)

## Issue isn't listed here

If your issue isn't listed here, visit [our repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
