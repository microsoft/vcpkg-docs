---
title: Install and use packages with CMake in Visual Studio
description: Tutorial guides the user through the process of installing and using packages with CMake and Visual Studio.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 09/27/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to install and manage packages using CMake and Visual Studio, so that I can easily set up and maintain C++ projects with necessary dependencies.
---

# Tutorial: Install and use packages with vcpkg

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake, vcpkg and Visual Studio. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- [Visual Studio with C++ development workload and CMake component](https://visualstudio.microsoft.com/downloads/)
- [Git](https://git-scm.com/downloads)

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](setup-vcpkg.md)]

## 2 - Set up the Visual Studio project

1. Create the Visual Studio project
    
    * Open Visual Studio
    * Click on "Create a new project"
    * Choose "CMake Project" as the template.
    * Name your project "helloworld"
    * Check the box for "Place solution and project in the same directory."
    * Click the "Create"

2. Configure the `VCPKG_ROOT` environment variable.
   
    Open the built-in Developer PowerShell window (``Ctrl+` ``) in Visual Studio and run the following commands:

    ```console
    $env:VCPKG_ROOT = "C:\path\to\vcpkg"
    $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
    ```

    Setting `VCPKG_ROOT` helps Visual Studio locate where your vcpkg instance is located.
    Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

3. Generate a Manifest file and add dependencies.

    Run the following command to create a vcpkg manifest file (`vcpkg.json`):

    ```console
    vcpkg new --application
    ```

    The command adds a `vcpkg.json` and a `vcpkg-configuration.json` file to your project directory. 

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

    The default `vcpkg-configuration.json` file introduces [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) constraints, specifying the minimum versions of dependencies that your project should use. While modifying this file is beyond the scope of this tutorial, it plays a crucial role in defining version constraints for your project's dependencies. Therefore, even though it's not strictly necessary for this tutorial, it's a good practice to add `vcpkg-configuration.json` to your source control to ensure version consistency across different development environments.

## 3 - Set up the project files

1. Modify the `helloworld.cpp` file.

    Replace the content of `helloworld.cpp` with the following code:

    :::code language="cpp" source="../examples/snippets/get-started/main.cpp":::

    In this `helloworld.cpp` file, the `<fmt/core.h>` header is included for using the `fmt` library. The `main()` function then calls `fmt::print()` to output the "Hello World!" message to the console.

2. Configure the `CMakePresets.json` file.
   
   * Rename the `CMakePresets.json` to `CMakeUserPresets.json`
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

  * Add `CMakeUserPresets.json` to you `.gitignore` file if you're using version control. 
  
   This `CMakeUserPresets.json` file contains a single "default" preset for CMake and sets the `CMAKE_TOOLCHAIN_FILE` variable. The `CMAKE_TOOLCHAIN_FILE` allows the CMake project system to recognize C++ libraries provided by vcpkg. Adding the `CMakeUserPresets.json` automates the process of specifying the toolchain when running CMake.

3. Edit the `CMakeLists.txt` file.
   
    Replace the contents of the `CMakeLists.txt` file with the following code:

    :::code language="cmake" source="../examples/snippets/get-started/CMakeLists.txt":::

    Now, let's break down what each line in the `CMakeLists.txt` file does:

    - `cmake_minimum_required(VERSION 3.10)`: Specifies that the minimum version of CMake required to build the project is 3.10. If the version of CMake installed on your system is lower than this, an error will be generated.
    - `project(HelloWorld)`: Sets the name of the project to "HelloWorld."
    - `find_package(fmt CONFIG REQUIRED)`: Looks for the `fmt` library using its CMake configuration file. The `REQUIRED` keyword ensures that an error is generated if the package is not found.
    - `add_executable(HelloWorld helloworld.cpp)`: Adds an executable target named "HelloWorld," built from the source file `helloworld.cpp`.
    - `target_link_libraries(HelloWorld PRIVATE fmt::fmt)`: Specifies that the `HelloWorld` executable should link against the `fmt` library. The `PRIVATE` keyword indicates that `fmt` is only needed for building `HelloWorld` and should not propagate to other dependent projects.

## 4 - Build and run the project

1. Build the project.
   
   Press `Ctrl+Shift+B` to build the project in Visual Studio.
   

2. Run the application.

    Finally, run the executable:

    You should see the output:

    ```console
    Hello world!
    ```


## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
