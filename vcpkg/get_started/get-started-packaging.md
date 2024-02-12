---
title: "Tutorial: Package a library with vcpkg"
description: Tutorial guides the user through the process of packaging a library for vcpkg.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 01/10/2024
#CustomerIntent: As a beginner C++ developer, I want to learn how to package libraries for vcpkg using custom overlays.
---

# Tutorial: Package a library with vcpkg

This tutorial guides you on how to package a library for vcpkg using a custom
overlay. We recommended that you read the [Install and use packages with
CMake](get-started.md) tutorial before proceeding.

## Prerequisites

- A terminal
- A C++ compiler
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)

::: zone pivot="shell-cmd, shell-powershell"

> [!NOTE]
> On Windows, this tutorial uses Visual Studio's MSVC as the compiler for C++ development.

::: zone-end

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

## 2 - Configure the `VCPKG_ROOT` environment variable

To set the `VCPKG_ROOT` environment variables, run the following commands:

::: zone pivot="shell-bash"

```bash
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

> [!NOTE]
> Setting the `VCPKG_ROOT` environment variable using the `export` command only
> affects the current shell session. To make this change permanent across
> sessions, you'll need to add the `export` command to your shell's profile
> script (e.g., `~/.bashrc` or `~/.zshrc`).

::: zone-end
::: zone pivot="shell-cmd"

```console
set "VCPKG_ROOT=C:\path\to\vcpkg"
set PATH=%VCPKG_ROOT%;%PATH%
```

> [!NOTE]
> Setting the `VCPKG_ROOT` environment variable using the `set` command only
> affects the current shell session. To make this change permanent across
> sessions, you can use the `setx` command and restart the shell session.

::: zone-end
::: zone pivot="shell-powershell"

```powershell
$env:VCPKG_ROOT="C:\path\to\vcpkg"
$env:PATH="$env:VCPKG_ROOT;$env:PATH"
```

> [!NOTE]
> Setting the `VCPKG_ROOT` and updating the `PATH` environment variables in this
> manner only affects the current PowerShell session. To make these changes
> permanent across all sessions, you should add them to your PowerShell profile
> or set them through the Windows System Environment Variables panel.
::: zone-end

Setting `VCPKG_ROOT` tells vcpkg where your vcpkg instance is located.
Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

## 3 - Set up the custom overlay

1. Create a new directory called `custom-overlay` next to the `Hello World`
   project you created in the [Install and use packages with
   CMake](get-started.md) tutorial.
2. Within `custom-overlay` directory, create a folder named `vcpkg-sample-library`.

## 4 - Set up the port files

First, create the `vcpkg.json` file within the
`custom-overlay\vcpkg-sample-library` folder with the following content:

```json
{
  "name": "vcpkg-sample-library",
  "version": "1.0.2",
  "homepage": "https://github.com/microsoft/vcpkg-docs/tree/cmake-sample-lib",
  "description": "A sample C++ library designed to serve as a foundational example for a tutorial on packaging libraries with vcpkg.",
  "license": "MIT",
  "dependencies": [
    {
      "name" : "vcpkg-cmake",
      "host" : true
    },
    {
      "name" : "vcpkg-cmake-config",
      "host" : true
    },
    "fmt"
  ]
}
```

The `vcpkg.json` file serves as a manifest that defines metadata and
dependencies for a C++ library, providing vcpkg with the necessary information
to build, install, and manage the package.

- `name`: Specifies the name of the library. This is used as the package identifier.
- `version`: Indicates the version number of the library.
- `homepage`: URL to the project's homepage, often its repository. Useful for
  those who want to know more or contribute.
- `description`: Brief text describing what the library does. This is for
  documentation and users.
- `license`: Specifies the license under which the library is distributed.
- `dependencies`: An array containing the list of dependencies that the library needs.
- `name`: `vcpkg-cmake`: Specifies a dependency on `vcpkg-cmake`, which provides
  CMake functions and macros commonly used in vcpkg ports.
- `host`: true: Specifies that `vcpkg-cmake` is a host dependency, meaning it's
  required for building the package but not for using it.
- `name`: `vcpkg-cmake-config`: Specifies a dependency on `vcpkg-cmake-config`,
  which assists in using CMake config scripts.
- `fmt`: Specifies a run-time dependency on the `fmt` library. This means `fmt`
  is required for both building and using the package.

For more information on `vcpkg.json`, see the following documentation on
[manifests](../reference/vcpkg-json.md).

Now, create the `usage` file within the `custom-overlay\vcpkg-sample-library`
directory with the following content:

```usage
vcpkg-sample-library provides CMake targets:

