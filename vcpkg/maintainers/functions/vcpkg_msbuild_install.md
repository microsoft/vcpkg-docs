---
title: vcpkg_msbuild_install
description: Learn how to use vcpkg_msbuild_install.
author: Neumann-A
ms.author: viromer
ms.date: 08/14/2023
---

# vcpkg_msbuild_install

Build and install a MSBuild project.

This function replaces [`vcpkg_install_msbuild`](vcpkg_install_msbuild.md).

```json
"dependencies": [
  { 
    "name", "vcpkg-msbuild",
    "host": "true"
  }
]
```
`vcpkg_msbuild_install` will generate a props file and target file containing the CMake toolchain build flags and include it via `/p:ForceImportAfterCpp(Props|Targets)`

## Usage

```cmake
vcpkg_install_msbuild(
    SOURCE_PATH <${SOURCE_PATH}>
    PROJECT_SUBPATH <port.sln>
    [ADD_BIN_TO_PATH]
    [NO_INSTALL]
    [NO_TOOLCHAIN_PROPS]
    [CLEAN]
    [RELEASE_CONFIGURATION <Release>]
    [DEBUG_CONFIGURATION <Debug>]
    [TARGET <Build>]
    [PLATFORM <Win32>]
    [OPTIONS </p:ZLIB_INCLUDE_PATH=X>...]
    [OPTIONS_RELEASE </p:ZLIB_LIB=X>...]
    [OPTIONS_DEBUG </p:ZLIB_LIB=X>...]
    [DEPENDENT_PKGCONFIG <zlib>]
    [ADDITIONAL_LIBS <getopt.lib>]
    [ADDITIONAL_LIBS_DEBUG <zlibd.lib>]
    [ADDITIONAL_LIBS_RELEASE <zlib.lib>]
)
```

## Parameters

### SOURCE_PATH
The path to the root of the source tree.

Because MSBuild uses in-source builds, the source tree is copied into a temporary location for the build. This
parameter is the base for that copy and forms the base for all XYZ_SUBPATH options.


### PROJECT_SUBPATH

The subpath to the solution (`.sln`) or project (`.vcxproj`) file relative to `SOURCE_PATH`.

### ADD_BIN_TO_PATH

Adds the configuration dependent binary path to the `PATH` environment variable before the build.
This is required to run tools built by the project itself. The `PATH` environment variable is not be mutated if `VCPKG_CROSSCOMPILING` is `true`

### NO_TOOLCHAIN_PROPS

Do not generate the toolchain properties via cmake. This automatically disables any vcpkg integration.

### NO_INSTALL

Do not automatically try to install any .lib|.dll|.pdb|.exe files found in the build dir.

### CLEAN

Indicates that the intermediate files should be removed after installation.

### RELEASE_CONFIGURATION

The configuration (`/p:Configuration` msbuild parameter) used for Release builds. Defaults to: `Release`

### DEBUG_CONFIGURATION

The configuration (`/p:Configuration` msbuild parameter) used for Debug builds. Defaults to: `Debug`

### TARGET

The MSBuild target to build (`/t:<TARGET>`).  Defaults to: `Rebuild`

### PLATFORM

The platform (`/p:Platform` msbuild parameter) used for the build.

This defaults to a value mapping `VCPKG_TARGET_ARCHITECTURE` to the default values Visual Studio uses when creating a `.vcxproj`:

* `x86` becomes `Win32`
* `x64` becomes `x64`
* `arm` becomes `ARM`
* `arm64` becomes `arm64`

When passing a `.sln` rather than a `.vcxproj`, this may need to be set back to `${VCPKG_TARGET_ARCHITECTURE}` to match the Platform strings used by solutions.

### OPTIONS

Additional options to pass to MSBuild for all builds.

### OPTIONS_RELEASE

Additional options passed to msbuild for Release builds. These are in addition to `OPTIONS`.

### OPTIONS_DEBUG

Additional options passed to msbuild for Debug builds. These are in addition to `OPTIONS`.

### DEPENDENT_PKGCONFIG

A list of `pkg-config` dependencies to add to the build. This adds all required build flags (includes/definitions/libraries) to the build properties.

### ADDITIONAL_LIBS

Additional libraries to add to the project.

### ADDITIONAL_LIBS_DEBUG

Additional libraries to add to msbuild for Release builds. These are in addition to `ADDITIONAL_LIBS`.

### ADDITIONAL_LIBS_RELEASE

Additional libraries to add to msbuild for Debug builds. These are in addition to `ADDITIONAL_LIBS`.

## Source

[ports/vcpkg-msbuild/vcpkg\_msbuild\_install.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-msbuild/vcpkg_msbuild_install.cmake)