---
title: Install and use packages with CMake in Visual Studio
description: Tutorial guides the user through the process of installing and using packages with CMake and Visual Studio.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 09/27/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to install and manage packages using CMake and Visual Studio, so that I can easily set up and maintain C++ projects with necessary dependencies.
---

# Tutorial: Install and use packages with CMake in Visual Studio

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake, vcpkg and Visual Studio. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- [Visual Studio](<https://visualstudio.microsoft.com/downloads/>) with C++ development workload and [CMake component](</cpp/build/cmake-projects-in-visual-studio#installation>)
- [Git](<https://git-scm.com/downloads>)

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

## 2 - Set up the Visual Studio project

1. Create the Visual Studio project
    
    * Create a new project in Visual Studio using the "CMake Project" template
    :::image type="complex" source="../resources/get_started/visual-studio-create-project.png" alt-text="create a new CMake project":::
        Screenshot of the Visual Studio UI for showing how to create a new CMake project in Visual Studio
    :::image-end:::
    * Name your project "helloworld"
    * Check the box for "Place solution and project in the same directory."
    * Click the "Create" button
    :::image type="complex" source="../resources/get_started/visual-studio-name-project.png" alt-text="naming your CMake project":::
        Screenshot of Visual Studio UI for naming your CMake project and clicking the "create" button.
    :::image-end:::

2. Configure the `VCPKG_ROOT` environment variable.
   
    ::: zone pivot="shell-powershell"
    Open the built-in Developer PowerShell window in Visual Studio.
    
    :::image type="complex" source="../resources/get_started/visual-studio-developer-powershell.png" alt-text="opening built-in developer powershell":::
        Screenshot of Visual Studio UI for the built-in PowerShell developer window
    :::image-end:::

    Run the following commands:

    ```PowerShell
    $env:VCPKG_ROOT = "C:\path\to\vcpkg"
    $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
    ```

    :::image type="complex" source="../resources/get_started/visual-studio-environment-variable-setup-powershell.png" alt-text="setting up your environment variables":::
        Screenshot of Visual Studio UI for the built-in PowerShell developer window showing how to set up VCPKG_ROOT and and add it to PATH.
    :::image-end:::
    ::: zone-end
    ::: zone pivot="shell-cmd"
    Open the Developer command prompt in Visual Studio.
    
    :::image type="complex" source="../resources/get_started/visual-studio-developer-cmd.png" alt-text="opening Visual Studio developer command prompt.":::
        Screenshot of Visual Studio UI for developer command prompt.
    :::image-end:::

    Run the following commands:

    ```console
    set VCPKG_ROOT="C:\path\to\vcpkg"
    set PATH=%VCPKG_ROOT%;%PATH%
    ```

    :::image type="complex" source="../resources/get_started/visual-studio-environment-variable-setup-cmd.png" alt-text="setting up your environment variables":::
        Screenshot of Visual Studio developer command prompt showing how to set up VCPKG_ROOT and and add it to PATH.
    :::image-end:::
    ::: zone-end

    Setting `VCPKG_ROOT` helps Visual Studio locate your vcpkg instance.
    Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

1. Generate a manifest file and add dependencies.

    Run the following command to create a vcpkg manifest file (`vcpkg.json`):

    ```console
    vcpkg new --application
    ```

    The [`vcpkg new`](../commands/new.md) command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file in the project's directory.

    Add the `fmt` package as a dependency:

    ```console
    vcpkg add port fmt
    ```

    Your `vcpkg.json` should now contain:

    ```json
    {
        "dependencies": [
            "fmt"
        ]
    }
    ```

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install and integrates with CMake to provide the dependencies required by your project.

    The generated `vcpkg-configuration.json` file introduces a [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) that places [minimum version constraints](../users/versioning.md) on the project's dependencies. Modifying this file is beyond the scope of this tutorial. While not applicable in this tutorial, it's a good practice to keep the `vcpkg-configuration.json` file under source control to ensure version consistency across different development environments.

## 3 - Set up the project files

1. Modify the `helloworld.cpp` file.

    Replace the content of `helloworld.cpp` with the following code:

    :::code language="cpp" source="../examples/snippets/get-started/main.cpp":::

    This source file includes the `<fmt/core.h>` header which is part of the `fmt` library. The `main()` function calls `fmt::print()` to output the "Hello World!" message to the console.

2. Configure the `CMakePresets.json` file.
   
   * Rename the `CMakePresets.json` file to `CMakeUserPresets.json`
   * Update its contents as shown below. Replace `<VCPKG_ROOT>` with the path to your vcpkg directory.
  
    ```json
    {
    "version": 3,
    "configurePresets": [
        {
        "name": "default",
        "cacheVariables": {
            "CMAKE_TOOLCHAIN_FILE": "<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake"
        }
        }
    ]
    }
    ```

    * Since this file includes a hardcoded absolute path, it is recommended that you don't keep this file under source control. If you're using Git, add `CMakeUserPresets.json` to your `.gitignore` file.

    The `CMakeUserPresets.json` file contains a single preset named "default", this preset sets the [`CMAKE_TOOLCHAIN_FILE`](<https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html>) to use vcpkg's CMake toolchain file. This lets vcpkg provide packages to CMake when you configure and build the project. Read the [vcpkg CMake integration documentation](../users/buildsystems/cmake-integration.md) to learn more.

3. Edit the `CMakeLists.txt` file.
   
    Replace the contents of the `CMakeLists.txt` file with the following code:

    :::code language="cmake" source="../examples/snippets/get-started/CMakeLists.txt":::

    Now, let's break down what each line in the `CMakeLists.txt` file does:

    - `cmake_minimum_required(VERSION 3.10)`: Specifies that the minimum version of CMake required to build the project is 3.10. If the version of CMake installed on your system is lower than this, the build fails.
    - `project(HelloWorld)`: Sets the name of the project to "HelloWorld."
    - `find_package(fmt CONFIG REQUIRED)`: Looks for the `fmt` library using its CMake configuration file. The `REQUIRED` keyword ensures that an error is generated if the package is not found.
    - `add_executable(HelloWorld helloworld.cpp)`: Adds an executable target named "HelloWorld," built from the source file `helloworld.cpp`.
    - `target_link_libraries(HelloWorld PRIVATE fmt::fmt)`: Specifies that the `HelloWorld` executable should link against the `fmt` library. The `PRIVATE` keyword indicates that `fmt` is only needed for building `HelloWorld` and should not propagate to other dependent projects.

## 4 - Build and run the project

1. Build the project.
   
   Press `Ctrl+Shift+B` to build the project in Visual Studio.
   

2. Run the application.

    Finally, run the executable:
    :::image type="complex" source="../resources/get_started/visual-studio-run-project.png" alt-text="Running the executable":::
        Screenshot of Visual Studio UI for running the executable.
    :::image-end:::

    You should see the output:
    :::image type="complex" source="../resources/get_started/visual-studio-helloworld-output.png" alt-text="Program output":::
        Screenshot of the program outputs - "Hello World!"
    :::image-end:::

## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
