---
title: vcpkg integrate
description: Command line reference for the vcpkg integrate command. Integrate vcpkg with shells and buildsystems.
ms.date: 01/10/2024
---
# vcpkg integrate

## Synopsis

- **Build system integration**

```console
vcpkg integrate [options] install
vcpkg integrate [options] remove
vcpkg integrate [options] project
```

- **Shell integration**

```console
vcpkg integrate [options] powershell
vcpkg integrate [options] bash
vcpkg integrate [options] zsh
vcpkg integrate [options] x-fish
```

## Description

Integrate vcpkg with shells and buildsystems.

### `vcpkg integrate install`

Integrates with [Visual Studio](../users/buildsystems/msbuild-integration.md#user-wide-integration) (Windows-only), sets the user-wide vcpkg instance, and displays CMake integration help.

On Windows with Visual Studio 2015, this subcommand will add redirecting logic into the MSBuild installation which will automatically pick up each user's user-wide vcpkg instance. Visual Studio 2017 and newer have this logic in the box.

To set the user-wide vcpkg instance, vcpkg creates a few short files containing the absolute path to the vcpkg instance inside the user's user-wide configuration location:

- `%LOCALAPPDATA%\vcpkg` or `%APPDATA%\Local\vcpkg` on Windows
- `$HOME/.vcpkg` or `/var/.vcpkg` on non-Windows

Displays the full path to the [CMake toolchain file](../users/buildsystems/cmake-integration.md). Running this command is not required to use the toolchain file.

### `vcpkg integrate remove`

Removes the user-wide vcpkg instance setting.

This command deletes the linking files from the user-wide configuration location created by `vcpkg integrate install`.

### `vcpkg integrate project`

Creates a linked NuGet package for MSBuild integration.

See [MSBuild Per-Project Integration](../users/buildsystems/msbuild-integration.md#linked-nuget-package) for more information.

### <a name="vcpkg-autocompletion"></a> `vcpkg integrate powershell`

- **Windows only**

Adds vcpkg tab-completion support to the current user's Powershell profile.

### `vcpkg integrate bash`

- **Non-Windows only**

Adds vcpkg tab-completion support to the current user's `.bashrc` (`.bash_profile` on MacOS).

### `vcpkg integrate zsh`

- **Non-Windows only**

Adds vcpkg tab-completion support to the current user's `.zshrc`.

### `vcpkg integrate x-fish`

- **Non-Windows only**

Adds vcpkg tab-completion support to the current user's fish shell completions directory.

## Example

```console
$ vcpkg integrate install
Applied user-wide integration for this vcpkg root.

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=/workspaces/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

## Options

All vcpkg commands support a set of [common options](common-options.md).