find_package(my_sample_lib CONFIG REQUIRED)
target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)
```

Providing usage documentation for ports allows users to easily adopt them in
their projects. We highly encourage providing a `usage` file within the port's
directory (`ports/<port name>/usage`) that describes the minimal steps necessary
to integrate with a build system. To determine the correct usage instructions it
is recommended to follow upstream's guidance. In the case that upstream does not
provide usage information, it may be necessary to dig through their build system
to find the exported targets.

For more guidance, see [handling usage files](../maintainers/handling-usage-files.md)

Finally, create the `portfile.cmake` file within the
`custom-overlay\vcpkg-sample-library` directory with the following content:

```cmake
vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Microsoft/vcpkg-docs
    REF "${VERSION}"
    SHA512 0  # This is a temporary value. We will modify this value in the next section.
    HEAD_REF cmake-sample-lib
)


vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

This `portfile` defines how to download, build, install, and package a specific
C++ library from GitHub using vcpkg.

- `vcpkg_check_linkage(ONLY_STATIC_LIBRARY)`: Specifies that only static linking
  is supported for this package.
- `vcpkg_from_github`: Starts the function to download the source code from a
  GitHub repository.
  - `OUT_SOURCE_PATH SOURCE_PATH`: Sets the directory where the source code will
    be extracted.
  - `REPO JavierMatosD/vcpkg-sample-library`: The GitHub repository containing
    the source code.
  - `REF "${VERSION}"`: The version of the source code to download.
  - `SHA512 0`: Placeholder for the SHA-512 hash of the source code for
    integrity verification.
  - `HEAD_REF main`: Specifies the default branch for the repository.
- `vcpkg_cmake_configure`: Configures the project using CMake, setting up the build.
  - `SOURCE_PATH "${SOURCE_PATH}"`: The path to the source code downloaded earlier.
- `vcpkg_cmake_install()`: Builds and installs the package using CMake.
- `vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")`: Fixes the CMake
  package configuration files to be compatible with vcpkg.
- `file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")`: Deletes the
  include directory from the debug installation to prevent overlap.
- `file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION ...)`: Installs the LICENSE
  file to the package's share directory and renames it to copyright.
- `configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" ...)`: Copies a usage
  instruction file to the package's share directory.

For more information, refer to the [maintainer guide](../contributing/maintainer-guide.md).

## 5 - Update SHA512 for `portfile.cmake`

Run:

```console
vcpkg install vcpkg-sample-library --overlay-ports=C:\path\to\custom-overlay
```

You will get a long error message. Scan the output until you find:

```console
Expected hash: 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
Actual hash: 4202125968a01219deeee14b81e1d476dab18d968425ba36d640816b0b3db6168f8ccf4120ba20526e9930c8c7294e64d43900ad2aef9d5f28175210d0c3a417
```

Copy the "Actual hash"
`4202125968a01219deeee14b81e1d476dab18d968425ba36d640816b0b3db6168f8ccf4120ba20526e9930c8c7294e64d43900ad2aef9d5f28175210d0c3a417`,
and replace the `SHA512` value in the `portfile.cmake` with its value.

Re-run the install command:

```console
vcpkg install vcpkg-sample-library --overlay-ports=C:\path\to\custom-overlay
```

