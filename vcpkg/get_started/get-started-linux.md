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
   
   Downloads the vcpkg registry. 

    ```bash
    git clone https://github.com/microsoft/vcpkg.git
    ```
   vcpkg's curated registry refers to a set of libraries that have been vetted and packaged to work smoothly with the vcpkg package management system. These libraries are usually open-source projects from various domains, and they are included in vcpkg's main GitHub repository. Each package or "port" in the curated registry typically contains:
   \
   **a.** `Portfile.cmake`: A CMake script that contains instructions for downloading, building, and installing the library.
   **b.** `vcpkg.json`: Information like version numbers, dependencies, and other data that vcpkg uses to manage the package.
   **c.** Patches: Any necessary patches to make the library work with vcpkg or to fix issues in the library itself.

2. **Navigate to vcpkg Directory and Bootstrap vcpkg**
   Provides the vcpkg executable.
    ```bash
    cd vcpkg && ./bootstrap-vcpkg.sh
    ```
3. **Integrate vcpkg with Bash**
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
    >The `vcpkg new` command searches for a directory named "helloworld" to place the manifest file. Hence, the projects name should exactly match the name of the projects directory.
  
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
    
    The `vcpkg.json` manifest file serves as the central configuration for specifying dependencies in a project that uses the vcpkg package manager. It lists all required libraries, potentially specific versions, and can even specify platform-dependent requirements. Optional features within those libraries can also be enabled or disabled via the manifest. 
    
    
1. **Add dependencies**
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

2. **Create `CMakeLists.txt` and `main.cpp` Files**

    ```bash
    touch CMakeLists.txt main.cpp
    ```

3. **Edit `CMakeLists.txt` and `main.cpp`**

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

1. Configure the Build with CMake:

    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/vcpkg/scripts/buildsystems/vcpkg.cmake
    ```

    Ensure to replace \<vcpkg-root\> with the actual path to the vcpkg directory. The toolchain file teaches cmake where to find your installed libraries.

2. Build the project

    ```bash
    cmake --build build
    ```

3. Run the Executable

    ```bash
    ./build/HelloWorld
    ```
