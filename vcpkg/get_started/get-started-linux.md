---
title: Install and use packages on Linux
description: Tutorial guides the user through the process of installing and using packages on Linux with vcpkg.
author: JavierMatosD
ms.date: 08/23/2023
ms.author: javiermat
ms.prod: vcpkg
---
# Install and use packages on Linux

## Description

This tutorial guides you through setting up a new C++ project using the fmt library. You'll manage dependencies, build the application, and eventually run your project, all using vcpkg. For this tutorial, you'll need to have `git` and `bash` installed.

## Setup vcpkg

1. **Clone the vcpkg repository**

   The first step is to clone the vcpkg repository. The repository contains scripts to acquire the vcpkg executable and a registry of open-source packages that is curated and maintained by the vcpkg team and it's community of contributors.

    ```bash
    git clone https://github.com/microsoft/vcpkg.git
    ```

   The curated registry is a set of libraries that have been vetted and packaged to work smoothly with the vcpkg. vcpkg uses "ports" to package open-source projects and make them available to install. Ports typically contain:

   **a.** `portfile.cmake`: A CMake script that contains instructions for downloading, building, and installing the library.

   **b.** `vcpkg.json`: A JSON file including package metadata: package version, dependencies, supported platforms and features, etc.

   **c.** Patches: Sometimes a port will contain patches to make the library work with vcpkg or to provide bug fixes in the library itself.

2. **Navigate to vcpkg directory and bootstrap vcpkg**

   Provides the vcpkg executable.

    ```bash
    cd vcpkg && ./bootstrap-vcpkg.sh
    ```

    The shell script bootstraps the vcpkg package manager on Unix-like systems, handles command-line arguments for metrics and binary types, performs prerequisite checks, downloads the vcpkg executable, and manages telemetry settings.

3. **Integrate vcpkg with Bash**
   Adds vcpkg tab-completion support to the current user's `.bashrc`.

    ```bash
    ./vcpkg integrate bash
    ```

    The `vcpkg integrate bash` command sets up bash shell integration for vcpkg.

## Setup the project

1. **Create and navigate to the project directory**

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

4. **Create `CMakeLists.txt` and `main.cpp` files**

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

## Build and run the project

1. Configure the build with CMake:

    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/vcpkg/scripts/buildsystems/vcpkg.cmake
    ```

    Ensure to replace \<vcpkg-root\> with the actual path to the vcpkg directory. The toolchain file teaches cmake where to find your installed libraries.

2. Build the project

    ```bash
    cmake --build build
    ```

3. Run the executable

    ```bash
    ./build/HelloWorld
    ```
