---
title: Create and publish packages
description: Tutorial guides the user through the process of packaging a library for vcpkg.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 08/23/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to package libraries for vcpkg.
---

# Tutorial: Create and publish packages

This tutorial shows you how to publish a simle library for the vcpkg registry.

## Prerequisites

- A terminal
- A C++ compiler
- [CMake](https://cmake.org/download/)
- [Git](https://git-scm.com/downloads)

::: zone pivot="os-windows"

> [!NOTE]
> For Windows users, Visual Studio's MSVC (Microsoft Visual C++ Compiler) is the required compiler for C++ development.

::: zone-end

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

## 2 - Configure the `VCPKG_ROOT` environment variable

To set the `VCPKG_ROOT` environment variables, run the following commands:

::: zone pivot="os-linux,os-macos"

```bash
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```

> [!NOTE]
> Setting the `VCPKG_ROOT` environment variable using the `export` command only affects the current shell session. To make this change permanent across sessions, you'll need to add the `export` command to your shell's profile script (e.g., `~/.bashrc` or `~/.zshrc`).

::: zone-end

::: zone pivot="os-windows"

```console
set VCPKG_ROOT="C:\path\to\vcpkg"
set PATH=%VCPKG_ROOT%;%PATH%
```

> [!NOTE]
> Setting the `VCPKG_ROOT` environment variable using the `set` command only affects the current shell session. To make this change permanent across sessions, you can use the `setx` command and restart the shell session.

::: zone-end

Setting `VCPKG_ROOT` tells vcpkg where your vcpkg instance is located.
Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

## 3 - Set up the port files

1. create the `vcpkg.json`
   Modify the `vcpkg.json` file with the following content:
   ```json
    {
    "name": "vcpkg-sample-library",
    "version": "1.0.3",
    "homepage": "https://github.com/JavierMatosD/vcpkg-sample-library",
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
        {
        "name": "fmt"
        }
    ]
    }
    ```
    TODO: provide line-by-line explanation
    TODO: For more information on `vcpkg.json`, see the following:

2. create the `usage`
   Create a file in the `vcpkg/ports/vcpkg-sample-library` directory named `usage` with the following content:

   ```
   vcpkg-sample-library provides CMake targets:
   
   find_package(my_sample_lib CONFIG REQUIRED)
   target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)
   ```
   Providing usage documentation for ports allows users to easily adopt them in their projects. We highly encourage providing a `usage` file within the port's directory (`ports/<port name>/usage`) that describes the minimal steps necessary to integrate with a build system.

3. create the `portfile.cmake`
   
   Modify the `portfile.cmake` with the following content:
   
   ```
    vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO JavierMatosD/vcpkg-sample-library
        REF "${VERSION}"
        SHA512 0
        HEAD_REF main
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

    TODO: Line-by-line explanation:

    For more information on port files, see the following:
    - 

## 4 - Acquire SHA512 for `portfile.cmake`

TODO: instructions for getitng the hash. I.e., install, copy error message, paste into `portfile.cmake`

## 5 - Set up versioning

TODO: instructions for setting up the version db, i.e., `x-add-version`

## 6 - Verify the port build

TODO: provide a method for verifying the library builds

## Next steps

To learn more about packaging libraries, see our reference documentation:

TODO: add appropriate next steps

- Ports concepts
- Updating a library in vcpkg
- maintainer guide
