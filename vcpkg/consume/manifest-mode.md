---
title: "Tutorial: Install a dependency from a manifest file"
description: Learn to install your projects dependencies with vcpkg by using a manifest file.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to learn how to install my project dependencies using vcpkg in manifest mode
---
# Tutorial: Install a dependency from a manifest file

vcpkg has two operation modes: classic mode and manifest mode. This article
describes how to install packages using manifest mode, which is the recommended
workflow for most users.

In manifest mode, you declare your project's direct dependencies in a manifest file named
[`vcpkg.json`](../reference/vcpkg-json.md).

Manifest files have their own `vcpkg_installed` directory where they install dependencies,
unlike classic mode, where all packages are installed in a common `%VCPKG_ROOT%/installed`
directory. Therefore, each project can have its own manifest and its own set of dependencies that do not conflict with other projects' dependencies.

Manifest mode is also required to use advanced features like
[versioning](../users/versioning.md) and [custom registries](../concepts/registries.md).

In this tutorial, you will learn how to:

> [!div class="checklist"]
> * [Create a project with a manifest]
> * [Integrate vcpkg with your build system]
> * [Install dependencies]
> * [Build the project]

## Prerequisites

* vcpkg
* A terminal
* A code editor
* A C++ compiler
* (Optional) CMake or MSBuild

## 1 - Create a project with a manifest

In a new folder, create a source file named `main.cxx` with these contents:

:::code language="cpp" source="../examples/snippets/manifest-mode-cmake/main.cxx":::

The code references the open-source libraries: `cxxopts`, `fmt`, and `range-v3`; which are all
available in the vcpkg public registry at <https://github.com/Microsoft/vcpkg>.

To declare these dependencies, create a file named `vcpkg.json` in the same directory as your project:

`vcpkg.json`:

:::code language="json" source="../examples/snippets/manifest-mode-cmake/vcpkg.json":::

You only need to specify your direct dependencies in the `"dependencies"` list. When it runs, vcpkg
resolves and installs any required transitive dependencies.

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

Use any of these methods to set the toolchain file:

* Set the `CMAKE_TOOLCHAIN_FILE` in your CMakePresets.json file.
* Pass `-DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake` as a parameter in your
CMake configure call.
* Set the `CMAKE_TOOLCHAIN_FILE` CMake variable before the first call to `project()` in your
`CMakeLists.txt` file.

---

## 3 - Install dependencies

If you're using CMake or MSBuild and followed the previous step, you can skip ahead to the next step:
[Build the project].

If you're using a different build system or want to install the dependencies manually, all you need to do
is run `vcpkg install` in the directory containing your manifest file.

```Console
PS D:\projects\manifest-example> vcpkg install
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

When the command finishes, all built packages will be present in a `vcpkg_installed` directory. The specific location of this directory depends on your build system; usually, inside the build system's default output folder, or next to your `vcpkg.json` file. 

## 4 - Build the project

### [MSBuild](#tab/build-MSBuild)

By default, manifest mode is disabled in MSBuild projects.

To enable manifests in your project, set the `VcpkgEnableManifest` property in your `.vcxproj` file:
  
```xml
<PropertyGroup Label="Vcpkg">
  <VcpkgEnableManifest>true</VcpkgEnableManifest>
</PropertyGroup>
```

Alternatively, you can enable manifest mode in your MSBuild call by passing `msbuild /p:VcpkgEnableManifest=true` as a parameter.

```Console
PS D:\projects\manifest-example> msbuild /p:VcpkgEnableManifest=true
MSBuild version 17.7.0-preview-23319-02+6829506b8 for .NET Framework
Build started 8/11/2023 11:29:50 AM.

Project "D:\projects\manifest-example\manifest-example.sln" on node 1 (default targets).
ValidateSolutionConfiguration:
  Building solution configuration "Debug|x64".
Project "D:\projects\manifest-example\manifest-example.sln" (1) is building "D:\projects\manifest-example\manifest-example.vcxproj" (2) on node 1 (default targets).
PrepareForBuild:
  (omitted)
InitializeBuildStatus:
  (omitted)
ComputeStdModulesCompileInputs:
  (omitted)
SetModuleDependencies:
  Creating directory "x64\Debug\manifest.ceffc6eb_MD.tlog\".
VcpkgTripletSelection:
  Using triplet "x64-windows" from "D:\projects\manifest-example\vcpkg_installed\x64-windows\x64-windows\"
  Using normalized configuration "Debug"
