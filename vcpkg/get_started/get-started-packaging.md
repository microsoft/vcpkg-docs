---
title: "Tutorial: Create and publish packages"
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

This tutorial shows you how to publish a simple library for the vcpkg registry.

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

1. First, create a new folder under the `vcpkg/ports/` directory named `vcpkg-sample-library`

2. Next, create the `vcpkg.json` file within the `vcpkg-sample-library` folder with the following content:
   
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

3. Now, create the `usage` file within the `vcpkg-sample-library` folder with the following content:

   ```
   vcpkg-sample-library provides CMake targets:
   
   find_package(my_sample_lib CONFIG REQUIRED)
   target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)
   ```
   Providing usage documentation for ports allows users to easily adopt them in their projects. We highly encourage providing a `usage` file within the port's directory (`ports/<port name>/usage`) that describes the minimal steps necessary to integrate with a build system.

   TODO: link to usage reference documentation

4. Finally, create the `portfile.cmake` file within the `vcpkg-sample-library` folder with the following content:
      
   ```
    vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO JavierMatosD/vcpkg-sample-library
        REF "${VERSION}"
        SHA512 0  # This is a temporary value. We will modify this value in the next section.
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

    TODO: link to portfile reference documentation

## 4 - Update SHA512 for `portfile.cmake`

Run:

```console
vcpkg install vcpkg-sample-library
```

You will get a long error message. Scan the output until you find:
```console
.
.
.

Expected hash: 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
Actual hash: 56cffcf9bfb6e9c6bc41e28ae58e8ad25c112db49eafa63f20f95c1ced5712ba7bada9e0990741f70a3f45a4cfd6cb25a3500ef04d36037309b1b65e2f8b569d
```

Copy the "Actual hash"  `56cffcf9bfb6e9c6bc41e28ae58e8ad25c112db49eafa63f20f95c1ced5712ba7bada9e0990741f70a3f45a4cfd6cb25a3500ef04d36037309b1b65e2f8b569d`, and replace the `SHA512` value in the `portfile.cmake` with its value.

Re-run the install command:
```
vcpkg install vcpkg-sample-library


Computing installation plan...
The following packages will be built and installed:
    vcpkg-sample-library:x64-windows -> 1.0.3
Detecting compiler hash for triplet x64-windows...
Restored 0 package(s) from C:\Users\javiermat\AppData\Local\vcpkg\archives in 132 us. Use --debug to see more details.
Installing 1/1 vcpkg-sample-library:x64-windows...
Building vcpkg-sample-library:x64-windows...
-- Note: vcpkg-sample-library only supports static library linkage. Building static library.
-- Downloading https://github.com/JavierMatosD/vcpkg-sample-library/archive/1.0.3.tar.gz -> JavierMatosD-vcpkg-sample-library-1.0.3.tar.gz...
-- Extracting source C:/dev/vcpkg/downloads/JavierMatosD-vcpkg-sample-library-1.0.3.tar.gz
-- Using source at C:/dev/vcpkg/buildtrees/vcpkg-sample-library/src/1.0.3-8d09a66448.clean
-- Found external ninja('1.11.0').
-- Configuring x64-windows
-- Building x64-windows-dbg
-- Building x64-windows-rel
-- Installing: C:/dev/vcpkg/packages/vcpkg-sample-library_x64-windows/share/vcpkg-sample-library/copyright
-- Performing post-build validation
Stored binaries in 1 destinations in 111 ms.
Elapsed time to handle vcpkg-sample-library:x64-windows: 4.4 s
Total install time: 4.4 s
vcpkg-sample-library provides CMake targets:

  find_package(my_sample_lib CONFIG REQUIRED)
  target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)
```

## 5 - Set up versioning

TODO: instructions for setting up the version db, i.e., `x-add-version`
TODO: link to x-add-version reference documentation

## 6 - Verify the port build

TODO: provide a method for verifying the library builds

## Next steps

To learn more about packaging libraries, see our reference documentation:

TODO: add appropriate next steps

- Ports concepts
- Updating a library in vcpkg
- maintainer guide
