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

In this tutorial you'll create a C++ "Hello World" CMake project using vcpkg and the fmt library. You'll manage dependencies and build and run a simple application.

## Prerequisites

- A terminal to work in.
- `git`, `CMake`, and a C++ compiler installed on your system
- vcpkg installed on your system

## 1 - Set up the project

1. Create the project directory

    This tutorial assumes that your project will be located in a directory named `helloworld`. The `helloworld` directory should be a sibling (or peer) of your vcpkg directory, meaning both should reside under the same parent directory. If you intend to place your project somewhere else, replace any references to that path with your preferred path.
    
    To create the directory and navigate into it, run:

    ```console
    mkdir helloworld && cd helloworld
    ```

2. Create the manifest

    In this step, you'll create a file named `vcpkg.json` in your project's directory. 
    
    From within the `helloworld` directory, run:

    ```console
    ../vcpkg/vcpkg new --application
    ```

    The `vcpkg new` command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file to your projects directory. The `vcpkg.json` file should look like this:

    ```json
    {}
    ```

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install. The manifest integrates vcpkg into your C++ project by providing a list of dependencies and supported features that are part of your project. Initially, the file contains an empty JSON object; additional attributes like dependencies and supported features can be added as your project grows.

    The `vcpkg-configuration.json` file serves a different purpose. This file allows full control over your dependencies' sources and it's out of scope for this tutorial.

## 2 - Add dependencies and project files

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

## 3 - Build and run the project

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
