---
title: Binary Caching Troubleshooting Guide
description: Troubleshooting your binary caching setup
ms.date: 8/16/2023
---

# Binary Caching Troubleshooting

This guide assumes a basic understanding of binary caching. To set up binary caching instead, see [here](./users/binarycaching.md).

## Print debug output

Set up vcpkg to print debugging information:

* Classic Mode: add `--debug` to your command invocation.
* CMake Toolchain: set `-DVCPKG_INSTALL_OPTIONS="--debug"` in your CMake call.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to `--debug`

## Configuring your Binary Cache

Consider the case of using a NuGet feed as a binary cache.

### Binary Package ABI

The full ABI of the binary package is in the debug output. For example, for zlib:

```
[DEBUG] Trying to hash <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt
[DEBUG] <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt has hash bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87
```

The package zlib has ABI: `bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87`. This ABI will be used to reference the package in the cache. You can also use this information between binary caching runs to diagnose issues.

### Creating the binary package

This happens when somehow `vcpkg` has failed to package the dependencies, perhaps due to a disk space error. In these cases, we recommend reading the debug output for any tool specific errors.

A successful creation of the binary package should output:

```
[DEBUG] 1010: CreateProcessW("<path>\downloads\tools\nuget-6.2.1-windows\nuget.exe" pack -ForceEnglishOutput -Verbosity detailed -NonInteractive "<path>\buildtrees\zlib\x86-windows.nuspec" -OutputDirectory "<path>\buildtrees" -NoDefaultExcludes)
[DEBUG] 1010: cmd_execute_and_stream_data() returned 0 after  1038094 us
...
Successfully created package '<path>\buildtrees\zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg'.
```
If creating the package is failing, consider the following:

* Make sure `CreateProcessW` returns a successful exit code `0` when packing
* Make sure you have enough disk space on your machine

### Failing to write to the binary cache

A successful write to the binary cache should have the following output for each dependency:
```
[DEBUG] 1011: CreateProcessW("<path>\downloads\tools\nuget-6.2.1-windows\nuget.exe" push -ForceEnglishOutput -Verbosity detailed -NonInteractive "<path>\buildtrees\zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg" -Timeout 100 -Source <NuGet Feed>)
[DEBUG] 1011: cmd_execute_and_stream_data() returned 0 after   767027 us
NuGet Version: 6.2.1.2
Pushing zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg to '<NuGet Feed>'...
Your package was pushed.
Stored binaries in 1 destinations in 1.8 s.
```

Your NuGet feed or cache should look something like this:
```
Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a---           8/16/2023  8:53 PM         529044 zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg
```

If writing to the cache is failing, consider the following:

* Make sure your binary source has `write` permissions
* Make sure you are correctly authenticating for your provider. This may mean your authentication token has correct permissions or for cloud environments, you may need to run a tool to authenticate
* Make sure `CreateProcessW` returns a successful exit code when pushing

### Failing to read from the binary cache

A successful reuse of a stored binary package should output:
```
[DEBUG] 1008: CreateProcessW("<path>\downloads\tools\nuget-6.2.1-windows\nuget.exe" install -ForceEnglishOutput -Verbosity detailed -NonInteractive "<path>\buildtrees\packages.config" -OutputDirectory "<path>\packages" -ExcludeVersion -PreRelease -PackageSaveMode nupkg -DirectDownload -NoCache -Source <NuGet Feed>)
[DEBUG] 1008: cmd_execute_and_stream_data() returned 0 after  1168243 us
...
Restored 1 package(s) from NuGet in 1.2 s. Use --debug to see more details.
```

If the ABI of your package is the same between runs, consider:

* Make sure your binary source has `read` permissions
* Make sure you are correctly authenticating for your provider. This may mean your authentication token has correct permissions or for cloud environments, you may need to run a tool to authenticate

If the ABI of your package differs between runs, see the next section.

## Fixing your Binary Cache setup

This section assumes you have correctly configured binary caching: both writes and reads work. However, vcpkg is not always using cached binaries and perhaps is rebuilding your libraries almost every time. Or perhaps there are ocassionally unexpected rebuilds, even though you haven't changed your manifest. In this section, we will set up debug mode for vcpkg to get more information to debug our binary caching issue and look at common problems and recommended solutions.

If your vcpkg binary cache is constantly rebuilding, it's important to figure out what in your [ABI](./users/binarycaching.md#abi-hash) changed. Suppose we are building `zlib` for `x86-windows`. What information in the debug output is relevant?

Scrolling through the debug output, the version of your compiler is part of the ABI.
```
[DEBUG] -- The C compiler identification is MSVC 19.36.32538.0
[DEBUG] -- The CXX compiler identification is MSVC 19.36.32538.0
[DEBUG] #COMPILER_HASH#f5d02a6542664cfbd4a38db478133cbb1a18f315
```

After that, we will see a list of `abientries`, hashes of relevant files that are combined to form the final ABI hash:

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

You might notice the `triplet_abi` entry has three hashes: the hash of the file content of the `x86-windows` triplet, the `windows.cmake` toolchain, and the compiler hash. Theses hashes would change if you decide to target a different platform.

### The hashes of the files used to build your library (`portfile.cmake`, `*.patch`, or `vcpkg.json`) change between runs

This usually means the baseline of your libraries is not fixed. Frequently, the problem occurs when you clone the latest repository from vcpkg, which will pull in the updated baseline of libraries.
Possible solutions:

* (recommended) The copy of the vcpkg catalog must be pinned to a specific commit. When you run `git clone vcpkg`, make sure you run `git checkout [commit_id]` or add vcpkg as a submodule in your project. The reason is the ABI of a library is calculated based on the hashes of scripts and ports in the vcpkg repository. Pulling the latest changes will likely cause frequent rebuilds.
* Set `builtin-baseline` in your [vcpkg manifest](./users/manifests.md).

### The hash of the vcpkg internal CMake functions (vcpkg_from_github, vcpkg_copy_pdbs, vcpkg_fixup_pkgconfig, etc) changes between runs

You are always pulling the latest internal CMake functions, which are used in the ABI calculation. This is probably also caused by cloning the latest vcpkg repo. We expect this issue to go away in the future when we version these functions.
Possible solutions:

* See the previous section on how to pin the vcpkg catalog to a specific commit.

### The version of CMake, Powershell, compiler version changes between runs

Somehow, your environment changes between runs. To verify that this indeed is the issue, temporarily disable including the compiler hash in your ABI by setting `VCPKG_DISABLE_COMPILER_TRACKING` to `ON` in your custom triplet. If the issue persists, fix tool versions with `--x-abi-tools-use-exact-versions`. Note it is not recommended to use these options in development or a production environment, as they can cause very hard to diagnose errors.

In these cases, vcpkg is behaving correctly, we expect the ABI hashes to change if your toolchain changes. Rebuilds can be expensive, however, so we have recommendations for the following environments:

* CI: We’ve seen this as an issue if you are using CI (such as GitHub Hosted Runners) and using an image that may update your C++/C compiler or CMake version from underneath you. To minimize this, choose a versioned stable image if possible.
* Visual Studio C++ Compiler Updates: Major, minor and even patch updates to the compiler will cause your library ABI to change.  To minimize this, stop automatic Visual Studio updates.

If the above options do not work, consider the following solutions:

* Use `vcpkg export` to package your libraries instead (currently, only works in classic mode)
* Consider using a Docker image to build your libraries
* Have an auxiliary CI run that builds vcpkg libraries daily or weekly
