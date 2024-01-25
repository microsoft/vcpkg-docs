---
title: "Tutorial: Export compiled dependencies"
description: In this tutorial we show you how to export compiled dependencies to be consumed by another project.
author: JavierMatosD
ms.author: Javiermat
ms.date: 11/06/2023
ms.topic: tutorial
# CustomerIntent: As an advanced vcpkg user I want to export compiled dependencies to be consumed by another project.
---

# Tutorial: Export compiled dependencies

In this tutorial, we'll walk through how to export compiled dependencies using vcpkg. This can be particularly useful when you want to share libraries across multiple projects or for ease of distribution.

## Prerequisites:

- [Visual Studio](<https://visualstudio.microsoft.com/downloads/>) with C++ development workload and [CMake component](</cpp/build/cmake-projects-in-visual-studio#installation>)
- [Git](<https://git-scm.com/downloads>)
* vcpkg
* A terminal

## 1 - Install `fmt` library

Open a terminal and navigate to the directory where vcpkg is installed then install the `fmt` library for your target platform:

```console
vcpkg install fmt:x64-windows
```

Replace `x64-windows` with your target triplet.

## 2 - Export `fmt` library

After installing `fmt`, export it by running:

```console
vcpkg export fmt:x64-windows --zip
```

You should see output similar to the following:

```console
The following packages are already built and will be exported:
    fmt:x64-windows
  * vcpkg-cmake:x64-windows
  * vcpkg-cmake-config:x64-windows
Additional packages (*) need to be exported to complete this operation.
Exporting vcpkg-cmake:x64-windows...
Exporting vcpkg-cmake-config:x64-windows...
Exporting fmt:x64-windows...
Creating zip archive...
Zip archive exported at: C:\dev\vcpkg\vcpkg-export-20231106-164550.zip
To use exported libraries in CMake projects, add -DCMAKE_TOOLCHAIN_FILE=[...]/scripts/buildsystems/vcpkg.cmake to your CMake command line.
```

The command creates a zip archive containing all the necessary files to use the `fmt` library, including binaries and headers. For a list of all the supported formats check out the [`export` command documentation](../commands/export.md#formats).

## 3 - Use the `fmt` library in a new CMake project

To use the `fmt` library in a new CMake project, follow these steps:

1. Unzip the exported archive to a known location on your machine.
2. Make a new directory for your CMake project and navigate into it.
3. Create a `CMakeLists.txt` with the following content:

    ```CMake
    cmake_minimum_required(VERSION 3.10)
    project(HelloWorld)

    find_package(fmt CONFIG REQUIRED)
    add_executable(HelloWorld main.cpp)
    target_link_libraries(HelloWorld PRIVATE fmt::fmt)
    ```

4. Write your `main.cpp` file to include `fmt` and print "Hello, World":
   
   ```cpp
    #include <fmt/core.h>

    int main() 
    {
        fmt::print("Hello, World!\n");
        return 0;
    }
   ```

5. Build the project - In your terminal, from the project directory, run:
   
   ```console
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-archive>/scripts/buildsystems/vcpkg.cmake
   cmake --build build
   ```
   
6. Execute:
   
   ```console
   ..\build\Debug\HelloWorld.exe
   ```

## Next Steps

And that's it! You have exported the `fmt` library and used it in a separate project.

