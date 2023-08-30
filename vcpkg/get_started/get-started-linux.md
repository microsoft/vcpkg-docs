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

This tutorial guides you through setting up a new C++ "Hello World" project using the `fmt` library. You'll manage dependencies, build the application, and eventually run your project, all using vcpkg. For this tutorial, you'll need to have `git`, `bash`, `CMake`, and a C++ compiler installed.

## Setup vcpkg

The first step is to clone the vcpkg repository. The repository contains scripts to acquire the vcpkg executable and a registry of open-source packages that is curated and maintained by the vcpkg team and it's community of contributors. To do this, run:

```bash
git clone https://github.com/microsoft/vcpkg.git
```

The curated registry is a set of libraries that have been vetted and packaged to work smoothly with vcpkg. vcpkg uses "ports" to package open-source projects and make them available to install. Ports typically contain:

1. `portfile.cmake`: A CMake script that contains instructions for downloading, building, and installing the library.

2. `vcpkg.json`: A JSON file including package metadata: package version, dependencies, supported platforms and features, etc.

3. Patches: Sometimes a port will contain patches to make the library work with vcpkg or to provide bug fixes in the library itself.

Once you have the repository, navigate to the `vcpkg` directory and execute the bootstrap script:

```bash
cd vcpkg && ./bootstrap-vcpkg.sh
```

The shell script bootstraps the vcpkg package manager, handles command-line arguments for metrics and binary types, performs prerequisite checks, downloads the vcpkg executable, and manages telemetry settings.

You'll want to integrate vcpkg into your bash shell to enjoy tab-completion features. To do this, run:

```bash
./vcpkg integrate bash
```

The `vcpkg integrate bash` command sets up bash shell integration for vcpkg, which adds vcpkg tab-completion support to the current user's `.bashrc`.

## Setup the project

Create a directory for your new project navigate into it:

```bash
cd .. && mkdir helloworld && cd helloworld
```

Initiate a new project within this directory, creating the manifest (`vcpkg.json`) and default configuration files (`vcpkg-configuration.json`):

```bash
../vcpkg/vcpkg new --name helloworld --version 1.0
```

>[!NOTE]
>Ensure the project directory name matches exactly with the name you give to the `vcpkg new` command. This ensures the manifest file lands in the correct directory.

As mentioned above, the `vcpkg new` command will add a `vcpkg.json` file and a `vcpkg-configuration.json` file to your projects directory. The `vcpkg.json` file should look like this:

```json
{
    "name": "helloworld",
    "version": "1.0"
}
```

This `vcpkg.json` file serves as the foundation for your vcpkg-based C++ project. It identifies your project by name and version, setting the stage for additional attributes and dependencies that might be added as your project grows.

Let's dive into each line to understand it's purpose:

* `"name": "helloworld,`: The `"name"` key specifies the name of the project. Here, it's set to `"helloworld"`.
* `"version": "1.0"`: The `"version"` key indicates the version of your project. In this example, the version is set to `"1.0"`. This is valuable for dependency management and understanding the evolution of your project.

 For all things manifest (`vcpkg.json`), check out our reference [documentation](..\reference\vcpkg.json).

The `vcpkg-configuration.json` file serves a different purpose. This file essentially allows you to specify which registry vcpkg should use to look for packages (both by default and additionally). It should look like this:

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

There's a lot going on here, but lets break it down line-by-line starting with the `default-registry` JSON object:

As the name suggests, the `"default-registry"` specifies the default location vcpkg should use to look for packages. The `"default-registry":{` key specifies the default package-registry settings containing details about the registry.

* `"kind": "git",`: Specifies the type of registry. In this case, it's a Git repository.
* `"baseline": "b98e11a16474488633d8a1ab3e160da1f94b98e2",`: Specifies a git-treeish object that points to a set of versions which are likely to work together.
* "`"repository": "https://github.com/microsoft/vcpkg"`: Is the URL of the Git repository to be used as the default registry.

This tutorial will only be using the default registry, but know that you can specify additional registries by adding them to the `registries` array. Note that different kinds of registries will have different attributes. In this case, the `registries` array contains one additional registry that's different from the default. Let's break it down:

* `"kind": "artifact,"`: Specifies the type of this additional registry. In this example, it's an "artifact" type.
* `"location": https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",`: The URL from which this artifact registry can be fetched.
* `"name": "microsoft"`: Specifies the name for this additional registry, which is "microsoft".

Now that the project is properly set up, lets add the `fmt` library as a dependency and generate the project files.

## Adding dependencies and project files

Since this project will use the `fmt` library to print "Hello World", you'll want to add `fmt` to the "dependencies" list in the manifest. To do this, run:

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

Notice that your `vcpkg.json` file now lists `fmt` as a dependency. Now, create the `CMakeLists.txt` and `main.cpp` files:

```bash
touch CMakeLists.txt main.cpp
```

Fill in `CMakeLists.txt` with the necessary information to find and link the `fmt` package. The `CMakeLists.txt` should look like this:

```cmake
cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(fmt CONFIG REQUIRED)

add_executable(HelloWorld main.cpp)

target_link_libraries(HelloWorld PRIVATE fmt::fmt)
```

Likewise, populate `main.cpp` to print a simple "Hello World" using the `fmt` library. The `main.cpp` should look like this:

```cpp
#include <fmt/core.h>

int main(){
fmt::print("Hello World!\n");
return 0;
}
```

## Build and execute the project

Configure the build using CMake. Remember to point to the correct location of your vcpkg directory by updating the `<vcpkg-root>` placeholder:

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/vcpkg/scripts/buildsystems/vcpkg.cmake
```

The toolchain file teaches cmake where to find your installed libraries. Next, proceed to build the project:

```bash
cmake --build build
```

Finally, run the executable to see your application in action:

```bash
./build/HelloWorld
```
