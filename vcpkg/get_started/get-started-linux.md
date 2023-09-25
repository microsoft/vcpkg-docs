---
title: Install and use packages on Linux with vcpkg
description: Tutorial guides the user through the process of installing and using packages on Linux with vcpkg.
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 08/23/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to install and manage packages on a Linux environment using vcpkg, so that I can easily set up and maintain C++ projects with necessary dependencies.
---

# Tutorial: Install and use packages on Linux with vcpkg

This tutorial shows you how to set up a C++ "Hello World" CMake project using the `fmt` library. Manage dependencies, construct the application, and execute the project with the help of vcpkg. This guide is tailored for a Linux environment.

## Prerequisites

- A terminal to work in.
- `git`, `CMake`, and a C++ compiler installed on your system
- vcpkg installed on your system and on your PATH

## 1 - Set up the project

1. Create the project directory

    This tutorial assumes your project is located in `/home/helloworld`. If you intend to place your project somewhere else, replace any references to that path with your preferred path.

    Create a directory named "helloworld" to contain your project files, put it right next to your vcpkg directory. Then, navigate to your project directory.

    ```bash
    mkdir helloworld && cd helloworld
    ```

2. Create the manifest

    Next, create a file named `vcpkg.json` in your project's directory. This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install.

    From within the `helloworld` directory, run:

    ```bash
    vcpkg new --application
    ```

    The `vcpkg new` command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file to your projects directory. The `vcpkg.json` file should look like this:

    ```json
    {}
    ```

    The `vcpkg.json` file serves as the foundation to integrate vcpkg in your C++ project. Initially, the file contains an empty JSON object; additional attributes like dependencies and supported features can be added as your project grows.

    The `vcpkg-configuration.json` file serves a different purpose. This file allows full control over your dependencies' sources and it's out of scope for this tutorial.

    For this tutorial, all the dependencies come from the vcpkg registry at <https://github.com/Microsoft/vcpkg>. When there's no explicit configuration provided, vcpkg uses this registry as default.

## 2 - Add dependencies and project files

Now that the project is set up, add the `fmt` library as a dependency and generate the project files.

1. Add `fmt` dependency

    To do so, run:

    ```bash
    ..\vcpkg\vcpkg add port fmt
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

    Create the `CMakeLists.txt` and `main.cpp` files:

    ```bash
    touch CMakeLists.txt main.cpp
    ```

    Fill in `CMakeLists.txt` with the necessary information to find and link the `fmt` package. The `CMakeLists.txt` should look like this:

    :::code language="cmake" source="../examples/snippets/get-started-linux/CMakeLists.txt":::

    Likewise, populate `main.cpp` to print a simple "Hello World" using the `fmt` library. The `main.cpp` should look like this:

    :::code language="cpp" source="../examples/snippets/get-started-linux/main.cpp":::

## 3 - Build and run the project

1. Run CMake configuration

    Configure the build using CMake. Update the `<vcpkg-root>` placeholder with the directory you installed vcpkg:

    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/vcpkg/scripts/buildsystems/vcpkg.cmake
    ```

    The vcpkg.cmake toolchain file allows the CMake project system to recognize C++ libraries provided by vcpkg. This makes it easy to use these libraries in your project with minimal effort.

2. Build the project

    Next, proceed to build the project:

    ```bash
    cmake --build build
    ```

3. Run the application

    Finally, run the executable to see your application in action:

    ```bash
    ./build/HelloWorld
    
    Hello World!
    ```

## Next steps

To learn more about `vcpkg.json`, check out our reference documentation:

- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
