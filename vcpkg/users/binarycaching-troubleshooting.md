---
title: Binary caching troubleshooting guide
description: Troubleshooting your binary caching setup
author: dan-shaw
ms.author: danshaw2
ms.date: 9/20/2023
ms.prod: vcpkg
---

# Binary caching troubleshooting guide

This guide is intended for vcpkg binary caching users that are running into unexpected behavior from this experience. Please review the [binary caching](./binarycaching.md) reference documentation for a basic understanding of what this feature does and how it works.

## Set up vcpkg to print debugging information

Debug mode outputs information such as the compiler verison and ABI hashes, which will help us to diagnose any binary caching issues.

* [Classic mode](./classic-mode.md): add `--debug` to your command invocation.
* CMake toolchain: set `-DVCPKG_INSTALL_OPTIONS="--debug"` in your `cmake` command call or in your `CMakePresets.json` file.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to `--debug`

## Troubleshooting binary caching configuration issues

There are many choices for a binary caching provider including AWS, NuGet, Azure. While each provider will have its own error messages, we expect the type of error and the actual fix will be the same across all providers. We will walk through a specific example of using NuGet as a binary cache provider.

### Binary package ABI

The full Application Binary Interface (ABI) of the binary package is in the debug output. For example, for zlib:

```
[DEBUG] Trying to hash <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt
[DEBUG] <path>\buildtrees\zlib\x86-windows.vcpkg_abi_info.txt has hash bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87
```

The ABI hash `bb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87` for package zlib is constructed by hashing all the possible relevant information (sources, compiler information) to distinguish binary packages. The binary cache provider uses this as a key to refer to the binary package artifact. If this hash were to change between runs for the same package, it would mean some part of the package had changed.

### Creating the binary package

A successful creation of the binary package should output:
```
[DEBUG] 1010: CreateProcessW("<path>\downloads\tools\nuget-6.2.1-windows\nuget.exe" pack -ForceEnglishOutput -Verbosity detailed -NonInteractive "<path>\buildtrees\zlib\x86-windows.nuspec" -OutputDirectory "<path>\buildtrees" -NoDefaultExcludes)
[DEBUG] 1010: cmd_execute_and_stream_data() returned 0 after  1038094 us
...
Successfully created package '<path>\buildtrees\zlib_x86-windows.1.2.13-vcpkgbb1c96759ac96102b4b18215db138daedd3eb16c2cd3302ae7bffab2b643eb87.nupkg'.
```
If you are not seeing this message, consider the following:

* Make sure `CreateProcessW` returns a successful exit code `0` when compressing the package into an artifact
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
* Make sure you are correctly authenticating to your provider. This may mean your authentication token has correct permissions, or, for cloud environments, you may need to run a tool to authenticate
* Make sure `CreateProcessW` returns a successful exit code when pushing

### Failing to read from the binary cache

A successful reuse of a stored binary package should output:
```
[DEBUG] 1008: CreateProcessW("<path>\downloads\tools\nuget-6.2.1-windows\nuget.exe" install -ForceEnglishOutput -Verbosity detailed -NonInteractive "<path>\buildtrees\packages.config" -OutputDirectory "<path>\packages" -ExcludeVersion -PreRelease -PackageSaveMode nupkg -DirectDownload -NoCache -Source <NuGet Feed>)
[DEBUG] 1008: cmd_execute_and_stream_data() returned 0 after  1168243 us
...
Restored 1 package(s) from NuGet in 1.2 s. Use --debug to see more details.
```

If the ABI of your package is the same between runs, make sure:

* Your binary source has `read` permissions
* You are correctly authenticating to your provider. This may mean your authentication token has correct permissions or for cloud environments, you may need to run a tool to authenticate

If the ABI hash of your package differs between runs, see the next section.

## Fixing your binary cache setup

If you have correctly configured binary caching for both reads and writes but vcpkg is sometimes or always rebuilding your libraries instead of using existing cached binaries, read on.

If your vcpkg binary cache is constantly rebuilding, it's important to figure out what changed in your [ABI hash](./binarycaching.md#abi-hash). Suppose we are building `zlib` for `x86-windows`. What information in the debug output is relevant?

The version of your compiler is part of the ABI hash. You can find it in the debug output.
```
[DEBUG] -- The C compiler identification is MSVC 19.36.32538.0
[DEBUG] -- The CXX compiler identification is MSVC 19.36.32538.0
[DEBUG] #COMPILER_HASH#f5d02a6542664cfbd4a38db478133cbb1a18f315
```

After that, we will see a list of `abientries`. These are hashes of relevant files that are combined to form the final ABI hash:

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
* Set `builtin-baseline` in your [vcpkg manifest](../reference/vcpkg-json.md#builtin-baseline).

### ABI hash of vcpkg CMake helper functions changes between runs

Internal vcpkg helper functions like vcpkg_from_github, vcpkg_copy_pdbs, vcpkg_fixup_pkgconfig, etc. are used in the ABI calculation. If you are getting new ABI hashes for these ports, this is probably also caused by pulling the latest vcpkg repo. This issue will be addressed in a future update when these functions will be separately versioned.

Possible solutions:

* (recommended) The copy of the vcpkg catalog must be pinned to a specific commit. When you run `git clone vcpkg`, make sure you run `git checkout [commit_id]` or add vcpkg as a submodule in your project. The reason is the ABI of a library is calculated based on the hashes of scripts and ports in the vcpkg repository. Pulling the latest changes will likely cause frequent rebuilds.

### Version of CMake, PowerShell, or your compiler changes between runs

In these cases, vcpkg is behaving correctly. It is expected for toolchain changes to cause ABI hash changes as well. Rebuilds can be expensive, so we suggest the following:

* Continuous integration systems: Some systems such as GitHub Hosted Runners update the underlying image periodically. Updates may change your C/C++ compiler or CMake version, which will cause a rebuild. If possible, choose a versioned stable image.
* MSVC compiler updates: Major, minor and patch updates to the MSVC compiler will cause your library ABI hash to change. To minimize this, turn off automatic Visual Studio updates.

If the above options do not work, consider the following workarounds:

* Use `vcpkg export` to package your libraries instead
* Consider using a Docker image to build your libraries
* Have an auxiliary continuous integration run that builds vcpkg libraries on a regular cadence (e.g. daily or weekly)