```console
Computing installation plan...
The following packages will be built and installed:
    vcpkg-sample-library:x64-windows -> 1.0.2 -- C:\Users\dev\demo\custom-overlay\vcpkg-sample-library
Detecting compiler hash for triplet x64-windows...
Restored 0 package(s) from C:\Users\dev\AppData\Local\vcpkg\archives in 174 us. Use --debug to see more details.
Installing 1/1 vcpkg-sample-library:x64-windows...
Building vcpkg-sample-library:x64-windows...
-- Installing port from location: C:\Users\dev\demo\custom-overlay\vcpkg-sample-library
-- Note: vcpkg-sample-library only supports static library linkage. Building static library.
-- Using cached Microsoft-vcpkg-docs-1.0.2.tar.gz.
-- Cleaning sources at C:/Users/dev/demo/vcpkg/buildtrees/vcpkg-sample-library/src/1.0.2-2aff836404.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source C:/Users/dev/demo/vcpkg/downloads/Microsoft-vcpkg-docs-1.0.2.tar.gz
-- Using source at C:/Users/dev/demo/vcpkg/buildtrees/vcpkg-sample-library/src/1.0.2-2aff836404.clean
-- Configuring x64-windows
-- Building x64-windows-dbg
-- Building x64-windows-rel
-- Installing: C:/Users/dev/demo/vcpkg/packages/vcpkg-sample-library_x64-windows/share/vcpkg-sample-library/copyright
-- Performing post-build validation
Stored binaries in 1 destinations in 94 ms.
Elapsed time to handle vcpkg-sample-library:x64-windows: 6.1 s
Total install time: 6.1 s
vcpkg-sample-library provides CMake targets:

find_package(my_sample_lib CONFIG REQUIRED)
target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)
```

## 6 - Verify the port build

To verify the library builds and links properly, add a new dependency to the
`helloworld` project created in the [install packages tutorial](get-started.md).
Make the following changes to the project's manifest and configuration files.

Modify `helloworld/vcpkg.json` to add a dependency on `vcpkg-sample-library`:

```json
{
    "dependencies": [
        "fmt",
        "vcpkg-sample-library"
    ]
}
```

Modify `helloworld/vcpkg-configuration.json` to include the `overlay-ports`
folderthat contains the new port:

```json
{
  "default-registry": {
    "kind": "git",
    "baseline": "45f6e57d3e10ad96b7db206cf7888f736ba5aa61",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "artifact",
      "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
      "name": "microsoft"
    }
  ],
  "overlay-ports": [
    "../custom-overlay"
  ]
}
```

Next, modify `helloworld/CMakeLists.txt` and `helloworld/main.cpp` to use the
new dependency.

Modify the `helloworld/CMakeLists.txt` with the following content:

```cmake
cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(fmt CONFIG REQUIRED)
find_package(my_sample_lib CONFIG REQUIRED)  # Add this line

add_executable(HelloWorld helloworld.cpp)

target_link_libraries(HelloWorld PRIVATE fmt::fmt)
target_link_libraries(HelloWorld PRIVATE my_sample_lib::my_sample_lib)  # Add this line
```

Modify the `main.cpp` with the following content:

```cpp
#include "my_sample_lib.h"  // Replace #include <fmt/core.h> with "my_sample_lib.h"

int main()
{
    greet("vcpkg!");  // Replace fmt::print("Hello World!\n) with this line
    return 0;
}

```

Configure, build, and run the application.

1. Configure the build using CMake:

  ```console
  cmake --preset=default
  ```

2. Build the project:

  ```console
  cmake --build build
  ```

3. Run the application:

  ```console
  ./build/HelloWorld
  ```

  The path to your project's executable may be different, for example: `./build/Debug/HelloWorld.exe`.

  ```console
  Hello vcpkg!
  ```

## Next steps

Now that the `vcpkg-sample-library` has been packaged as port, the next step is
to add it to the vcpkg curated registry. See, [Adding ports to vcpkg
registry](get-started-adding-to-registry.md).

For more information, see:

- [Ports](../concepts/ports.md)
- [Maintainer guide](../contributing/maintainer-guide.md)
