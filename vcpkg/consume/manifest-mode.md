---
title: Install a dependency from a manifest file
description: Learn to install your project's dependencies with vcpkg by using a manifest file.
author: vicroms
ms.author: viromer
ms.date: 8/10/2023
ms.prod: vcpkg
---
# Install a dependency from a manifest file (manifest mode)

vcpkg has two operation modes, this article describes how to install packages using manifest
mode.

Manifest mode, uses a manifest file to install packages to a per-project installation tree. A
manifest is a file named [`vcpkg.json`](../reference/vcpkg-json.md) where you declare your project's
dependencies.

This is the recommended operation mode for most users. Use this mode when you want to have different sets
of dependencies installed in a per-project basis. Manifest mode is also required to engage some of
vcpkg's advanced features like [versioning](../users/versioning.md) and
[custom registries](../users/registries.md).

## Pre-requisite: Create your project

For this tutorial, we'll use the following source file:

`main.cxx`:

:::code language="cxx" source="../examples/snippets/manifest-mode-cmake/main.cxx":::

## Step 1: Create the manifest file

The project depends on the open-source libraries: `cxxopts`, `fmt`, and `range-v3`; these are all
available in the vcpkg public registry at <https://github.com/Microsoft/vcpkg>.

To satisfy these dependencies, let's create a file named `vcpkg.json` in the project's folder.

`vcpkg.json`:

:::code language="json" source="../examples/snippets/manifest-mode-cmake/vcpkg.json":::

The `"dependencies"` list should contain your project's direct dependencies, vcpkg will take care
of resolving any transitive dependencies and install them as well.

## Step 2. Install the dependencies

### [Manual installation](#tab/manual-installation)

In a [Developer Command Prompt](/visualstudio/ide/reference/command-prompt-powershell) open to the
project's folder run the `vcpkg install` command.

```Console
PS D:\projects\manifest-example> D:\vcpkg\vcpkg.exe install
Detecting compiler hash for triplet x64-windows...
The following packages will be built and installed:
    cxxopts:x64-windows -> 3.1.1
    fmt:x64-windows -> 10.0.0
    range-v3:x64-windows -> 0.12.0#1
  * vcpkg-cmake:x64-windows -> 2023-05-04
  * vcpkg-cmake-config:x64-windows -> 2022-02-06#1
Additional packages (*) will be modified to complete this operation.
Installing 1/5 vcpkg-cmake-config:x64-windows...
Installing 2/5 vcpkg-cmake:x64-windows...
Installing 3/5 cxxopts:x64-windows...
Installing 4/5 fmt:x64-windows...
Installing 5/5 range-v3:x64-windows...
Total install time: 48 s
cxxopts provides CMake targets:

    # this is heuristically generated, and may not be correct
    find_package(cxxopts CONFIG REQUIRED)
    target_link_libraries(main PRIVATE cxxopts::cxxopts)

The package fmt provides CMake targets:

    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(main PRIVATE fmt::fmt)

    # Or use the header-only version
    find_package(fmt CONFIG REQUIRED)
    target_link_libraries(main PRIVATE fmt::fmt-header-only)

range-v3 provides CMake targets:

    # this is heuristically generated, and may not be correct
    find_package(range-v3 CONFIG REQUIRED)
    target_link_libraries(main PRIVATE range-v3::meta range-v3::concepts range-v3::range-v3)
```

Once the command has finished, there will be a new folder named `vcpkg_installed` in your project's
directory containing all the built packages.

### [CMake integration](#tab/cmake-integration)

When using CMake, you can take advantage of the automatic integration through the [vcpkg toolchain
file](../users/buildsystems/cmake-integration.md#cmake_toolchain_file).

### Step 2.1: Create a `CMakeLists.txt` file

Let's add the following `CMakeLists.txt` file in the project folder:

`CMakeLists.txt`:

:::code language="cmake" source="../examples/snippets/manifest-mode-cmake/CMakeLists.txt":::

### Step 2.2: Configure the project

Add `-DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake` as a parameter to the
CMake configure call and vcpkg will be automatically invoked to install the manifest dependencies.

The command should look something like this: `cmake -B <build directory> -S <source directory> -DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake`

```Console
PS D:\projects\manifest-example> cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=D:\vcpkg\scripts\buildsystems\vcpkg.cmake
-- Running vcpkg install
Detecting compiler hash for triplet x64-windows...
The following packages will be built and installed:
    cxxopts:x64-windows -> 3.1.1
    fmt:x64-windows -> 10.0.0
    range-v3:x64-windows -> 0.12.0#1
  * vcpkg-cmake:x64-windows -> 2023-05-04
  * vcpkg-cmake-config:x64-windows -> 2022-02-06#1
<output ommited for brevity>
-- Running vcpkg install - done
-- Selecting Windows SDK version 10.0.18362.0 to target Windows 10.0.19041.
-- The CXX compiler identification is MSVC 19.27.29111.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: D:/projects/manifest-example/build
```

### Step 2.3: Build the project

Once the configure step is completed, build the project with `cmake --build <build directory>`:

```Console
PS D:\projects\manifest-example> cmake --build build
Microsoft (R) Build Engine version 16.7.0+b89cb5fde for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  Building Custom Rule D:\projects\manifest-example\CMakeLists.txt
  main.cxx
  fibo.vcxproj -> D:\projects\manifest-example\build\Debug\fibo.exe
  Building Custom Rule D:\projects\manifest-example\CMakeLists.txt
```

### [MSBuild integration](#tab/msbuild-integration)

TBD

### [Visual Studio integration](#tab/vs-integration)

TBD

---

## Next Steps

In this tutorial, you installed dependencies for a simple project using a manifest file.

The next steps for this project include:

* Locking down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reusing binaries across Continuous Integration runs using [binary caching](../users/binarycaching.md)
* Managing your private libraries using [custom registries](../maintainers/registries.md)
