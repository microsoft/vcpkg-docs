---
title: Install a dependency from a manifest file
description: Learn to install your project's dependencies with vcpkg by using a manifest file.
author: vicroms
ms.author: viromer
ms.date: 8/10/2023
ms.prod: vcpkg
---
# Install a dependency from a manifest file (manifest mode)

vcpkg has two operation modes: classic mode, and manifest mode. This article describes how to install packages using manifest mode, which is the recommended workflow for most users.

In manifest mode, you declare your project's direct dependencies in a manifest file named
[`vcpkg.json`](../reference/vcpkg-json.md).

Manifest files have their own `vcpkg_installed` directory where they install their dependencies,
unlike classic mode, where all packages are installed in a common `%VCPKG_ROOT%/installed`
directory. Manifests are suitable for using vcpkg with multiple codebases, as each codebase can have
a different set of dependencies that do not interfere with each other.

Manifest mode is also required to use advanced features like
[versioning](../users/versioning.md) and [custom registries](../users/registries.md).

## Create your project

For this tutorial, we use the following source file:

`main.cxx`:

:::code language="cxx" source="../examples/snippets/manifest-mode-cmake/main.cxx":::

## Create the manifest file

The code references the open-source libraries: `cxxopts`, `fmt`, and `range-v3`; which are all
available in the vcpkg public registry at <https://github.com/Microsoft/vcpkg>.

To satisfy these dependencies, create a file named `vcpkg.json` in the project's folder:

`vcpkg.json`:

:::code language="json" source="../examples/snippets/manifest-mode-cmake/vcpkg.json":::

You only need to specify your direct dependencies in the `"dependencies"` list. When it runs, vcpkg
resolves and installs any required transitive dependencies.

## Install the dependencies

### [MSBuild](#tab/msbuild)

> [!div class="nextstepaction"]
> [Learn more about using vcpkg from MSBuild](../users/buildsystems/msbuild-integration.md)

### Enable user-wide integration (MSBuild)

To use [vcpkg in your MSBuild projects](../users/buildsystems/msbuild-integration.md) run the
following command:

```Console
vcpkg integrate install
```

You only need to run the `vcpkg integrate install` command the first time you want to enable MSBuild
integration. This enables MSBuild integration for all your existing and future projects. Use [`vcpkg integrate remove`](../commands/integrate.md#vcpkg-integrate-remove) to remove MSBuild system-wide integration.

This integration method automatically adds vcpkg-installed packages to the following project properties: Include Directories, Link Directories, and Link Libraries. Additionally, this creates a post-build action that ensures
that any required DLLs are copied in the build output folder. This works for all solutions and projects using
VS2015 or newer.

### Build the project (MSBuild)

By default, manifest mode is disabled in MSBuild projects.

To enable manifests in your project, set the `VcpkgEnableManifest` property in your `.vcxproj` file:
  
```xml
<PropertyGroup Label="Vcpkg">
  <VcpkgEnableManifest>true</VcpkgEnableManifest>
</PropertyGroup>
```

Alternatively you can enable manifest mode in your MSBuild call: `msbuild /p:VcpkgEnableManifest=true`.

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

### [Visual Studio](#tab/visual-studio)

> [!div class="nextstepaction"]
> [Learn more about using vcpkg from Visual Studio](../users/buildsystems/msbuild-integration.md)

### Enable user-wide integration (Visual Studio)

To use [vcpkg in your Visual Studio projects](../users/buildsystems/msbuild-integration.md) run the
following command:

```Console
vcpkg integrate install
```

You only need to run the `vcpkg integrate install` command the first time you want to enable MSBuild
integration. This enables MSBuild integration for all your existing and future projects. Use [`vcpkg integrate remove`](../commands/integrate.md#vcpkg-integrate-remove) to remove MSBuild system-wide integration.

This integration method automatically adds vcpkg-installed packages to the following project properties: Include Directories, Link Directories, and Link Libraries. Additionally, this creates a post-build action that ensures
that any required DLLs are copied in the build output folder. This works for all projects using
VS2015 or newer.

### Enable vcpkg manifests

By default, manifest mode is disabled in Visual Studio projects. To enable manifests, open the
project's Properties window and set `Use Vcpkg Manifest` to "Yes" in the vcpkg tab.

:::image type="complex" source="../resources/vs-enable-vcpkg-manifest.png" alt-text="Screenshot of a Visual Studio's project Properties window.":::
Screenshot of a Visual Studio project properties window. The VCPKG tab is selected and highlighted. The
option named "use VCPKG manifest" is highlighted with its value set to "Yes".
:::image-end:::

### Build the project (Visual Studio)

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

### [CMake](#tab/cmake-integration)

> [!div class="nextstepaction"]
> [Learn more about using vcpkg from CMake](../users/buildsystems/cmake-integration.md)

### Create a `CMakeLists.txt` file

The vcpkg tool integrates with CMake through the [vcpkg toolchain
file](../users/buildsystems/cmake-integration.md#cmake_toolchain_file).

Add the following `CMakeLists.txt` file in the project folder:

`CMakeLists.txt`:

:::code language="cmake" source="../examples/snippets/manifest-mode-cmake/CMakeLists.txt":::

### Configure the project

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

### Build the project (CMake)

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

### [Manual installation](#tab/manual-installation)

> [!div class="nextstepaction"]
> [Learn more about manually integrating vcpkg in your build system](../users/buildsystems/manual-integration.md)

In a terminal open to the project's folder run the `vcpkg install` command.

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

---

## Next Steps

In this guide, you installed dependencies for a simple project using a manifest file.

Here are some additional tasks to try next:

> [!div class="nextstepaction"]
> [Install packages for custom platforms using triplets](../users/triplets.md)
> [Lock down your versions for repeatable builds using versioning](../users/versioning.concepts.md)
> [Reuse binaries across continuous integration runs using binary caching](../users/binarycaching.md)
> [Manage your private libraries using custom registries](../maintainers/registries.md)
