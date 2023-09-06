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

This tutorial guides you through setting up a new C++ "Hello World" project using the `fmt` library. You'll manage dependencies, build the application, and eventually run your project, all using vcpkg. This tutorial assumes you have vcpkg setup and ready to use. If not, check out our initial setup [tutorial](setup-vcpkg.md).

### Prerequisites

For this tutorial, you'll need: a terminal, `git`, `cmake`, and a C++ compiler.

## Step 1: Setup the project

### Step 1.1: Create the project directory

Create a directory for your new project next to your vcpkg clone and navigate into it:

```bash
mkdir helloworld && cd helloworld
```

### Step 1.2 Create the manifest

From within the `helloworld` directory, run:

```bash
..\vcpkg\vcpkg new --name helloworld --version 1.0
```

>[!NOTE]
>Ensure the project directory name matches exactly with the name you give to the `vcpkg new` command. This will ensure subsequent commands work smoothly.

As mentioned above, the `vcpkg new` command will add a `vcpkg.json` file and a `vcpkg-configuration.json` file to your projects directory. The `vcpkg.json` file should look like this:

```json
{
    "name": "helloworld",
    "version": "1.0"
}
```

The `vcpkg.json` file serves as the foundation to integrate vcpkg in your C++ project. Initially, it contains your project's name and version; additional attributes like dependencies and supported features can be added as your project grows.

To learn more about `vcpkg.json`, check out our reference [documentation](..\reference\vcpkg-json.md).

### Step 1.3 Remove configuration file

The `vcpkg-configuration.json` file serves a different purpose. This file allows full control over your dependencies' sources and it's out of scope for this tutorial.

For this tutorial, our dependencies all come from the vcpkg registry at <https://github.com/Microsoft/vcpkg>. When there's no explicit configuration provided, vcpkg uses this registry as default. So the next step is to delete this file.

```bash
rm vcpkg-configuration.json
```

## Step 2: Add dependencies and project files

Now that the project is properly set up, lets add the `fmt` library as a dependency and generate the project files.

### Step 2.1: Add fmt dependency

Since this project will use the `fmt` library to print "Hello World", you'll want to add `fmt` to the "dependencies" list in the manifest. To do this, run:

```bash
..\vcpkg\vcpkg add port fmt
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

Notice that your `vcpkg.json` file now lists `fmt` as a dependency.

### Step 2.2: Create project files

Now, create the `CMakeLists.txt` and `main.cpp` files:

```bash
touch CMakeLists.txt main.cpp
```

Fill in `CMakeLists.txt` with the necessary information to find and link the `fmt` package. The `CMakeLists.txt` should look like this:

:::code language="cmake" source="../examples/snippets/get-started-linux/CMakeLists.txt":::

Likewise, populate `main.cpp` to print a simple "Hello World" using the `fmt` library. The `main.cpp` should look like this:

:::code language="cpp" source="../examples/snippets/get-started-linux/main.cpp":::

## Step 3: Build and execute the project

### Step 3.1: Configure

Configure the build using CMake. Remember to point to the correct location of your vcpkg directory by updating the `<vcpkg-root>` placeholder:

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/vcpkg/scripts/buildsystems/vcpkg.cmake
```

The toolchain file allows users to access vcpkg-provided libraries in their CMake project.

### Step 3.2: Build

Next, proceed to build the project:

```bash
cmake --build build
```

### Step 3.3: Execute

Finally, run the executable to see your application in action:

```bash
./build/HelloWorld
```
