---
title: Getting Started with Manifest mode
description: Learn to get started in vcpkg with Manifest mode.
ms.date: 11/30/2022
---
# Manifest mode: CMake example

In this tutorial, you'll create a CMake project using the open source libraries [fmt](https://github.com/fmtlib/fmt), [range-v3](https://github.com/ericniebler/range-v3), and [cxxopts](https://github.com/jarro2783/cxxopts).

## Step 1. Install vcpkg

Download vcpkg using Git and run bootstrap to get the tool.

```cmd
D:\src> git clone https://github.com/microsoft/vcpkg
Cloning into 'vcpkg'...
remote: Enumerating objects: 190163, done.
remote: Counting objects: 100% (1565/1565), done.
remote: Compressing objects: 100% (590/590), done.
remote: Total 190163 (delta 1041), reused 1436 (delta 975), pack-reused 188598     
Receiving objects: 100% (190163/190163), 70.97 MiB | 20.73 MiB/s, done.
Resolving deltas: 100% (122381/122381), done.
Updating files: 100% (10014/10014), done.
D:\src> cd vcpkg
D:\src\vcpkg> .\bootstrap-vcpkg.bat
```

In the rest of this tutorial, we will assume you have cloned vcpkg into `D:\src\vcpkg`.

## Step 2. Create the project

Our project will have the following file layout:

```
fibo/
  main.cxx
  CMakeLists.txt
  vcpkg.json
```

First, create `CMakeLists.txt` with the following content:

:::code language="cmake" source="snippets/manifest-mode-cmake/CMakeLists.txt":::

Next, create `main.cxx` with the following content:

:::code language="cxx" source="snippets/manifest-mode-cmake/main.cxx":::

Finally, we'll create [`vcpkg.json`](../reference/vcpkg-json.md) in the same directory as the project's `CMakeLists.txt` listing our open source dependencies:

:::code language="json" source="snippets/manifest-mode-cmake/vcpkg.json":::

## Step 3. Configure and Build

In a [Developer Command Prompt](/visualstudio/ide/reference/command-prompt-powershell) open to the `fibo/` project directory, run `cmake` to configure the project using the [vcpkg toolchain file](../users/buildsystems/cmake-integration.md#cmake_toolchain_file). vcpkg will run during the configure step to build your open source dependencies and provide them to `find_package()`.

```cmd
D:\src\fibo> cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=D:\src\vcpkg\scripts\buildsystems\vcpkg.cmake
-- Running vcpkg install
Detecting compiler hash for triplet x64-windows...
The following packages will be built and installed:
    cxxopts[core]:x64-windows
    fmt[core]:x64-windows
    range-v3[core]:x64-windows
Starting package 1/3: cxxopts:x64-windows
...
Total elapsed time: 1.742 s

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
-- Build files have been written to: D:/src/fibo/build
```

Once the configure step completes, build the project using `cmake --build build`:

```cmd
D:\src\fibo> cmake --build build
Microsoft (R) Build Engine version 16.7.0+b89cb5fde for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  Building Custom Rule D:/src/fibo/CMakeLists.txt
  main.cxx
  fibo.vcxproj -> D:\src\fibo\build\Debug\fibo.exe
  Building Custom Rule D:/src/fibo/CMakeLists.txt
```

Finally, we can run our build program directly from the Debug directory (`build\Debug\fibo.exe`).

```cmd
D:\src\fibo> .\build\Debug\fibo.exe -n 7 
fib(1) = 1
fib(2) = 1
fib(3) = 2
fib(4) = 3
fib(5) = 5
fib(6) = 8
fib(7) = 13
```

## Next Steps

In this tutorial, you created a simple command line application using multiple open source libraries.

The next steps for this project include:

- Lock down your versions for repeatable builds using [Versioning](../users/versioning.concepts.md)
- Reuse binaries across Continuous Integration runs using [Binary Caching](../users/binarycaching.md)
- Manage your private libraries with vcpkg using [Custom Registries](../maintainers/registries.md)
