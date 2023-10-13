---
title: Troubleshoot binary caching issues
description: Troubleshooting guide for common binary caching issues
author: data-queue
ms.author: danshaw2
ms.date: 10/11/2023
ms.prod: vcpkg
ms.topic: troubleshooting-general
---

# Binary caching troubleshooting guide

This guide is intended for users that are running into issues with [binary caching](./binarycaching.md).

## <a name="debug-output"></a>Set up vcpkg to print debugging information
In debug mode, vcpkg outputs additional information, such as the compiler version, which makes diagnosing binary caching issues easier.

* [Classic mode](./classic-mode.md): add `--debug` to your command invocation.
* CMake toolchain: add `-DVCPKG_INSTALL_OPTIONS="--debug"` in your CMake configure call or in your `CMakePresets.json` file.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to `--debug`.

## <a name="push-failure"></a> NuGet push to {url} fails 
<!-- should be an include -->
> [!IMPORTANT]
> Update your vcpkg tool to the latest release. Additionally, enable [debug output](#debug-output) for comprehensive error logs.

If you are using a NuGet binary source, the complete error message is:
```
Pushing NuGet to {url} failed. Use --debug for more information.
```

If you are using a NuGet config binary source, the complete error message is:
```
Pushing NuGet config to {url} failed. Use --debug for more information.
```

An error occurred while vcpkg was invoking the NuGet command line to push NuGet packages to a feed. 

### Cause 1: The user lacks write permissions.
<!-- 
Steps to reproduce:
1. Create a NuGet Feed
2. Set your role to a Reader and remove any write permissions you have
3. `vcpkg` install with variables set `$env:VCPKG_BINARY_SOURCES="clear;nuget,<feed url>,readwrite"`
-->

You see the error:
```
System.Net.Http.HttpRequestException: Response status code does not indicate success: 403 (Forbidden - User <user> lacks permission to complete this action. You need to have 'AddPackage'.
```

Push was rejected by remote source because the user does not have sufficient write permissions. 
* Ensure the user or your user group has write permissions. In NuGet, the user must have at least a [Contributor role](https://learn.microsoft.com/en-us/azure/devops/artifacts/feeds/feed-permissions?view=azure-devops#permissions-table) to the feed.

### Cause 2: The URL to the NuGet feed is incorrect.
<!--
Steps to reproduce:
1. Push a binary package to an existing NuGet feed with an incorrect URI.
2. Set $env:VCPKG_BINARY_SOURCES="clear;nuget,<incorrect url>,readwrite"
-->
You may see the error:
```
System.Net.Http.HttpRequestException: Response status code does not indicate success: 405 (Method Not Allowed).
```

The server rejected NuGet's push request because it did not recognize the request method.
* Check if you have specified the correct URI in your binary source. The URI should point to the service index of the feed, usually `<feed>/nuget/v3/index.json`.

### Additional resources for diagnosing NuGet issues
Review [NuGet documentation](https://learn.microsoft.com/en-us/azure/devops/artifacts/nuget/publish?view=azure-devops) for connecting and publishing to a NuGet feed.

## <a name="cache-upload"></a> Errors occur during cache uploads

You encounter errors when uploading a binary package to your cache.

Uploads can fail for a variety of reasons.
* Ensure you are authenticated to your cache.  Different providers authenticate differently.
* Check if you've specified the correct URI for your cache.
* Refer to [push troubleshooting](#push-failure) if you are using NuGet as a binary source.
* Review documentation or the troubleshooting guide of your specific provider.

## <a name="empty-cache"></a> Binary cache is empty

You didn't see any errors, vcpkg installation succeeded, but the binary cache is still empty. If you did see errors, refer to [push troubleshooting](#push-failure) for NuGet and [upload troubleshooting](#cache-upload) for other providers.

### Cause 1: Vcpkg doesn't have permissions to write to the binary cache.
<!--
Steps to reproduce:
1. Create a NuGet feed
2. $env:VCPKG_BINARY_SOURCES="clear;nuget,C:/nugetfeed,read"
3. Run ./vcpkg install rapidjson
-->
You are missing the following message in your output.
```
Uploading binaries for 'rapidjson:x64-windows' to <binary source> source <url>.
Stored binaries in 1 destinations in 1.5 s.
```

Vcpkg skipped uploading the binary package to your binary cache.
* Ensure your [binary cache configuration](binarycaching.md#configuration-syntax) is set to `write` or `readwrite`

## Libraries rebuild instead of consuming from the remote binary cache

You are rebuilding libraries locally even though the remote binary cache contains the binary package you need.

There is the absence of the following output:
```
Restored 1 package(s) from <remote binary cache> in 1.1 s. Use --debug to see more details.
```

### Cause 1: Vcpkg doesn't have permissions to read from the remote binary cache.

Vcpkg choose to read your default binary cache over the remote one.
* Ensure your [binary cache configuration](binarycaching.md#configuration-syntax) is set to `read` or `readwrite`

### Cause 2: The remote binary cache is empty.

The remote cache should contain a list of binary packages you've pushed. 
* Refer to the [empty binary cache](#empty-cache) section.

### Cause 3: The remote package was built in a different environment than your local setup.

Each package in binary cache is labeled with an [ABI hash](./binarycaching.md#abi-hash) which contains compiler versions, sources, and other information to distinguish between binary packages. The ABI hash calculated locally was unable to retrieve the package stored remotely.
* Review the [ABI Hash mismatch troubleshooting guide](#abi-mismatch) to determine the root cause.

## Libraries are rebuilding more frequently or sporadically than expected.

You are building libraries in the same environment as you did before. You did not update vcpkg, but occassionally you need to rebuild libraries again.

### Cause 1: Something in the build environment changed without you knowing about it.

Each package in binary cache is labeled with an [ABI hash](./binarycaching.md#abi-hash) which contains compiler versions, sources, and other information to distinguish between binary packages. The ABI hash calculated locally was unable to retrieve the package stored remotely.
* Review the [ABI Hash mismatch troubleshooting guide](#abi-mismatch) to determine the root cause.

## <a name="abi-mismatch"></a> Troubleshooting ABI hash mismatch

This guide is intended for users to diagnose why they have different ABI hashes for two identically named binary packages.

### Comparing two binary packages

To determine how two identically named packages are different, you will need to compare a tremendous amount of information, sources, tool versions, compilers, target platforms. The [ABI hash](./binarycaching.md#abi-hash) serves as a convinient way to represent that in a single hash. To calculate the ABI hash of a package, vcpkg lists every piece of relevant information including file content, tool versions, system information of how the package is built. Each entry in that list is hashed. The final hash for a binary package is constructed by taking that list of hashes and combining it to form a single hash.

#### Comparing the binary package ABI hashes

The [ABI hash](./binarycaching.md#abi-hash) of the library zlib is `bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87`:
```
[DEBUG] Trying to hash <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt
[DEBUG] <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt has hash bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87
```

If this hash were to change between runs for the same package, it would mean you have two different packages, each with a different key when stored in a binary cache. Compare the ABI hashes between the two packages: it should be different.

#### Comparing the ABI hash for the compiler version

Check if the version of your compiler changed between runs.
```
[DEBUG] -- The C compiler identification is MSVC 19.36.32538.0
[DEBUG] -- The CXX compiler identification is MSVC 19.36.32538.0
[DEBUG] #COMPILER_HASH#f5d02a6542664cfbd4a38db478133cbb1a18f315
```
The compiler hash is `f5d02a6542664cfbd4a38db478133cbb1a18f315`.

#### Comparing the ABI hash entries

Check for differences in each ABI entry between the two packages. A entry represents a piece of information needed to compute the final hash.
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

#### Cause 1: CI or pipeline pulled the latest port catalog before building.
Before running vcpkg in your CI, there was a step that cloned the latest vcpkg repository.
* If your setup involves `git clone https://github.com/microsoft/vcpkg` and then running the `bootstrap` script, ensure you checkout your vcpkg repository to a specific commit.
* Consider adding vcpkg as a git submodule to your project.

#### Cause 2: GitHub Actions updated vcpkg as a system dependency.
You are using the system copy of vcpkg, which GitHub Actions updated.
* Clone your own copy of vcpkg.
* Consider adding vcpkg as a git submodule to your project.

### Mismatch 2: vcpkg CMake helper functions
CMake helper functions include any file that live in the scripts directory: `scripts/*`, typically prefixed by `vcpkg_`.

#### Cause 1: CI or pipeline pulled the latest helper script changes before building.
Before running vcpkg in your CI, there was a step that cloned the latest vcpkg repository.
* If your setup involves `git clone https://github.com/microsoft/vcpkg` and then running the `bootstrap` script, ensure you checkout your vcpkg repository to a specific commit.
* Consider adding vcpkg as a git submodule to your project.

#### Cause 2: GitHub Actions updated vcpkg as a system dependency.
You are using the system copy of vcpkg, which GitHub Actions updated.
* Clone your own copy of vcpkg and checkout to a specific commit.
* Consider adding vcpkg as a git submodule to your project.

### Mismatch 3: Compiler version
vcpkg rebuilt your dependencies with a different version of the compiler.

#### Cause 1: The Visual Studio C++ compiler automatically updated.
Visual Studio automatically updated the C++ workload, including the compiler, between runs. Even minor version updates will result in vcpkg rebuilding the set of libraries.
* [Disable automatic compiler updates](https://learn.microsoft.com/en-us/visualstudio/install/update-visual-studio?view=vs-2022#always-update-on-close-and-other-download-behaviors).

#### Cause 2: The library was built on a different machine than the machine used to consume it.
One machine created and published the binary package to a remote cache. Another machine typically used for development consumed the cached library. 
* Install a C++ compiler locally with the same version as your remote machine's. For Visual Studio, you want to use a [fixed version bootstrapper](https://learn.microsoft.com/en-us/visualstudio/releases/2022/release-history#fixed-version-bootstrappers).
* Rebuild your dependencies locally for development purposes. Test and fix issues later during continuous integration.

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
* [Disable automatic Visual Studio updates](https://learn.microsoft.com/en-us/visualstudio/install/update-visual-studio?view=vs-2022#always-update-on-close-and-other-download-behaviors).
* Add `--x-abi-tools-use-exact-versions` to your vcpkg invocation. This fixes the ABI of your tools based on the version in `vcpkgTools.xml`; vcpkg fetches its own copy if necessary.

#### Cause 2: The library was built on a different machine than the machine used to consume it.
One machine created and published the binary package to a remote cache. Another machine typically used for development consumed the cached library. 
* Install a tools locally with the same version as your remote machine's.
* Rebuild your dependencies locally for development purposes. Test and fix issues later during continuous integration.
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
- Use [`vcpkg export`](../commands/export.md) to produce a standalone archive of your dependencies instead of restoring them from a manifest.
- Consider using a Docker self-hosted image to build your libraries
- Have an auxiliary continuous integration run that builds vcpkg libraries on a regular cadence (e.g. daily or weekly)

## Issue isn't listed here

If your issue isn't listed here, visit [our repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
