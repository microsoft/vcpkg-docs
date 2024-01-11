---
title: "Tutorial: Install a dependency from the command line"
description: Learn to install your project's dependencies with vcpkg using the command line.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to learn how to install my project dependencies using vcpkg's command-line interface.
---
# Tutorial: Install a dependency from the command line

> [!TIP]
> See ["Install a dependency from a manifest file"](manifest-mode.md) for the recommended method of installing your
> dependencies.

> [!WARNING]
> Some vcpkg features are not available in classic mode.

vcpkg has two operation modes: classic mode, and manifest mode. This article describes how to install packages using classic
mode. For most users we recommend using manifest mode instead.

In classic mode, you use vcpkg as a command-line interface to install your
dependencies in a common [installation
directory](../users/buildsystems/manual-integration.md). Usually, located in
`%VCPKG_ROOT%/installed`, where `%VCPKG_ROOT%` is the vcpkg's installation
directory.

In this tutorial, you will learn to:

> [!div class="checklist"]
> * [Create a project]
> * [Integrate vcpkg with your build system]
> * [Install dependencies]
> * [Build the project]

## Prerequisites

* vcpkg
* A terminal
* A code editor
* A C++ compiler
* (Optional) CMake or MSBuild

## 1 - Create a project

In a new folder, create a source file named `main.cxx` with these contents:

:::code language="cpp" source="../examples/snippets/manifest-mode-cmake/main.cxx":::

The code references the open-source libraries: `cxxopts`, `fmt`, and `range-v3`; which are all
available in the vcpkg public registry at <https://github.com/Microsoft/vcpkg>.

## 2 - Integrate vcpkg with your build system

In this step we show you how to integrate vcpkg with CMake or MSBuild, so that your project dependencies get automatically installed or restored whenever you build the project.

If you're using a different build system, skip to the next step: [Install dependencies].

### [MSBuild](#tab/msbuild)

> [!div class="nextstepaction"]
> [Learn more about using vcpkg from MSBuild](../users/buildsystems/msbuild-integration.md)

To use [vcpkg in your MSBuild projects](../users/buildsystems/msbuild-integration.md), run the
following command:

```Console
vcpkg integrate install
```

You only need to run the [`vcpkg integrate install`](../commands/integrate.md#vcpkg-integrate-install)
command the first time you want to enable MSBuild integration. This enables MSBuild integration for all
your existing and future projects. Use [`vcpkg integrate remove`](../commands/integrate.md#vcpkg-integrate-remove)
to remove MSBuild system-wide integration.

This integration method automatically adds vcpkg-installed packages to the following project properties:
`Include Directories`, `Link Directories`, and `Link Libraries`. Additionally, this creates a post-build action
that ensures that any required DLLs are copied into the build output folder. This works for all solutions and
projects using Visual Studio 2015 or newer.

### [CMake](#tab/cmake)

> [!div class="nextstepaction"]
> [Learn more about using vcpkg from CMake](../users/buildsystems/cmake-integration.md)

To use [vcpkg in your CMake projects](../users/buildsystems/cmake-integration.md), you need to set the
`CMAKE_TOOLCHAIN_FILE` variable to use vcpkg's CMake toolchain file. The vcpkg toolchain is in
`%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake`, where `%VCPKG_ROOT%` is your vcpkg installation path.

To set the toolchain file use any of these methods:

* Set the `CMAKE_TOOLCHAIN_FILE` in your CMakePresets.json file.
* Pass `-DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake` as a parameter in your
CMake configure call.
* Set the `CMAKE_TOOLCHAIN_FILE` CMake variable before the first call to `project()` in your
`CMakeLists.txt` file.

---

## 3 - Install dependencies

The code references the open-source libraries: `cxxopts`, `fmt`, and `range-v3`; these are all
available in the vcpkg public registry at <https://github.com/Microsoft/vcpkg>.

To install these packages use the [`vcpkg install`](../commands/install.md) command.

```Console
vcpkg install cxxopts fmt range-v3
```

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

## 4 - Build the project

> [!IMPORTANT]
> Make sure that the [triplet](../users/triplets.md) of your installed packages matches your project's
> configuration. Use `x64-windows` or `x64-windows-static` for your 64-bit projects and
> `x86-windows` or `x86-windows-static` for your 32-bit projects.

### [MSBuild](#tab/build-MSBuild)

With system-wide integration enabled, just run `msbuild` to build the project:

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

### [Visual Studio](#tab/build-visual-studio)

Build the project by right-clicking it in **Solution Explorer** and selecting **Build**.

```Console
Build started...
1>------ Build started: Project: manifest-example, Configuration: Debug x64 ------
1>main.cxx
1>manifest-example.vcxproj -> D:\projects\manifest-example\x64\Debug\manifest-example.exe
1>Done building project "manifest-example.vcxproj".
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build started at 3:16 PM and took 18.608 seconds ==========
```

### [CMake](#tab/build-cmake)

#### 1 - Create a CMakeLists.txt file

Add the following `CMakeLists.txt` file in the project folder:

`CMakeLists.txt`:

:::code language="CMake" source="../examples/snippets/manifest-mode-cmake/CMakeLists.txt":::

#### 2 - Configure your CMake project

Run the following command: `cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake`, but
substitute `%VCPKG_ROOT%` with your vcpkg installation path.

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

#### 3 - Build the CMake project

Run the following command to build the project: `cmake --build build`:

```Console
PS D:\projects\manifest-example> cmake --build build
MSBuild version 17.7.0-preview-23319-02+6829506b8 for .NET Framework

  Checking Build System
  Building Custom Rule D:/projects/manifest-example/CMakeLists.txt
  main.cxx
  fibo.vcxproj -> D:\projects\manifest-example\build\Debug\fibo.exe
  Building Custom Rule D:/projects/manifest-example/CMakeLists.txt
```

---

## Next Steps

In this tutorial, you installed dependencies for a simple project using vcpkg as a command-line interface.

Here are some additional tasks to try next:

* Install packages using a [manifest file](manifest-mode.md)
* Install packages for custom platforms using [triplets](../users/triplets.md)
* Lock down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reuse binaries across Continuous Integration runs using [binary caching](../users/binarycaching.md)
* Manage your private libraries using [custom registries](../maintainers/registries.md)

[Create a project]: #1---create-a-project
[Integrate vcpkg with your build system]: #2---integrate-vcpkg-with-your-build-system
[Install dependencies]: #3---install-dependencies
[Build the project]: #4---build-the-project
