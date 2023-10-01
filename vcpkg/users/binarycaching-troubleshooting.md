---
title: Binary caching troubleshooting guide
description: Troubleshooting your binary caching setup
author: dan-shaw
ms.author: danshaw2
ms.date: 9/20/2023
ms.prod: vcpkg
---

# Binary caching troubleshooting guide

This guide is intended for vcpkg users that are running into issues with [binary caching](./binarycaching.md).

## General

## One or more {vendor} credential providers requested manual action. Add the binary source 'interactive' to allow interactivity.

See the [binary caching syntax](./binarycaching.md#configuration-syntax) for how to add a new binary source.

## Pushing NuGet/NuGet config to {url} failed. Use --debug for more information.
Try the following:
- Ensure [debug output](#debug-output) is enabled to see detailed error messages.
- If you are using a personal access token or API key, ensure it has correct scope for writing to a feed and hasn't expired. There may be specific scopes you need to enable for private or public feeds.
- Refer to the troubleshooting suggestions for general cloud storage providers

Success means you see in your output:
```
Pushing zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg to '<NuGet Feed>'...
Your package was pushed.
Stored binaries in 1 destinations in 1.8 s.
```

Your feed also contains the expected NuGet package:
```
Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a---           8/16/2023  8:53 PM         529044 zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg
```

## Failures and issues when uploading/pushing to a binary caching provider

Each cloud storage provider has different authentication methods and error messages, so we recommend referring to the troubleshooting guide or documentation of your specific provider.

In addition, try the following:
- Ensure [debug output](#debug-output) is enabled to see detailed error messages.
- Make sure you've authenticated to your cloud storage provider; some providers may required you to run a tool.
- Check you have enough disk space on your machine to create an archived package when uploading. Some libraries use up significant disk space even when compressed.
- Ensure your binary cache provider has write permissions: some providers allow you to have a read only lock on a cloud resource.

## Binary cache is empty
Try the following:
- Ensure [debug output](#debug-output) is enabled to see detailed error messages.
- Check if there are any push or upload message failures [link]
- Make sure your vcpkg has `write` permissions for your binary source. For example, for `x-gcs,gs://<bucket-name>/,readwrite` vcpkg has both read and write permissions.

## Not restoring cached packages from the cloud binary cache and instead consuming from the local cache
Try the following:
- Ensure [debug output](#debug-output) is enabled to see detailed error messages.
- Make sure your vcpkg has `read` permissions for your binary source. For example, for `x-gcs,gs://<bucket-name>/,readwrite` vcpkg has both read and write permissions.
- [Compare ABI hashes]() between the packages in your cloud storage provider and the packages built locally. You may have a CI pipeline building those packages. If you have a different environment locally than the environment that cached packages were produced in, vcpkg will build a different set of packages. To workaround this issue, either use the same versions of the tools as you are using in your CI pipeline to create the packages, or simply wait for the packages to rebuild. 

> [!NOTE]
> We generally do not recommend local developers pushing or uploading packages to a cloud storage provider as this is a supply chain security risk.

## Libraries are rebuilding a majority of the runs / Libraries rebuild unexpectedly

Try the following:
- Ensure [debug output](#debug-output) is enabled to see detailed error messages.
- Ensure the binary cache is not empty. Refer to troubleshooting steps otherwise [link]
- Ensure for some runs you are actually consuming from the binary cache [cloud][link] ???
- [Compare ABI hashes]() between a successful run (where it is fetching from the cache) and an unsuccessful run (where it is not)

## Debugging Tips

## <a name="debug-output"></a>Set up vcpkg to print debugging information
Debug mode outputs information such as the compiler verison and ABI hashes, which will help us to diagnose any binary caching issues.

* [Classic mode](./classic-mode.md): add `--debug` to your command invocation.
* CMake toolchain: set `-DVCPKG_INSTALL_OPTIONS="--debug"` in your `cmake` command call or in your `CMakePresets.json` file.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to `--debug`

## Troubleshoot why two packages have different ABI hashes

The full Application Binary Interface (ABI) hash of the binary package is in the debug output [link]. For zlib:

```
[DEBUG] Trying to hash <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt
[DEBUG] <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt has hash bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87
```

The ABI hash `bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87` for package zlib is constructed by hashing all the possible relevant information [link] (sources, compiler information) to distinguish binary packages. The binary cache provider uses this as a key to refer to the package artifact. If this hash were to change between runs for the same package, it would mean some part of the package had changed.

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
> The `triplet_abi` entry has three hashes: the hash of the file content of the `x86-windows` triplet, the `windows.cmake` toolchain, and the compiler hash. These hashes would change if you decide to target a different platform.

### vcpkg CMake helper functions changes between packages

Internal vcpkg helper functions like vcpkg_from_github, vcpkg_copy_pdbs, vcpkg_fixup_pkgconfig, etc. are used in the ABI calculation. If you are getting new ABI hashes for these ports, this is probably also caused by pulling the latest vcpkg repo. This issue will be addressed in a future update when these functions will be separately versioned.

Try the following:
- Pin the vcpkg catalog to a specific commit. If you are cloning vcpkg, run `git checkout [commit_id]` or add vcpkg as a submodule in your project. Because the ABI hash is calculated from both scripts and ports in the vcpkg repository, pulling the latest changes will likely cause frequent rebuilds.

> [!NOTE]
> Setting the `builtin-baseline` is not sufficient. 

### Files used to build your library (`portfile.cmake`, `*.patch`, or `vcpkg.json`) change between runs

This usually means the baseline of your libraries is not fixed. Frequently, the problem occurs when you clone the latest repository from vcpkg, which will pull in the updated baseline of libraries.

Try the following:
- Pin the vcpkg catalog to a specific commit. If you are cloning vcpkg, run `git checkout [commit_id]` or add vcpkg as a submodule in your project. Because the ABI hash is calculated from both scripts and ports in the vcpkg repository, pulling the latest changes will likely cause frequent rebuilds.
- Set `builtin-baseline` in your [vcpkg manifest](../reference/vcpkg-json.md#builtin-baseline).

### Compiler version changes between runs / Tool versions (CMake/PowerShell) changes between runs

Try the following:
- Turn off automatic compiler updates. For example, major, minor and patch updates to the MSVC compiler can cause your library to be built with two different versions between runs.
- If using a hosted image, choose a stable and versioned image. Updates to your underlying image may change your C/C++ compiler or CMake version, causing a rebuild.

If the above options do not work, consider the following workarounds:
- Use `vcpkg export` to package your libraries instead
- Consider using a Docker self-hosted image to build your libraries
- Have an auxiliary continuous integration run that builds vcpkg libraries on a regular cadence (e.g. daily or weekly)

## Issue isn't listed here

If you're issue isn't listed here, see our [repo](https://github.com/microsoft/vcpkg/issues) to create a new issue.
