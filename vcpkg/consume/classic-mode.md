---
title: Install a dependency from a command-line
description: Learn to install your project's dependencies with vcpkg using the command line.
author: vicroms
ms.author: viromer
ms.date: 8/10/2023
ms.prod: vcpkg
---
# Install a dependency from a command-line (classic mode)

> [!TIP]
> See ["Install a dependency from a manifest file"](manifest-mode.md) for the recommended method of installing your
> dependencies.

> [!WARNING]
> Some features are not available in classic mode.

vcpkg has two operation modes. This article describes how to install packages using classic
mode, for most users we recommend using manifest mode instead.

In this mode, you use vcpkg as a command-line interface to install your dependencies in a global
[installation tree](../users/buildsystems/manual-integration.md).

## Pre-requisite: Create your project

For this tutorial, we'll use the following source file:

`main.cxx`:

:::code language="cxx" source="../examples/snippets/manifest-mode-cmake/main.cxx":::

## Step 1: Install the dependencies

The project depends on the open-source libraries: `cxxopts`, `fmt`, and `range-v3`; these are all
available in the vcpkg public registry at <https://github.com/Microsoft/vcpkg>.

To install these packages use the [`vcpkg install`](../commands/install.md) command.

```Console
$ ./vcpkg install cxxopts fmt range-v3
Computing installation plan...
The following packages will be built and installed:
    cxxopts:x64-windows -> 3.1.1
    fmt:x64-windows -> 10.0.0
    range-v3:x64-windows -> 0.12.0#1
  * vcpkg-cmake:x64-windows -> 2023-05-04
  * vcpkg-cmake-config:x64-windows -> 2022-02-06#1
Additional packages (*) will be modified to complete this operation.
(omitted)
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

## Step 2: Integrate with your project

### [CMake](#tab/cmake)

### Step 2.1: Create a `CMakeLists.txt` file

When using CMake, you can take advantage of the automatic integration through the [vcpkg toolchain
file](../users/buildsystems/cmake-integration.md#cmake_toolchain_file).

Let's add the following `CMakeLists.txt` file in the project folder:

`CMakeLists.txt`:

:::code language="cmake" source="../examples/snippets/manifest-mode-cmake/CMakeLists.txt":::

### Step 2.2: Configure the project

Add `-DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake` as a parameter to the
CMake configure call and vcpkg will be automatically invoked to install the manifest dependencies.

The command should look something like this: `cmake -B <build directory> -S <source directory> -DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake`

```Console
PS D:\projects\manifest-example> cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.25921.
-- The CXX compiler identification is MSVC 19.37.32814.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.37.32814/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: D:/projects/manifest-example/build
```

### Step 2.3: Build the project (CMake)

Once the configure step is completed, build the project with `cmake --build <build directory>`:

```Console
PS D:\projects\manifest-example> cmake --build build
MSBuild version 17.7.0-preview-23319-02+6829506b8 for .NET Framework

  Checking Build System
  Building Custom Rule D:/projects/manifest-example/CMakeLists.txt
  main.cxx
  fibo.vcxproj -> D:\projects\manifest-example\build\Debug\fibo.exe
  Building Custom Rule D:/projects/manifest-example/CMakeLists.txt
```

### [MSBuild](#tab/msbuild)

### Step 2.1: Enable user-wide integration (MSBuild)

To use [vcpkg in your MSBuild or Visual Studio projects](../users/buildsystems/msbuild-integration.md) run the
[`vcpkg integrate`](../commands/integrate.md) command:

```Console
vcpkg integrate install
```

This integration method automatically adds vcpkg-installed packages to the project's Include Directories,
Link Directories, and Link Libraries. Additionaly, this creates a post-build action that ensures
that any required DLLs are copied in the build output folder. This works for all VS2015, VS2017,
VS2019, and VS2022 projects.

### Step 2.2: Build the project (MSBuild)

> [!IMPORTANT]
> Make sure that the [triplet](../users/triplets.md) of your installed packages matches your project's
> configuration. Use `x64-windows` or `x64-windows-static` for your 64-bits projects and
> `x86-windows` or `x86-windows-static` for your 32-bits projects.

After the previous steps, vcpkg-installed packages should be automatically available when building
your project. 

Run `msbuild` to build the project:

```Console
PS D:\projects\manifest-example> msbuild
MSBuild version 17.7.0-preview-23319-02+6829506b8 for .NET Framework
Build started 8/13/2023 3:07:36 PM.

Project "D:\projects\manifest-example\manifest-example.sln" on node 1 (default targets).
ValidateSolutionConfiguration:
  (omitted)
PrepareForBuild:
  (omitted)
InitializeBuildStatus:
  (omitted)
ComputeStdModulesCompileInputs:
  (omitted)
SetModuleDependencies:
VcpkgTripletSelection:
  Using triplet "x64-windows" from "D:\vcpkg\installed\x64-windows\"
  Using normalized configuration "Debug"
ClCompile:
  (omitted)
Link:
  (omitted)
AppLocalFromInstalled:
  pwsh.exe -ExecutionPolicy Bypass -noprofile -File "D:\vcpkg\scripts\buildsystems\msbuild\applocal.ps1" "D:\projects\manifest-example\x64\Debug\manifest-example.exe"
   "D:\vcpkg\installed\x64-windows\debug\bin" "x64\Debug\manifest-example.tlog\manifest-example.write.1u.tlog" "x64\Debug\vcpkg.applocal.log"
  D:\projects\manifest-example\x64\Debug\fmtd.dll
FinalizeBuildStatus:
  Deleting file "x64\Debug\manifest-example.tlog\unsuccessfulbuild".
  Touching "x64\Debug\manifest-example.tlog\manifest-example.lastbuildstate".
Done Building Project "D:\projects\manifest-example\manifest-example.vcxproj" (default targets).

Done Building Project "D:\projects\manifest-example\manifest-example.sln" (default targets).

Build succeeded.
```

#### [Visual Studio](#tab/vs-integration)

### Step 2.1: Enable user-wide integration (Visual Studio)

> [!IMPORTANT]
> Make sure that the [triplet](../users/triplets.md) of your installed packages matches your project's
> configuration. Use `x64-windows` or `x64-windows-static` for your 64-bits projects and
> `x86-windows` or `x86-windows-static` for your 32-bits projects.

To use [vcpkg in your MSBuild or Visual Studio projects](../users/buildsystems/msbuild-integration.md) run the
[`vcpkg integrate`](../commands/integrate.md) command:

```Console
vcpkg integrate install
```

This integration method automatically adds vcpkg-installed packages to the project's Include Directories,
Link Directories, and Link Libraries. Additionaly, this creates a post-build action that ensures
that any required DLLs are copied in the build output folder. This works for all VS2015, VS2017,
VS2019, and VS2022 projects.

### Step 2.2: Build the project (Visual Studio)

After the previous steps, vcpkg-installed packages should be automatically available when building
your project. 

Build the project:

```Console
Build started...
1>------ Build started: Project: manifest-example, Configuration: Debug x64 ------
1>main.cxx
1>manifest-example.vcxproj -> D:\projects\manifest-example\x64\Debug\manifest-example.exe
1>Done building project "manifest-example.vcxproj".
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build started at 3:16 PM and took 18.608 seconds ==========
```

---

## Next Steps

In this tutorial, you installed dependencies for a simple project using a manifest file.

The next steps for this project include:

* Locking down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reusing binaries across Continuous Integration runs using [binary caching](../users/binarycaching.md)
* Managing your private libraries using [custom registries](../maintainers/registries.md)
