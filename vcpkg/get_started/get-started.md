---
title: Install and use packages with vcpkg
description: Tutorial guides the user through the process of installing and using packages with vcpkg.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 08/23/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to install and manage packages using vcpkg, so that I can easily set up and maintain C++ projects with necessary dependencies.
---

# Tutorial: Install and use packages with vcpkg

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake and vcpkg. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- A terminal to work in.
- A C++ compiler
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)

[!INCLUDE [setup-vcpkg](setup-vcpkg.md)]


## 2 - Set up the project

1. Create the project directory

    This tutorial assumes that your project will be located in a directory named `helloworld`. The `helloworld` directory should be a sibling (or peer) of your vcpkg directory, meaning both should reside under the same parent directory. If you intend to place your project somewhere else, replace any references to that path with your chosen path.
    
    To create the directory and navigate into it, run:

    ```console
    mkdir helloworld && cd helloworld
    ```

2. Create the manifest

    Create a manifest file (`vcpkg.json`) in your project's directory by running the [`vcpkg new`](../commands/new.md) command from within the `helloworld` directory:

    ```console
    ../vcpkg/vcpkg new --application
    ```

    The `vcpkg new` command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file to your projects directory. The `vcpkg.json` file should look like this:

    ```json
    {}
    ```

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install and integrates with CMake to provide the dependencies required by your project. Initially, the file contains an empty JSON object; additional attributes like dependencies and supported features can be added as your project grows.

    The default `vcpkg-configuration.json` file introduces [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) constraints, specifying the minimum versions of dependencies that your project should use. While this allows for more granular control over your project's dependencies, a detailed discussion is beyond the scope of this tutorial.

## 3 - Add dependencies and project files

Now that the project is set up, add the `fmt` library as a dependency and generate the project files.

1. Add the `fmt` dependency

    Run:

    ```console
    ../vcpkg/vcpkg add port fmt
    ```

    Your `vcpkg.json` should look like this:

    ```json
    {
        "dependencies": [
        "fmt"
        ]
    }
    ```

2. Create project files

    Create the `CMakeLists.txt` file with the following content:

    :::code language="cmake" source="../examples/snippets/get-started/CMakeLists.txt":::

    Now, let's break down what each line in the `CMakeLists.txt` file does:

    - `cmake_minimum_required(VERSION 3.10)`: Specifies that the minimum version of CMake required to build the project is 3.10. If the version of CMake installed on your system is lower than this, an error will be generated.
    - `project(HelloWorld)`: Sets the name of the project to "HelloWorld."
    - `find_package(fmt CONFIG REQUIRED)`: Looks for the `fmt` library using its CMake configuration file. The `REQUIRED` keyword ensures that an error is generated if the package is not found.
    - `add_executable(HelloWorld main.cpp)`: Adds an executable target named "HelloWorld," built from the source file `main.cpp`.
    - `target_link_libraries(HelloWorld PRIVATE fmt::fmt)`: Specifies that the `HelloWorld` executable should link against the `fmt` library. The `PRIVATE` keyword indicates that `fmt` is only needed for building `HelloWorld` and should not propagate to other dependent projects.

    Create the `main.cpp` file with the following content:

    :::code language="cpp" source="../examples/snippets/get-started/main.cpp":::

    In this `main.cpp` file, the `<fmt/core.h>` header is included for using the `fmt` library. The `main()` function then utilizes `fmt::print()` to output the "Hello World!" message to the console.

## 4 - Build and run the project

1. Run CMake configuration

    To allow the CMake project system to recognize C++ libraries provided by vcpkg, you'll need to provide the `vcpkg.cmake` toolchain file. 

    First, set the `VCPKG_ROOT` environment variable for the current shell session:

    ::: zone pivot="os-linux,os-macos"

    ```console
    export VCPKG_ROOT=/path/to/your/vcpkg/directory
    ```

    > [!NOTE]
    > Setting the `VCPKG_ROOT` using the `export` command will only affect the current shell session. To make this change permanent across sessions, you'll need to add the `export` command to your shell's profile script (e.g., `~/.bashrc` or `~/.zshrc`).

    ::: zone-end

    ::: zone pivot="os-windows"

    ```console
    set VCPKG_ROOT="/path/to/your/vcpkg/directory"
    ```

    > [!NOTE]
    > Setting the `VCPKG_ROOT` environment variable using the `set` command will only affect the current shell session. To make this change permanent across sessions, you can use the `setx` command and restart the shell session.

    ::: zone-end


    Next, create a `CMakePresets.json` file in the "helloworld" directory with the following content:

    :::code language="cmake" source="../examples/snippets/get-started/CMakePresets.json":::

    This `CMakePresets.json` file contains a single "default" preset for CMake. It uses version 3 of the schema and sets the `CMAKE_TOOLCHAIN_FILE` variable, pointing to a toolchain file in the `VCPKG_ROOT` environment variable. This automates the process of specifying the toolchain when running CMake.

    Finally, configure the build using CMake:

    ```console
    cmake -B build -S . --presets=default
    ```

    This will use the settings in the `CMakePresets.json` file, including the specified toolchain file, to configure the project.

2. Build the project

    Run:

    ```console
    cmake --build build
    ```

3. Run the application

    Finally, run the executable to see your application in action:

    ```console
    ./build/HelloWorld
    
    Hello World!
    ```

## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
