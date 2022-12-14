---
title: vcpkg export
description: Command line reference for the vcpkg export command. Create standalone SDK bundles.
ms.date: 11/30/2022
---
# vcpkg export

The `export` command creates a shrinkwrapped archive containing a specific set of libraries (and their dependencies) that can be quickly and reliably shared with build servers or other users in your organization. `export` only supports classic mode at this time.

- `--nuget`: NuGet package
- `--zip`: Zip archive
- `--7zip`: 7Zip archive
- `--raw`: Raw, uncompressed folder

Each of these have the same internal layout which mimics the layout of a full vcpkg instance:

- `installed/` contains the library files
- `scripts/buildsystems/vcpkg.cmake` is the [CMake toolchain file](../users/buildsystems/cmake-integration.md)
- `scripts/buildsystems/msbuild/vcpkg.props` and `scripts/buildsystems/msbuild/vcpkg.targets` are the [MSBuild integration files](../users/buildsystems/msbuild-integration.md)

NuGet package exports will also contain a `build\native\vcpkg.targets` that integrates with MSBuild projects using the NuGet package manager.

Please also see our [blog post](https://blogs.msdn.microsoft.com/vcblog/2017/05/03/vcpkg-introducing-export-command/) for additional examples.
