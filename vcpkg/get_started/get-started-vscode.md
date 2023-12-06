---
title: Install and use packages with CMake in Visual Studio Code
description: Tutorial guides the user through the process of installing and using packages with CMake and Visual Studio Code.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
author: data-queue
ms.author: danshaw2
ms.date: 12/06/2023
ms.prod: vcpkg
ms.topic: tutorial
---

# Tutorial: Install and use packages with CMake in VS Code

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake, vcpkg and Visual Studio Code. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- [VS Code](https://code.visualstudio.com)
- [C++ compiler](https://code.visualstudio.com/docs/languages/cpp#_install-a-compiler)
- Windows 7 or newer

This example uses the C++ MSVC compiler in the Visual Studio C++ development workload.

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

## 2 - Install VS Code Extensions

Navigate to the Extension view, and install the [C++ Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools). This enables C++ IntelliSense and code navigation.

:::image type="complex" source="../resources/get_started/vscode-c-extension.png" alt-text="installing C++ VS Code Extension":::
  Screenshot of VS Code Extension view with C++ Extension
:::image-end:::

Install the [CMake Tools Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools). This enables CMake support in VS Code.

:::image type="complex" source="../resources/get_started/vscode-cmake-extension.png" alt-text="installing CMake Tools VS Code Extension":::
  Screenshot of VS Code Extension view with CMake Tools Extension
:::image-end:::

## 3 - Setup vcpkg

1. Configure the `VCPKG_ROOT` environmental variable.

  Open a new Terminal in VS Code: **Terminal > New Terminal**

  Run the following commands:
  ::: zone pivot="shell-powershell"
  ```PowerShell
  $env:VCPKG_ROOT = "C:\path\to\vcpkg"
  $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
  ```

  :::image type="complex" source="../resources/get_started/vscode-terminal-vcpkg.png" alt-text="setting up vcpkg environment variables":::
    Screenshot of setting up VCPKG_ROOT and adding it to PATH in a VS Code terminal.
  :::image-end:::

  ::: zone-end
  ::: zone pivot="shell-cmd"

  ```console
  set VCPKG_ROOT="C:\path\to\vcpkg"
  set PATH=%VCPKG_ROOT%;%PATH%
  ```
  ::: zone-end
  ::: zone pivot="shell-cmd"
  ```console
  VCPKG_ROOT=/c/path/to/vcpkg
  PATH=$PATH:$VCPKG_ROOT
  ```
  ::: zone-end

  Setting `VCPKG_ROOT` helps Visual Studio locate your vcpkg instance.
  Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.
2. Generate a manifest file and add dependencies.

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

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install and integrates with MSBuild to provide the dependencies required by your project.

    The generated `vcpkg-configuration.json` file introduces a [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) that places [minimum version constraints](../users/versioning.md) on the project's dependencies. Modifying this file is beyond the scope of this tutorial. While not applicable in this tutorial, it's a good practice to keep the `vcpkg-configuration.json` file under source control to ensure version consistency across different development environments.

## 3 - Set up the project files

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

To allow the CMake project system to recognize C++ libraries provided by vcpkg, you'll need to provide the `vcpkg.cmake` toolchain file. To automate this, create a `CMakePresets.json` file in the "helloworld" directory with the following content:

:::code language="cmake" source="../examples/snippets/get-started/CMakePresets.json":::

This `CMakePresets.json` file contains a single "default" preset for CMake and sets the `CMAKE_TOOLCHAIN_FILE` variable. The `CMAKE_TOOLCHAIN_FILE` allows the CMake project system to recognize C++ libraries provided by vcpkg. Adding the `CMakePresets.json` automates the process of specifying the toolchain when running CMake.

4 - Build and run the project

1. Run the `CMake: Build` command the project by navigating to the Command Palette in **View > Command Palette**

:::image type="complex" source="../resources/get_started/vscode-command-build.png" alt-text="CMake build command in VS Code":::
  Screenshot of selecting the CMake build command in VS Code.
:::image-end:::

Select the `default` CMake preset. This enables the vcpkg toolchain.

:::image type="complex" source="../resources/get_started/vscode-command-build-preset.png" alt-text="Selecting preset in CMake build command in VS Code":::
  Screenshot of selecting the preset in the CMake build command in VS Code.
:::image-end:::

2. Launch the project

You should see the output:

```
Hello World!
```

## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [Packaging a library](get-started-packaging.md)
- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
