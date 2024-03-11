---
title: Install and use packages with CMake
description: Tutorial guides the user through the process of installing and using packages with CMake.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 01/10/2024
#CustomerIntent: As a beginner C++ developer, I want to learn how to install and manage packages using CMake and vcpkg, so that I can easily set up and maintain C++ projects with necessary dependencies.
---

# Tutorial: Install and use packages with vcpkg

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake and vcpkg. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- A terminal
- A C++ compiler
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)

::: zone pivot="shell-cmd, shell-powershell"

> [!NOTE]
> For Windows users, Visual Studio's MSVC (Microsoft Visual C++ Compiler) is the required compiler for C++ development.

::: zone-end

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

## 2 - Set up the project

1. Configure the `VCPKG_ROOT` environment variable.

    ::: zone pivot="shell-bash"

    ```bash
    export VCPKG_ROOT=/path/to/vcpkg
    export PATH=$VCPKG_ROOT:$PATH
    ```

    > [!NOTE]
    > Setting the `VCPKG_ROOT` environment variable using the `export` command only affects the current shell session. To make this change permanent across sessions, you'll need to add the `export` command to your shell's profile script (e.g., `~/.bashrc` or `~/.zshrc`).

    ::: zone-end

    ::: zone pivot="shell-cmd"

    ```console
    set VCPKG_ROOT="C:\path\to\vcpkg"
    set PATH=%VCPKG_ROOT%;%PATH%
    ```

    > [!NOTE]
    > Setting the `VCPKG_ROOT` environment variable using the `set` command only affects the current shell session. To make this change permanent across sessions, you can use the `setx` command and restart the shell session.

    ::: zone-end
    ::: zone pivot="shell-powershell"

    ```powershell
    $env:VCPKG_ROOT = "C:\path\to\vcpkg"
    $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
    ```

    > [!NOTE]
    > Setting the `VCPKG_ROOT` and updating the `PATH` environment variables in this manner only affects the current PowerShell session. To make these changes permanent across all sessions, you should add them to your PowerShell profile or set them through the Windows System Environment Variables panel.
    ::: zone-end

    Setting `VCPKG_ROOT` tells vcpkg where your vcpkg instance is located.
    Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

2. Create the project directory.

    ```console
    mkdir helloworld && cd helloworld
    ```

## 3 - Add dependencies and project files

1. Create the manifest file and add the `fmt` dependency.

    First, create a manifest file (`vcpkg.json`) in your project's directory by running the [`vcpkg new`](../commands/new.md) command from within the `helloworld` directory:

    ```console
    vcpkg new --application
    ```

    Next, add the `fmt` dependency:

    ```console
    vcpkg add port fmt
    ```

    Your `vcpkg.json` should look like this:

    ```json
    {
        "dependencies": [
            "fmt"
        ]
    }
    ```

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install and integrates with CMake to provide the dependencies required by your project.

    The default `vcpkg-configuration.json` file introduces [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) constraints, specifying the minimum versions of dependencies that your project should use. While modifying this file is beyond the scope of this tutorial, it plays a crucial role in defining version constraints for your project's dependencies. Therefore, even though it's not strictly necessary for this tutorial, it's a good practice to add `vcpkg-configuration.json` to your source control to ensure version consistency across different development environments.

2. Create the project files.

    Create the `CMakeLists.txt` file with the following content:

    :::code language="cmake" source="../examples/snippets/get-started/CMakeLists.txt":::

    Now, let's break down what each line in the `CMakeLists.txt` file does:

    - `cmake_minimum_required(VERSION 3.10)`: Specifies that the minimum version of CMake required to build the project is 3.10. If the version of CMake installed on your system is lower than this, an error will be generated.
    - `project(HelloWorld)`: Sets the name of the project to "HelloWorld."
    - `find_package(fmt CONFIG REQUIRED)`: Looks for the `fmt` library using its CMake configuration file. The `REQUIRED` keyword ensures that an error is generated if the package is not found.
    - `add_executable(HelloWorld main.cpp)`: Adds an executable target named "HelloWorld," built from the source file `main.cpp`.
    - `target_link_libraries(HelloWorld PRIVATE fmt::fmt)`: Specifies that the `HelloWorld` executable should link against the `fmt` library. The `PRIVATE` keyword indicates that `fmt` is only needed for building `HelloWorld` and should not propagate to other dependent projects.

    Create the `helloworld.cpp` file with the following content:

    :::code language="cpp" source="../examples/snippets/get-started/main.cpp":::

    In this `helloworld.cpp` file, the `<fmt/core.h>` header is included for using the `fmt` library. The `main()` function then calls `fmt::print()` to output the "Hello World!" message to the console.

## 4 - Build and run the project

1. Run CMake configuration

    To allow the CMake project system to recognize C++ libraries provided by vcpkg, you'll need to provide the `vcpkg.cmake` toolchain file. To automate this, create a `CMakePresets.json` file in the "helloworld" directory with the following content:

    :::code language="cmake" source="../examples/snippets/get-started/CMakePresets.json":::

    This `CMakePresets.json` file contains a single "default" preset for CMake and sets the `CMAKE_TOOLCHAIN_FILE` variable. The `CMAKE_TOOLCHAIN_FILE` allows the CMake project system to recognize C++ libraries provided by vcpkg. Adding the `CMakePresets.json` automates the process of specifying the toolchain when running CMake.

    Configure the build using CMake:

    ```console
    cmake --preset=default
    ```

2. Build the project

    Run:

    ```console
    cmake --build build
    ```
    
3. Run the application

    Finally, run the executable to see your application in action:

    ::: zone pivot="shell-bash"
    ```console
    ./build/HelloWorld
    
    Hello World!
    ```
    ::: zone-end
   
    ::: zone pivot="shell-cmd, shell-powershell"
    ```console
    .\build\HelloWorld.exe
    
    Hello World!
    ```
    ::: zone-end   

## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [Packaging a library](get-started-packaging.md)
- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\concepts\manifest-mode.md)
