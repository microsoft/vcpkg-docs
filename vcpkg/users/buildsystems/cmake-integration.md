---
title: vcpkg in CMake projects
description: Integrate vcpkg into a CMake project using a terminal, Visual Studio, Visual Studio Code, or other IDEs.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: concept-article
---
# vcpkg in CMake projects

vcpkg offers seamless integration with CMake to make installed packages available in
your projects automatically. The mechanism in which vcpkg integrates is by providing a CMake toolchain file.

The first time CMake configures a project, it runs internal search routines to locate a viable
[toolchain](<https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#id9>) (compiler,
linker, etc.). This search happens within the
[`project()`](https://cmake.org/cmake/help/latest/command/project.html#command:project) function in
your `CMakeLists.txt`.

To customize the toolchain selection process, CMake supports using custom CMake-language scripts,
known as toolchain files. A toolchain file is specified by setting the `CMAKE_TOOLCHAIN_FILE`
variable. CMake evaluates the contents of the provided toolchain script and sets variable definitions, paths to
required build tools, and other build parameters, such as cross-compilation flags, accordingly.

When you set `CMAKE_TOOLCHAIN_FILE` to use the vcpkg toolchain
(`<vcpkg-root>/scripts/buildsystems/vcpkg.cmake`), vcpkg takes advantage of the toolchain file
mechanism to inject code to integrate with built-in CMake functions transparently to you.

You can still use a toolchain file to configure your own toolsets by using the
[`VCPKG_CHAINLOAD_TOOLCHAIN_FILE`](../../users/triplets.md#vcpkg_chainload_toolchain_file) triplet variable.

The vcpkg integration works differently depending on the operation mode you're using:

In [classic mode](../../concepts/classic-mode.md), vcpkg sets CMake search paths
appropriately to make installed packages available via the `find_package()`,
`find_library()`, and `find_path()` functions.

In [manifest mode](../../concepts/manifest-mode.md), in addition to the above,
the toolchain detects manifest files (`vcpkg.json` files) and runs `vcpkg
install` to automatically acquire the project's dependencies.

Because the toolchain file is evaluated during the `project()` call, all CMake-level variables that
modify a vcpkg setting must be set before the first call to `project()`. It may also be necessary to
reconfigure your CMake project if you modify any vcpkg setting that results in [ABI
hash](../../reference/binarycaching.md#abi-hash) changes.

See [Installing and Using Packages Example: sqlite](../../examples/installing-and-using-packages.md) for a fully worked example using CMake.

## `CMAKE_TOOLCHAIN_FILE`

> [!NOTE]
> If you set `CMAKE_TOOLCHAIN_FILE` in your `CMakeList.txt` file, make sure that
> the variable is set before any calls to `project()`.

Projects configured to use the vcpkg toolchain file (via the CMake setting `CMAKE_TOOLCHAIN_FILE`) can find libraries from vcpkg using the standard CMake functions: `find_package()`, `find_path()`, and `find_library()`.

We recommend using [CMake Presets] to specify your toolchain file. For example, if you have defined the environment variable `VCPKG_ROOT`, you can use the following `CMakePresets.json` and pass `--preset debug` on the configure line:

:::code language="json" source="snippets/cmake-integration/CMakePresets.json":::

```sh
cmake -B build -S /my/project --preset debug
```

If you need to use an absolute path for vcpkg specific to your current machine, you can use `CMakeUserPresets.json` and add it to your `.gitignore` file.

:::code language="json" source="snippets/cmake-integration/CMakeUserPresets.json":::

CMake versions older than 3.19 must pass the toolchain file on the configure command line:

```console
cmake ../my/project -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
```

## Using Libraries

vcpkg supports CMake's native mechanisms for finding libraries: `find_package()`, `find_library()`, and `find_path()`. When installing libraries with specific CMake support, vcpkg will display usage information on how to consume the library:

```
The package zlib is compatible with built-in CMake targets:

    find_package(ZLIB REQUIRED)
    target_link_libraries(main PRIVATE ZLIB::ZLIB)

```

vcpkg does not automatically add any include or links paths into your project. To use a header-only library you can use `find_path()` which will correctly work on all platforms:

```cmake
# To find and use catch2
find_path(CATCH_INCLUDE_DIR NAMES catch.hpp PATH_SUFFIXES catch2)
target_include_directories(main PRIVATE ${CATCH_INCLUDE_DIR})
```

## IDE Integration

### Visual Studio / Visual Studio Code

We recommend using [CMake Presets] in both Visual Studio and Visual Studio Code.

Learn more at [Configure and build with CMake Presets in Visual Studio](/cpp/build/cmake-presets-vs) and [Configure and build with CMake Presets in Visual Studio Code](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-presets.md).

### CLion

Open the Toolchains settings (`File > Settings` on Windows and Linux, `CLion > Preferences` on macOS), and go to the CMake settings (`Build, Execution, Deployment > CMake`). In `CMake options`, add the following line:

```cmake
-DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
```

You must add this line to each profile separately.

## Using Multiple Toolchain Files

To combine vcpkg's toolchain file with another toolchain file, you can set the CMake cache variable `VCPKG_CHAINLOAD_TOOLCHAIN_FILE`:

```console
cmake ../my/project \
   -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake \
   -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=../my/project/toolchain.cmake
```

Alternatively, you can include the vcpkg toolchain at the end of the primary toolchain file:

```cmake
# MyToolchain.cmake
set(CMAKE_CXX_COMPILER ...)
set(VCPKG_TARGET_TRIPLET x64-my-custom-windows-triplet)
include(/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake)
```

> [!NOTE]
> vcpkg does not automatically apply your toolchain's settings, such as your compiler or compilation flags, while building libraries. To change vcpkg's library settings, you must make a [custom triplet file](../triplets.md) (which can [share your toolchain](../triplets.md#vcpkg_chainload_toolchain_file))**

## Settings Reference

All vcpkg-affecting variables must be defined before the first `project()` directive such as in a `CMakePresets.json`'s [`"cacheVariables"`](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html#configure-preset) map, via the command line, or `set()` statements.

### `VCPKG_TARGET_TRIPLET`

This setting controls the [triplet](../triplets.md) vcpkg will install and consume libraries from.

If unset, vcpkg will automatically detect an appropriate default triplet given the current compiler settings. If you change this CMake variable, you must delete your cache and reconfigure.

### `VCPKG_HOST_TRIPLET`

This variable controls which [triplet](../triplets.md) host dependencies will be installed for.

If unset, vcpkg will automatically detect an appropriate native triplet (x64-windows, x64-osx, x64-linux).

See also [Host dependencies](../host-dependencies.md).

### `VCPKG_INSTALLED_DIR`

This variable sets the location where libraries will be installed and consumed from.

In manifest mode, the default is `${CMAKE_BINARY_DIR}/vcpkg_installed`.

In classic mode, the default is `${VCPKG_ROOT}/installed`.

### `VCPKG_MANIFEST_MODE`

This variable forces vcpkg to operate in either manifest mode or classic mode.

Defaults to `ON` when `VCPKG_MANIFEST_DIR` is non-empty or `${CMAKE_SOURCE_DIR}/vcpkg.json` exists.

To disable manifest mode while a `vcpkg.json` is detected, set this to `OFF`.

### `VCPKG_MANIFEST_DIR`

This variable specifies an alternate folder containing a `vcpkg.json` manifest.

Defaults to `${CMAKE_SOURCE_DIR}` if `${CMAKE_SOURCE_DIR}/vcpkg.json` exists.

### `VCPKG_MANIFEST_INSTALL`

This variable controls whether vcpkg will be automatically run to install your dependencies during your configure step.

Defaults to `ON` if `VCPKG_MANIFEST_MODE` is `ON`.

### `VCPKG_BOOTSTRAP_OPTIONS`

This variable can be set to additional command parameters to pass to `./bootstrap-vcpkg`.

In manifest mode, vcpkg will be automatically bootstrapped if the executable does not exist.

### `VCPKG_OVERLAY_TRIPLETS`

This variable can be set to a list of paths to be passed on the command line as `--overlay-triplets=...`

### `VCPKG_OVERLAY_PORTS`

This variable can be set to a list of paths to be passed on the command line as `--overlay-ports=...`

### `VCPKG_MANIFEST_FEATURES`

This variable can be set to a list of features to activate when installing from your manifest.

For example, features can be used by projects to control building with additional dependencies to enable tests or samples:

```json
{
  "name": "mylibrary",
  "version": "1.0",
  "dependencies": [ "curl" ],
  "features": {
    "samples": {
      "description": "Build Samples",
      "dependencies": [ "fltk" ]
    },
    "tests": {
      "description": "Build Tests",
      "dependencies": [ "gtest" ]
    }
  }
}
```

This setting can be controlled directly by [CMake Presets] with `"cacheVariables"` or indirectly based on other settings:

```cmake
# CMakeLists.txt

option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

option(BUILD_SAMPLES "Build samples" OFF)
if(BUILD_SAMPLES)
  list(APPEND VCPKG_MANIFEST_FEATURES "samples")
endif()

project(myapp)

# ...
```

### `VCPKG_MANIFEST_NO_DEFAULT_FEATURES`

This variable controls activation of default features in addition to those listed in `VCPKG_MANIFEST_FEATURES`. If set to `ON`, default features will not be automatically activated.

Defaults to `OFF`.

### `VCPKG_INSTALL_OPTIONS`

This variable can be set to a list of additional command line parameters to pass to the vcpkg tool during automatic installation.

### `VCPKG_PREFER_SYSTEM_LIBS`

**This feature has been deprecated. Use empty overlay ports instead.**

This variable controls whether vcpkg will append instead of prepend its paths to `CMAKE_PREFIX_PATH`, `CMAKE_LIBRARY_PATH` and `CMAKE_FIND_ROOT_PATH` so that vcpkg libraries/packages are found after toolchain/system libraries/packages.

Defaults to `OFF`.

### `VCPKG_FEATURE_FLAGS`

This variable can be set to a list of feature flags to pass to the vcpkg tool during automatic installation to opt-in to experimental behavior.

See the [`--feature-flags=`](../../commands/common-options.md#) command line option for more information.

### `VCPKG_TRACE_FIND_PACKAGE`

When set to `ON`, Print every call to `find_package`. Nested calls (e.g. via `find_dependency`) are indented according to nesting depth.


### `VCPKG_LOCK_FIND_PACKAGE_<Pkg>`

When this option is set, non-nested calls to [`find_package`](<https://cmake.org/cmake/help/latest/command/find_package.html>) are either
required (`VCPKG_LOCK_FIND_PACKAGE_<Pkg>=ON`) or disabled (`VCPKG_LOCK_FIND_PACKAGE_<Pkg>=OFF`).

This variable is a tool to control direct dependencies and related features in vcpkg ports
which use the CMake build system. It can be used with [`vcpkg_check_features`](../../maintainers/functions/vcpkg_check_features.md) and avoids
unintended effects on transitive dependencies.

[CMake Presets]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
