---
title: vcpkg env
description: Command-line reference for the env command. Creates a clean build environment in your current terminal.
ms.author: viromer
ms.date: 8/8/2023
---

# vcpkg env


## Synopsis

> [!NOTE]
> This command is only supported on Windows.

```Console
vcpkg env [<optional command>] [--triplet=<triplet>] [--options]
```

## Description

Creates a clean build environment in your current terminal. The created environment is a
`cmd` session initialized to match the one vcpkg uses to build ports.

The session's configuration depends on your [triplet settings](../users/triplets.md). Use
the `--triplet` option to specify the triplet to target, otherwise, a [default triplet is
deduced](../users/config-environment.md#vcpkg_default_triplet). Depending on your settings
this may initialize a Visual Studio build environment via `vcvars`.

Most environment variables are cleared from the session, with the exception of the environment
variables specified in the [`VCPKG_KEEP_ENV_VARS`](../users/config-environment.md#vcpkg_keep_env_vars)
environment variable and the [`VCPKG_ENV_PASSTROUGH`](../users/triplets.md#vcpkg_env_passthrough)
triplet setting.

Using the `--bin`, `--debug-bin`, `--include`, `--tools`, or `--python` options prepends the
respective paths from the triplet's `installed` folder to the session's environment variables.
Use these if you want to make tools and libraries installed by vcpkg available in the session.
See the build system [manual integration documentation](../users/buildsystems/manual-integration.md)
to learn more about the `installed` folder's hierarchy.

Optionally, you can specify a single command to execute. The session will terminate immediately afterwards.

## Examples

### Create a build environment session

```Console
PS C:\Users\vcpkg\vcpkg> vcpkg env
Microsoft Windows [Version 10.0.25905.1000]
(c) Microsoft Corporation. All rights reserved.

C:\Users\vcpkg\vcpkg>
```

### Run a single command

```Console
PS C:\Users\vcpkg\vcpkg> vcpkg env "cmake --version"
cmake version 3.20.21032501-MSVC_2

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

### Preserve environment variables

```Console
PS C:\Users\vcpkg\vcpkg> $env:CLEARED_ENV_VAR="hello"
PS C:\Users\vcpkg\vcpkg> vcpkg env "set CLEARED_ENV_VAR"
Environment variable CLEARED_ENV_VAR not defined
PS C:\Users\vcpkg\vcpkg> $env:VCPKG_KEEP_ENV_VARS="CLEARED_ENV_VAR"
PS C:\Users\vcpkg\vcpkg> vcpkg env "set CLEARED_ENV_VAR"
CLEARED_ENV_VAR=hello
```

### Add `--tools` to the environment

```Console
PS C:\Users\vcpkg\vcpkg> vcpkg install minizip[tool]
PS C:\Users\vcpkg\vcpkg> vcpkg env --tools "minizip"
MiniZip 1.1, demo of zLib + MiniZip64 package, written by Gilles Vollant
more info on MiniZip at http://www.winimage.com/zLibDll/minizip.html

Usage : minizip [-o] [-a] [-0 to -9] [-p password] [-j] file.zip [files_to_add]

  -o  Overwrite existing file.zip
  -a  Append to existing file.zip
  -0  Store only
  -1  Compress faster
  -9  Compress better

  -j  exclude path. store only the file name.
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--triplet=<triplet>`

Specifies the triplet to target when configuring the environment. This affects how the Visual Studio
build environment is initialized and the `installed` paths used by the other options.

See
[`--triplet` in the common options](common-options.md#triplet) for more information.

### `--bin`

Adds the triplet's installed `bin` folder to the session's `PATH` environment variable.

### `--debug-bin`

Adds the triplet's installed `debug/bin` folder to the session's `PATH` environment variable.

### `--include`

Adds the triplet's installed `include` folder to the session's `INCLUDE` environment variable.

### `--tools`

Adds the triplet's installed `tools` folder to the session's `PATH` environment variable.

### `--python`

Adds the triplet's installed `python` folder to the session's `PYTHONPATH` environment variable.