VcpkgInstallManifestDependencies:
  Installing vcpkg dependencies to D:\projects\manifest-example\vcpkg_installed\x64-windows\
  Creating directory "D:\projects\manifest-example\vcpkg_installed\x64-windows\".
  "D:\vcpkg\vcpkg.exe" install  --x-wait-for-lock --triplet "x64-windows" --vcpkg-root "D:\vcpkg\" "--x-manifest-root=D:\projects\manifest-example\" "--x-install-root=D:\projects\manifest-example\vcpkg_installed\x64-windows\"
  "D:\vcpkg\vcpkg.exe" install  --x-wait-for-lock --triplet "x64-windows" --vcpkg-root "D:\vcpkg\" "--x-manifest-root=D:\projects\manifest-example\" "--x-install-root=D:\projects\manifest-example\vcpkg_installed\x64-windows\"
  Detecting compiler hash for triplet x64-windows...
  The following packages will be built and installed:
      cxxopts:x64-windows -> 3.1.1
      fmt:x64-windows -> 10.0.0
      range-v3:x64-windows -> 0.12.0#1
    * vcpkg-cmake:x64-windows -> 2023-05-04
    * vcpkg-cmake-config:x64-windows -> 2022-02-06#1
  (omitted)
ClCompile:
  (omitted)
Link:
  (omitted)
AppLocalFromInstalled:
  pwsh.exe -ExecutionPolicy Bypass -noprofile -File "D:\vcpkg\scripts\buildsystems\msbuild\applocal.ps1" "D:\projects\manif
  est-mode-msbuild\x64\Debug\manifest-example.exe" "D:\projects\manifest-example\vcpkg_installed\x64-windows\x64-windows\debug\bin"
  "x64\Debug\manifest.ceffc6eb.tlog\manifest-example.write.1u.tlog" "x64\Debug\vcpkg.applocal.log"
  D:\projects\manifest-example\x64\Debug\fmtd.dll
FinalizeBuildStatus:
  (omitted)
Done Building Project "D:\projects\manifest-example\manifest-example.vcxproj" (default targets).

Done Building Project "D:\projects\manifest-example\manifest-example.sln" (default targets).

Build succeeded.
```

### [Visual Studio](#tab/build-visual-studio)

By default, manifest mode is disabled in Visual Studio projects. To enable manifests, right-click your project in **Solution Explorer**, select **Properties**, select the **vcpkg** tab on the left, and set `Use Vcpkg Manifest` to **Yes**.

:::image type="complex" source="../resources/vs-enable-vcpkg-manifest.png" alt-text="Screenshot of a Visual Studio's project Properties window.":::
Screenshot of a Visual Studio project properties window. The VCPKG tab is selected and highlighted. The
option named "use VCPKG manifest" is highlighted with its value set to "Yes".
:::image-end:::

Build the project by right-clicking it in **Solution Explorer** and selecting **Build**.

```Console
Build started...
1>------ Build started: Project: manifest-example, Configuration: Debug x64 ------
1>Installing vcpkg dependencies to D:\projects\manifest-example\vcpkg_installed\x64-windows\
1>"D:\vcpkg\vcpkg.exe" install  --x-wait-for-lock --triplet "x64-windows" --vcpkg-root "D:\vcpkg\" "--x-manifest-root=D:\projects\manifest-example\" "--x-install-root=D:\projects\manifest-example\vcpkg_installed\x64-windows\"
1>Detecting compiler hash for triplet x64-windows...
1>The following packages will be built and installed:
1>    cxxopts:x64-windows -> 3.1.1
1>    fmt:x64-windows -> 10.0.0
1>    range-v3:x64-windows -> 0.12.0#1
1>  * vcpkg-cmake:x64-windows -> 2023-05-04
1>  * vcpkg-cmake-config:x64-windows -> 2022-02-06#1
1> (omitted)
1>main.cxx
1>manifest-example.vcxproj -> D:\projects\manifest-example\x64\Debug\manifest-example.exe
1>Done building project "manifest-example.vcxproj".
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build started at 12:07 PM and took 15.320 seconds ==========
```

### [CMake](#tab/build-cmake)

#### 1 - Create a CMakeLists.txt file

Add the following `CMakeLists.txt` file in the project folder:

`CMakeLists.txt`:

:::code language="CMake" source="../examples/snippets/manifest-mode-cmake/CMakeLists.txt":::

#### 2 - Configure your CMake project

Run the following command: `cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake`, but
substitute `%VCPKG_ROOT%` with your vcpkg installation path.

Notice how the project's dependencies are automatically installed while configuring the project.

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
<omitted output>
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

#### 3 - Build the CMake project

Run the following command to build the project: `cmake --build build`:

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

---

## Next Steps

In this guide, you installed dependencies for a simple project using a manifest file.

Here are some additional tasks to try next:

* Install packages for custom platforms, compilers, or build architectures using [triplets](../users/triplets.md)
* Lock down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reuse binaries across local or continuous integration runs using [binary caching](../users/binarycaching.md)
* Manage your private libraries using custom [registries](../maintainers/registries.md)

[Create a project with a manifest]: #1---create-a-project-with-a-manifest
[Integrate vcpkg with your build system]: #2---integrate-vcpkg-with-your-build-system
[Install dependencies]: #3---install-dependencies
[Build the project]: #4---build-the-project
