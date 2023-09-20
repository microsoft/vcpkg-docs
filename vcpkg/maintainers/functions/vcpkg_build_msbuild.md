---
title: vcpkg_build_msbuild
description: Use vcpkg_build_msbuild to build an MSBuild-based project.
ms.date: 11/30/2022
---
# vcpkg_build_msbuild

> ![WARNING]
> This function has been deprecated in favor of [`vcpkg_msbuild_install`](vcpkg_msbuild_install.md).

Builds a MSBuild project.

## Usage

```cmake
vcpkg_build_msbuild(
    PROJECT_PATH <${SOURCE_PATH}/port.sln>
    [RELEASE_CONFIGURATION <Release>]
    [DEBUG_CONFIGURATION <Debug>]
    [TARGET <Build>]
    [TARGET_PLATFORM_VERSION <10.0.15063.0>]
    [PLATFORM <Win32>]
    [PLATFORM_TOOLSET <v143>]
    [OPTIONS </p:ZLIB_INCLUDE_PATH=X>...]
    [OPTIONS_RELEASE </p:ZLIB_LIB=X>...]
    [OPTIONS_DEBUG </p:ZLIB_LIB=X>...]
    [USE_VCPKG_INTEGRATION]
)
```

## Parameters

### USE_VCPKG_INTEGRATION

Apply the normal `integrate install` integration for building the project.

By default, projects built with this command will not automatically link libraries or have header paths set.

### PROJECT_PATH

The path to the solution (`.sln`) or project (`.vcxproj`) file.

### RELEASE_CONFIGURATION

The configuration (`/p:Configuration` msbuild parameter) used for Release builds.

### DEBUG_CONFIGURATION

The configuration (`/p:Configuration` msbuild parameter) used for Debug builds.

### TARGET_PLATFORM_VERSION

The WindowsTargetPlatformVersion (`/p:WindowsTargetPlatformVersion` msbuild parameter).

### TARGET

The MSBuild target to build (`/t:<TARGET>`).

### PLATFORM

The platform (`/p:Platform` msbuild parameter) used for the build.

See the [`vcpkg_install_msbuild()` documentation](vcpkg_install_msbuild.md#platform) for this parameter.

### PLATFORM_TOOLSET

The platform toolset (`/p:PlatformToolset` msbuild parameter) used for the build.

### OPTIONS

Additional options passed to msbuild for all builds.

### OPTIONS_RELEASE

Additional options passed to msbuild for Release builds. These are in addition to `OPTIONS`.

### OPTIONS_DEBUG

Additional options passed to msbuild for Debug builds. These are in addition to `OPTIONS`.

## Source

[scripts/cmake/vcpkg\_build\_msbuild.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_build_msbuild.cmake)
