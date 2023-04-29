---
title: Microsoft Visual C++ for Windows
description: Building for Windows with the Microsoft Visual C++ Compiler
ms.date: 04/28/2023
---
# Microsoft Visual C++

## Triplets

vcpkg includes [triplets](https://github.com/microsoft/vcpkg/tree/master/triplets) for building Windows desktop applications using the MSVC ``cl.exe`` compiler.

| Architecture | vcpkg triplets     |
|--------------|--------------------|
| x64          | x64-windows        |
|              | x64-windows-static |
| x86          | x86-windows        |
| arm64        | arm64-windows      |

There are also a number of [community triplets](https://github.com/microsoft/vcpkg/tree/master/triplets/community) for the MSVC compiler toolset as well.

| Architecture | vcpkg community triplets |
|--------------|--------------------------|
| x64          | x64-windows-static-md    |
| x86          | x86-windows-static       |
|              | x86-windows-static-md    |
| arm          | arm-windows              |
|              | arm-windows-static       |
| arm64        | arm64-windows-static     |
|              | arm64-windows-static-md  |
|              | arm64ec-windows          |

> [!NOTE]
> Community triplets are not tested as part of vcpkg repository's CI process, so regressions can occur as part of library updates. PRs improving support are welcome!

The ``static`` linking triplets are set to use the MSVC Runtime as a static library (i.e. ``VCPKG_CRT_LINKAGE static``).

The ``static-md`` linking triplets are set to use the MSVC Runtime as a DLL (i.e. ``VCPKG_CRT_LINKAGE dynamic``). This is the recommended solution for redistributing the MSVC Runtime per [Microsoft Learn](/cpp/windows/deployment-in-visual-cpp).

## Maintainer notes

CMake projects for these triplets are built using ``CMAKE_SYSTEM_NAME`` set to "Windows".
