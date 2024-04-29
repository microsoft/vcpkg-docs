# Tutorial: Install and use packages with CMake in Visual Studio

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with CMake, vcpkg, and Visual Studio. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites

- [Visual Studio](<https://visualstudio.microsoft.com/downloads/>) with C++ development workload and [CMake component](<https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170>)
- [Git](<https://git-scm.com/downloads>)

## 1 - Set up vcpkg

### Clone the repository

The first step is to clone the vcpkg repository from GitHub. The repository contains scripts to acquire the vcpkg executable and a registry of curated open-source libraries maintained by the vcpkg community. To do this, run:

```shell
git clone https://github.com/microsoft/vcpkg.git
```
The vcpkg curated registry is a set of over 2,000 open-source libraries. Vcpkg's continuous integration pipelines have validated these libraries to work together. While the vcpkg repository does not contain the source code for these libraries, it holds recipes and metadata to build and install them in your system.

### Run the Bootstrap script

Now that you have cloned the vcpkg repository, navigate to the vcpkg directory and execute the bootstrap script:

#### Powershell or Command Prompt:
```shell-powershell
cd vcpkg && bootstrap-vcpkg.bat
```
or
```shell-powershell
cd vcpkg; .\bootstrap-vcpkg.bat
```

#### Bash:
```shell-bash
cd vcpkg && ./bootstrap-vcpkg.sh
```

The bootstrap script performs prerequisite checks and downloads the vcpkg executable.

That's it! vcpkg is set up and ready to use.

## 2 - Set up the Visual Studio project

1. Create the Visual Studio project

    * Create a new project in Visual Studio using the "CMake Project" template
    * Name your project "helloworld"
    * Check the box for "Place solution and project in the same directory."
    * Click the "Create" button

2. Configure the `VCPKG_ROOT` environment variable.

    ```powershell
    Open the built-in Developer PowerShell window in Visual Studio.
    ```
    
    Run the following commands:

    ```powershell
    $env:VCPKG_ROOT = "C:\path\to\vcpkg"
    $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
    ```
    Open the Developer command prompt in Visual Studio.

    Run the following commands:

    ```console
    set VCPKG_ROOT="C:\path\to\vcpkg"
    set PATH=%VCPKG_ROOT%;%PATH%
    ```
    > Setting environment variables in this manner only affects the current terminal
    > session. To make these changes permanent across all sessions, set them through
    > the Windows System Environment Variables panel.
    
    Setting `VCPKG_ROOT` helps Visual Studio locate your vcpkg instance.
    Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

3. Generate a manifest file and add dependencies.

    Run the following command to create a vcpkg manifest file (`vcpkg.json`):

    ```console
    vcpkg new --application
    ```

    The [`vcpkg new`](<https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/commands/new.md>) command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file in the project's directory.

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

    The generated `vcpkg-configuration.json` file introduces a [baseline](<https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/maintainers/registries.md>) that places minimum version constraints on the project's dependencies. Modifying this file is beyond the scope of this tutorial. While not applicable in this tutorial, keeping the `vcpkg-configuration.json` file under source control is a good practice to ensure version consistency across different development environments.

## 3 - Set up the project files

1. Modify the `helloworld.cpp` file.

    Replace the content of `helloworld.cpp` with the following code:
    
    ```cpp
    #include <fmt/core.h>
    
    int main()
    {
      fmt::print("Hello World!\n");
      return 0;
    }
    ```
    
    This source file includes the `<fmt/core.h>` header which is part of the `fmt` library. The `main()` function calls `fmt::print()` to output the "Hello World!" message to the console.

2. Configure the `CMakePresets.json` file.

   * Rename the `CMakePresets.json` file to `CMakeUserPresets.json`
   * Update its contents as shown below. Replace `<VCPKG_ROOT>` with the path to your vcpkg directory.
  
    ```json
    {
      "version": 2,
      "configurePresets": [
        {
          "name": "default",
          "generator": "Ninja",
          "binaryDir": "${sourceDir}/build",
          "cacheVariables": {
            "CMAKE_TOOLCHAIN_FILE": "<VCPKG_ROOT>/scripts/buildsystems/vcpkg.cmake"
          }
        }
      ]
    }
    ```

    * Since this file includes a hardcoded absolute path, it is recommended that you don't keep this file under source control. If you're using Git, add `CMakeUserPresets.json` to your `.gitignore` file.

    The `CMakeUserPresets.json` file contains a single preset named "default", this preset sets the [`CMAKE_TOOLCHAIN_FILE`](<https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html>) to use vcpkg's CMake toolchain file. This lets vcpkg provide packages to CMake when you configure and build the project. Read the [vcpkg CMake integration documentation](<https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/users/buildsystems/cmake-integration.md>) to learn more.

3. Edit the `CMakeLists.txt` file.

    Replace the contents of the `CMakeLists.txt` file with the following code:

    ```cmake
    cmake_minimum_required(VERSION 3.10)
    project(HelloWorld)
    find_package(fmt CONFIG REQUIRED)
    add_executable(HelloWorld main.cpp)
    target_link_libraries(HelloWorld PRIVATE fmt::fmt)
    ```


    Now, let's break down what each line in the `CMakeLists.txt` file does:

    - `cmake_minimum_required(VERSION 3.10)`: Specifies that the minimum version of CMake required to build the project is 3.10. If the version of CMake installed on your system is lower than this, the build fails.
    - `project(HelloWorld)`: Sets the project's name to "HelloWorld."
    - `find_package(fmt CONFIG REQUIRED)`: Looks for the `fmt` library using its CMake configuration file. The `REQUIRED` keyword ensures an error is generated if the package is not found.
    - `add_executable(HelloWorld helloworld.cpp)`: Adds an executable target named "HelloWorld," built from the source file `helloworld. cpp`.
    - `target_link_libraries(HelloWorld PRIVATE fmt::fmt)`: Specifies the `HelloWorld` executable should link against the `fmt` library. The `PRIVATE` keyword indicates that `fmt` is only needed for building `HelloWorld` and should not propagate to other dependent projects.

## 4 - Build and run the project

1. Build the project.

   Press `Ctrl+Shift+B` to build the project in Visual Studio.

2. Run the application.

## Next steps

To learn more about `vcpkg.json`, see our reference documentation:

- [vcpkg.json](<https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/reference/vcpkg-json.md>)
- [manifest](<https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/concepts/manifest-mode.md>)
