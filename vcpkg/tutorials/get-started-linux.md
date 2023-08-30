---
title: Installing and Using Packages on Linux
description: Tutorial guides the user through the process of installing and using packages on Linux with vcpkg.
author: JavierMatosD
ms.date: 08/23/2023
ms.author: javiermat
ms.prod: vcpkg
---
# Installing and Using Packages on Linux

## Description

In this tutorial, you will learn how to use vcpkg to set up a new "Hello World" project using the `fmt` library, manage dependencies, build, and run your C++ application. 

## Setup vcpkg

1. **Clone the vcpkg repository**
   Downloads the vcpkg registry. A registry is a collection of ports that contain open source libraries.
    ```bash
    git clone https://github.com/microsoft/vcpkg.git
    ```

1. **Navigate to vcpkg Directory and Bootstrap vcpkg**
   Provides the vcpkg executable.
    ```bash
    cd vcpkg && ./bootstrap-vcpkg.sh
    ```
2. **Integrate vcpkg with Bash**
   Adds vcpkg tab-completion support to the current user's `.bashrc`.
    ```bash
    ./vcpkg integrate bash
    ```

## Setup the Project

1. **Create and Navigate to the Project Directory**

    ```bash
    cd .. && mkdir helloworld && cd helloworld
    ```

2. **Create a new project**
    Creates a manifest file (`vcpkg.json`) and default configuration file (`vcpkg-configuration.json`)
ready to use in your project.
    ```bash
    ../vcpkg/vcpkg new --name helloworld --version 1.0
    ```
    >[!NOTE]
    >The name of the project should exactly match the projects directory.

    The `vcpkg.json` and `vcpkg-configuration.json` should look like this:
    **`vcpkg.json`**
    ```json
    {
      "name": "helloworld",
      "version": "1.0"
    }
    ```
    **`vcpkg-configuration.json`**
    ```json
    {
      "default-registry": {
          "kind": "git",
          "baseline": "b98e11a16474488633d8a1ab3e160da1f94b98e2",
          "repository": "https://github.com/microsoft/vcpkg"
    },
      "registries": [
          {
            "kind": "artifact",
            "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
            "name": "microsoft"
          }
       ]
    }
    ```

3. **Add dependencies**
    Adds a dependency to your existing `vcpkg.json`.
   ```bash
   ../vcpkg/vcpkg add port fmt
   ```

   Your `vcpkg.json` should look like this:
    ```json
    {
      "name": "helloworld",
      "version": "1.0",
      "dependencies": [
        "fmt"
      ]
    }
    ```

4. **Create `CMakeLists.txt` and `main.cpp` Files**

    ```bash
    touch CMakeLists.txt main.cpp
    ```

5. **Edit `CMakeLists.txt` and `main.cpp`**

    Edit `CMakeLists.txt` to look like this:

    ```cmake
    cmake_minimum_required(VERSION 3.10)

    project(HelloWorld)

    find_package(fmt CONFIG REQUIRED)

    add_executable(HelloWorld main.cpp)

    target_link_libraries(HelloWorld PRIVATE fmt::fmt)
    ```

    Edit `main.cpp` to look like this:
    ```cpp
    #include <fmt/core.h>

    int main(){
    fmt::print("Hello World!\n");
    return 0;
    }
    ```

## Build and Run the Project

1. Configure the Build with CMake

    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg.cmake
    ```
2. Build the project

    ```bash
    cmake --build build
    ```

3. Run the Executable

    ```bash
    ./build/HelloWorld
    ```