---
title: Install and use packages with vcpkg in Visual Studio
description: Tutorial guides the user through the process of installing and using packages with vcpkg and Visual Studio.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 09/27/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to install and manage packages using vcpkg, so that I can easily set up and maintain C++ projects with necessary dependencies.
---

# Tutorial: Install and use packages with vcpkg

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake, vcpkg and Visual Studio. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- [Visual Studio with C++ development workload](https://visualstudio.microsoft.com/downloads/)
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)

[!INCLUDE [setup-vcpkg](setup-vcpkg.md)]


## 2 - Set up the Visual Studio project

1. Create the project directory

    We'll be working within a directory named `helloworld`. Ensure that this directory is a sibling of your vcpkg directory, meaning both directories should reside under the same parent directory.
    
    * Open Visual Studio
    * Click on "Create a new project."
    * Choose "Empty Project" as the template.
    * Name your project "helloworld."
    * Set the location to be the parent directory of your vcpkg installation.
    * Check the box for "Place solution and project in the same directory."
    * Click the "Create."
  
2. Generate a Manifest file

    Create a vcpkg manifest file (`vcpkg.json`) by executing the following command in Visual Studio's built-in terminal:

    ```console
    ../vcpkg/vcpkg new --application
    ```

    The `vcpkg new` command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file to your projects directory. To view the manifest file (`vcpkg.json`) toggle the "Folder view" in the solution explorer.

    The `vcpkg.json` file should look like this:

    ```json
    {}
    ```

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install and integrates with CMake to provide the dependencies required by your project. Initially, the file contains an empty JSON object; additional attributes like dependencies and supported features can be added as your project grows.

    The default `vcpkg-configuration.json` file introduces [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) constraints, specifying the minimum versions of dependencies that your project should use. While this allows for more granular control over your project's dependencies, a detailed discussion is beyond the scope of this tutorial.

## 3 - Add dependencies and create project files

Now that the project is set up, add the `fmt` library as a dependency and generate the project files.

1. Add the `fmt` dependency

    From within Visual Studio's built-in terminal, run:

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

    Create the `main.cpp` file with the following content:

    :::code language="cpp" source="../examples/snippets/get-started/main.cpp":::

    In this `main.cpp` file, the `<fmt/core.h>` header is included for using the `fmt` library. The `main()` function then utilizes `fmt::print()` to output the "Hello World!" message to the console.

    Create the `CMakeLists.txt` file with the following content:

    :::code language="cmake" source="../examples/snippets/get-started/CMakeLists.txt":::

    Now, let's break down what each line in the `CMakeLists.txt` file does:

    - `cmake_minimum_required(VERSION 3.10)`: Specifies that the minimum version of CMake required to build the project is 3.10. If the version of CMake installed on your system is lower than this, an error will be generated.
    - `project(HelloWorld)`: Sets the name of the project to "HelloWorld."
    - `find_package(fmt CONFIG REQUIRED)`: Looks for the `fmt` library using its CMake configuration file. The `REQUIRED` keyword ensures that an error is generated if the package is not found.
    - `add_executable(HelloWorld main.cpp)`: Adds an executable target named "HelloWorld," built from the source file `main.cpp`.
    - `target_link_libraries(HelloWorld PRIVATE fmt::fmt)`: Specifies that the `HelloWorld` executable should link against the `fmt` library. The `PRIVATE` keyword indicates that `fmt` is only needed for building `HelloWorld` and should not propagate to other dependent projects.

## 4 - Build and run the project

1. Build
   
   Press `Ctrl+Shift+B` to build the project in Visual Studio.
   

2. Run the application

    Finally, run the executable:

    You should see the output:

    ```console
    Hello world!
    ```


## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
