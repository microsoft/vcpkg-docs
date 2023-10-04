---
title: Troubleshoot binary caching issues
description: Troubleshooting guide for common binary caching issues
author: data-queue
ms.author: danshaw2
ms.date: 9/20/2023
ms.prod: vcpkg
ms.topic: troubleshooting-general
---

# Binary caching troubleshooting guide

This guide is intended for vcpkg users that are running into issues with [binary caching](./binarycaching.md).

## <a name="debug-output"></a>Set up vcpkg to print debugging information
In debug mode, vcpkg outputs additional information, such as the compiler version and ABI hashes, which makes diagnosing binary caching issues easier.

* [Classic mode](./classic-mode.md): add `--debug` to your command invocation.
* CMake toolchain: set `-DVCPKG_INSTALL_OPTIONS="--debug"` in your `cmake` command call or in your `CMakePresets.json` file.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to `--debug`

## Manual intervention required for {vendor} credential providers

You might see the error message:
```
One or more {vendor} credential providers requested manual action. Add the binary source 'interactive' to allow interactivity.
```

Refer to the [binary caching syntax](./binarycaching.md#configuration-syntax) to introduce a new binary source.

## <a name="push-failure"></a> NuGet push to {url} failed
If you are using a NuGet binary source, the error message is:
```
Pushing NuGet to {url} failed. Use --debug for more information.
```

If you are using a NuGet config binary source, the error message is:
```
Pushing NuGet config to {url} failed. Use --debug for more information.
```

Try the following:
- Enable [debug output](#debug-output) for comprehensive error logs
- For personal access tokens or API keys, ensure the correct scope for feed writing and verify it's not expired. Private or public feeds might require particular scopes.
- Review [troubleshooting guidelines](#storage-providers) for cloud storage providers

If successful, you should see the following message:
```
Pushing zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg to '<NuGet Feed>'...
Your package was pushed.
Stored binaries in 1 destinations in 1.8 s.
```

Your feed contains the expected NuGet package:
```
Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a---           8/16/2023  8:53 PM         529044 zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg
```

## <a name="storage-providers"></a> Errors during cache uploads

Each cloud storage provider has different authentication methods and error messages, so we recommend referring to the troubleshooting guide or documentation of your specific provider.

Try the following:
- Enable [debug output](#debug-output) for comprehensive error logs
- Ensure you've authenticated to your cloud storage provider. Some providers require you to run a tool.
- Ensure your machine has enough disk space for creating archived packages when uploading. Some libraries use up significant disk space even when compressed.
- Confirm write permissions for your binary cache provider. Make sure you don't have a read only lock on the cache.

## <a name="empty-cache"></a> Binary cache is empty
Try the following:
- Enable [debug output](#debug-output) for comprehensive error logs
- Refer to troubleshooting guidelines for [cloud providers](#storage-providers) and [NuGet](#push-failure) if there are any push/upload failures
- Ensure your [binary cache configuration](binarycaching.md#configuration-syntax) is set to `write` or `readwrite`

## <a name="cloud-cache"></a> Using local cache instead of cloud binary cache
Try the following:
- Enable [debug output](#debug-output) for comprehensive error logs
- Ensure your [binary cache configuration](binarycaching.md#configuration-syntax) is set to `read` or `readwrite`
- [Compare ABI hashes](#compare-abi) between the packages built locally and packages in your binary cache. If you have have a pipeline building those packages, different environments in your image and local machine will produce packages with different ABI hashes. To workaround this issue, either use the same versions of the tools as you are using in your CI pipeline to create the packages, or simply use the packages created locally. 

> [!NOTE]
> We typically discourage developers pushing or uploading local packages to a cloud storage provider due to supply chain security concerns.

## Frequent or unexpected library rebuilds

Try the following:
- Enable [debug output](#debug-output) for comprehensive error logs
- Refer to [troubleshooting guidelines](#empty-cache) if your binary cache is empty
- Ensure you are using the correct binary cache. Refer to [troubleshooting guidelines](#cloud-cache) if you are using the local cache instead of the cloud binary cache.
- [Compare ABI hashes](#compare-abi) of packages between a successful run (where it is fetching from the cache) and an unsuccessful run (where it is not)

## <a name="compare-abi"></a>Troubleshooting ABI hash mismatch

Enable [debug output](#debug-output) to identify the full Application Binary Interface (ABI) hash of a pacakge. For zlib:

```
[DEBUG] Trying to hash <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt
[DEBUG] <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt has hash bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87
```

The [ABI hash](./binarycaching.md#abi-hash) `bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87` for package zlib is constructed by hashing all the possible relevant information to distinguish binary packages. The binary cache provider uses this as a key to refer to the package artifact. If this hash were to change between runs for the same package, it would mean some part of the package had changed.

First check the version of your compiler to see if it has changed between runs. In the debug output:
```
[DEBUG] -- The C compiler identification is MSVC 19.36.32538.0
[DEBUG] -- The CXX compiler identification is MSVC 19.36.32538.0
[DEBUG] #COMPILER_HASH#f5d02a6542664cfbd4a38db478133cbb1a18f315
```

After that, check for differences in the hashes of `abientries`. Relevant files are hashed to compute the final ABI hash:
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

### vcpkg CMake helper functions changes between packages

Internal vcpkg helper functions like vcpkg_from_github, vcpkg_copy_pdbs, vcpkg_fixup_pkgconfig, etc. are a part of computing the final ABI calculation. If you are getting new ABI hashes for these helper functions, it is likely because you are pulling the latest vcpkg changes.

Try the following:
- Pin the vcpkg catalog to a specific commit. If you are cloning vcpkg, run `git checkout [commit_id]` or add vcpkg as a submodule in your project. Since the ABI hash is calculated from both scripts and ports in the vcpkg repository, pulling the latest changes will likely cause frequent rebuilds.

### Files used to build your library (`portfile.cmake`, `*.patch`, or `vcpkg.json`) change between runs

This indicates the baseline of your libraries is not fixed and that you are fetching `HEAD` from the vcpkg repository.

Try the following:
- Pin the vcpkg catalog to a specific commit. If you are cloning vcpkg, run `git checkout [commit_id]` or add vcpkg as a submodule in your project. Because the ABI hash is calculated from both scripts and ports in the vcpkg repository, pulling the latest changes will likely cause frequent rebuilds.
- Set `builtin-baseline` in your [vcpkg manifest](../reference/vcpkg-json.md#builtin-baseline).

### Compiler version changes between runs

Try the following:
- Turn off automatic compiler updates. For example, major, minor and patch updates to the MSVC compiler can cause your library to be built with two different versions between runs.
- If using a hosted image, choose a stable and versioned image. Updates to your underlying image may change your C/C++ compiler version, causing a rebuild.

> [!NOTE]
> The expected behavior of vcpkg is to rebuild libraries if the compiler version changes.

If the above options do not work, consider the following workarounds:
- Use [`vcpkg export`](../commands/export.md) to produce a standalone archive of your dependencies instead of restoring them from a manifest.
- Consider using a Docker self-hosted image to build your libraries
- Have an auxiliary continuous integration run that builds vcpkg libraries on a regular cadence (e.g. daily or weekly)

### Tool versions (CMake/PowerShell) changes between runs

Try the following:
- Add `--x-abi-tools-use-exact-versions` to your vcpkg invocation. This fixes the ABI of your tools based on the version in `vcpkgTools.xml`; vcpkg fetches its own copy if necessary.
- If using a hosted image, choose a stable and versioned image. Updates to your underlying image may change the version of your tools, causing a rebuild.

## Issue isn't listed here

If you're issue isn't listed here, see our [repo](https://github.com/microsoft/vcpkg/issues) to create a new issue.
