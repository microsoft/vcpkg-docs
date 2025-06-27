---
title: Triplet variables
description: This article describes the configurable variables available to triplet files.
author: vicroms
ms.author: viromer
ms.date: 02/14/2024
ms.topic: reference
---

# Triplet variables

This article describes the vcpkg variables that are available to triplet files.
A triplet file can also include user defined variables.

See the [triplets concept documentation](../concepts/triplets.md) for a high-level view of triplet
capabilities.

## Variables

### VCPKG_TARGET_ARCHITECTURE

Specifies the target machine architecture.

Valid options include `x86`, `x64`, `arm`, `arm64`, `arm64ec`, `s390x`, `ppc64le`, `riscv32`, `riscv64`, `loongarch32`, `loongarch64`, `mips64`, and `wasm32`.

### VCPKG_CRT_LINKAGE

Specifies the desired CRT linkage (for MSVC).

Valid options are `dynamic` and `static`.

### VCPKG_LIBRARY_LINKAGE

Specifies the preferred library linkage.

Valid options are `dynamic` and `static`. Libraries can ignore this setting if
they do not support the preferred linkage type.

### VCPKG_BUILD_TYPE

You can set this value to `release` to build release-only versions of the ports.
By default this value is empty. When this value is empty vcpkg builds release
and debug configurations of ports.

### VCPKG_CMAKE_SYSTEM_NAME

Specifies the target platform.

Valid options include any CMake system name, such as:

- Empty (Windows Desktop for legacy reasons)
- `WindowsStore` (Universal Windows Platform)
- `MinGW` (Minimalist GNU for Windows)
- `Darwin` (Mac OSX)
- `iOS` (iOS)
- `Linux` (Linux)
- `Emscripten` (WebAssembly)

### VCPKG_CMAKE_SYSTEM_VERSION

Specifies the target platform system version.

This field is optional and, if present, will be passed into the build as
`CMAKE_SYSTEM_VERSION`.

See also the CMake documentation for
[`CMAKE_SYSTEM_VERSION`](<https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_VERSION.html>)

### VCPKG_CHAINLOAD_TOOLCHAIN_FILE

Specifies an alternate CMake toolchain file to use.

This (if set) will override all other compiler detection logic. By default, a
toolchain file is selected from `scripts/toolchains/` appropriate to the
platform.

> [!NOTE]
> To create a custom toolchain file, it's best to start by including an existing
> toolchain from `${VCPKG_ROOT}/scripts/toolchains` and extend it. This ensures that
> essential variables set by the vcpkg executable, such as `VCPKG_TARGET_ARCHITECTURE`,
> `VCPKG_CXX_FLAGS`, `VCPKG_LINKER_FLAGS`, and other variables are forwarded to CMake.

See also the CMake documentation for [toolchain
files](<https://cmake.org/cmake/help/v3.11/manual/cmake-toolchains.7.html>).

### VCPKG_CXX_FLAGS

Sets additional compiler flags to be used when not using
`VCPKG_CHAINLOAD_TOOLCHAIN_FILE`.

This option also has forms for configuration-specific flags and C language
flags:

- `VCPKG_CXX_FLAGS`
- `VCPKG_CXX_FLAGS_DEBUG`
- `VCPKG_CXX_FLAGS_RELEASE`
- `VCPKG_C_FLAGS`
- `VCPKG_C_FLAGS_DEBUG`
- `VCPKG_C_FLAGS_RELEASE`

If you set `VCPKG_CXX_FLAGS`, you also have to set `VCPKG_C_FLAGS`, and vice-versa. 
The same is true for the configuration-specific flags. These variables accept a 
space-delimited string of compiler flags:

```cmake
set(VCPKG_CXX_FLAGS "/wd4996 -D_CRT_SECURE_NO_WARNINGS")
set(VCPKG_C_FLAGS "/wd4996 -D_CRT_SECURE_NO_WARNINGS")
```

### VCPKG_LINKER_FLAGS

Sets additional linker flags to be used while building dynamic libraries and
executables in the absence of `VCPKG_CHAINLOAD_TOOLCHAIN_FILE`.

This option also has forms for configuration-specific flags:

- `VCPKG_LINKER_FLAGS`
- `VCPKG_LINKER_FLAGS_DEBUG`
- `VCPKG_LINKER_FLAGS_RELEASE`

### VCPKG_MESON_CONFIGURE_OPTIONS

Set additional Meson configure options that are appended to the configure
command (in
[`vcpkg_configure_meson`](../maintainers/functions/vcpkg_configure_meson.md)).

This field is optional.

Also available as build-type specific `VCPKG_MESON_CONFIGURE_OPTIONS_DEBUG` and
`VCPKG_MESON_CONFIGURE_OPTIONS_RELEASE` variables.

### VCPKG_MESON_NATIVE_FILE_RELEASE

Provide an additional configuration dependent file as a meson cross/native file.
Can be used to override settings provided by vcpkg since it will be passed after
vcpkg's generated cross/native files are passed.

Especially useful to provide your own build_machine and host_machine entries.

### VCPKG_MESON_NATIVE_FILE_DEBUG

See [VCPKG_MESON_NATIVE_FILE_RELEASE](#vcpkg_meson_native_file_release).

### VCPKG_MESON_CROSS_FILE_RELEASE

See [VCPKG_MESON_NATIVE_FILE_RELEASE](#vcpkg_meson_native_file_release).

### VCPKG_MESON_CROSS_FILE_DEBUG

See [VCPKG_MESON_NATIVE_FILE_RELEASE](#vcpkg_meson_native_file_release).

### VCPKG_CMAKE_CONFIGURE_OPTIONS

Sets additional CMake configure options that are appended to the configure
command (in
[`vcpkg_cmake_configure`](../maintainers/functions/vcpkg_cmake_configure.md)).

This field is optional.

Also available as build-type specific `VCPKG_CMAKE_CONFIGURE_OPTIONS_DEBUG` and
`VCPKG_CMAKE_CONFIGURE_OPTIONS_RELEASE` variables.

### VCPKG_CONFIGURE_MAKE_OPTIONS

Set additional automake/autoconf configure options that are appended to the
configure command (in
[`vcpkg_configure_make`](../maintainers/functions/vcpkg_configure_make.md)).

This field is optional.

For example, to skip certain libtool checks that may errantly fail:

```cmake
set(VCPKG_CONFIGURE_MAKE_OPTIONS "lt_cv_deplibs_check_method=pass_all")
```

Also available as build-type specific `VCPKG_CONFIGURE_MAKE_OPTIONS_DEBUG` and
`VCPKG_CONFIGURE_MAKE_OPTIONS_RELEASE` variables.

### VCPKG_HASH_ADDITIONAL_FILES

A list of files to include in the calculation of [package ABI hashes](../reference/binarycaching.md#abi-hash).

This field is optional.

Declare any files that affect the contents of a package and should be factored into the calculation of the ABI hash. For example:

* Files that are included (via `include(filepath)`) in custom triplets and toolchains.
* Files that are defined in `VCPKG_MESON_(NATIVE|CROSS)_FILE_<CONFIG>`

Only the contents and order of the files are considered, the paths of the files do not affect the ABI hash.

```cmake
set(VCPKG_HASH_ADDITIONAL_FILES
  "${CMAKE_CURRENT_LIST_DIR}/file1.cmake"
  "${CMAKE_CURRENT_LIST_DIR}/meson-cross.txt"
)
```

### VCPKG_POST_PORTFILE_INCLUDES

A list of CMake files to include after the execution of portfile.cmake.

This field is optional.

The contents and order of the files are used for the ABI hash, the paths of the files do not affect the ABI hash.

```cmake
set(VCPKG_POST_PORTFILE_INCLUDES
  "${CMAKE_CURRENT_LIST_DIR}/file1.cmake"
  "${CMAKE_CURRENT_LIST_DIR}/file2.cmake"
)
```

### VCPKG_DEP_INFO_OVERRIDE_VARS

[!INCLUDE [experimental](../../includes/experimental.md)]

Replaces the default computed list of triplet "Supports" terms.

This option (if set) will override the default set of terms used for [Platform
Expression](../reference/vcpkg-json.md#platform-expression) evaluation.

See the [`"supports"`](../reference/vcpkg-json.md#supports) manifest file field
documentation for more details.

> [!NOTE]
> This list is extracted via the `vcpkg_get_dep_info` helper function.

### VCPKG_DISABLE_COMPILER_TRACKING

> [!WARNING]
> Enabling this option is not recommended since it can lead to ABI
> incompatibility in restored binary packages. See the [binary caching
> documentation](../consume/binary-caching-overview.md) to learn more

When this option is set to `TRUE`, `ON`, or `1`, the compiler will not be
tracked as part of the package abis.

This will cause [binary caching](binarycaching.md) to reuse builds from older or
newer compilers.

## Windows-specific Variables

### VCPKG_ENV_PASSTHROUGH

Instructs vcpkg to allow additional environment variables into the build
process.

On Windows, vcpkg builds packages in a special clean environment that is
isolated from the current command prompt to ensure build reliability and
consistency. This triplet option can be set to a list of additional environment
variables that will be added to the clean environment. The values of these
environment variables will be hashed into the package abi -- to pass through
environment variables without abi tracking, see
[`VCPKG_ENV_PASSTHROUGH_UNTRACKED`](#vcpkg_env_passthrough_untracked).

See also the [`vcpkg env`](../commands/env.md) command for how you can inspect
the precise environment that will be used.

> [!NOTE]
> This list is extracted via the `vcpkg_get_tags` helper function.

### VCPKG_ENV_PASSTHROUGH_UNTRACKED

Instructs vcpkg to allow additional environment variables into the build process
without abi tracking.

See [`VCPKG_ENV_PASSTHROUGH`](#vcpkg_env_passthrough).

### VCPKG_VISUAL_STUDIO_PATH

Specifies the Visual Studio installation to use.

To select the precise combination of Visual Studio instance and toolset version,
we walk through the following algorithm:

1. Determine the setting for `VCPKG_VISUAL_STUDIO_PATH` from the triplet, or the
   environment variable `VCPKG_VISUAL_STUDIO_PATH`, or consider it unset
1. Determine the setting for `VCPKG_PLATFORM_TOOLSET` from the triplet or
   consider it unset
1. Gather a list of all pairs of Visual Studio instances with all toolsets
   available in those instances
   - This is ordered first by instance type (Stable, Prerelease, Legacy) and
     then by toolset version (v143, v142, v141, v140)
1. Filter the list based on the settings for `VCPKG_VISUAL_STUDIO_PATH` and
   `VCPKG_PLATFORM_TOOLSET`.
1. Select the best remaining option

The path should be absolute, formatted with backslashes, and have no trailing
slash:

```cmake
set(VCPKG_VISUAL_STUDIO_PATH "C:\\Program Files\\Microsoft Visual Studio\\2022\\Preview")
```

### VCPKG_PLATFORM_TOOLSET

Specifies the Visual Studio-based C/C++ compiler toolchain to use.

See [`VCPKG_VISUAL_STUDIO_PATH`](#vcpkg_visual_studio_path) for the full
selection algorithm.

Valid settings:

- The Visual Studio 2022 platform toolset is `v143`.
- The Visual Studio 2019 platform toolset is `v142`.
- The Visual Studio 2017 platform toolset is `v141`.
- The Visual Studio 2015 platform toolset is `v140`.

### VCPKG_PLATFORM_TOOLSET_VERSION

Specifies the detailed MSVC C/C++ compiler toolchain to use.

By default, [`VCPKG_PLATFORM_TOOLSET`](#vcpkg_platform_toolset) always chooses the latest installed minor
version of the selected toolset. If you need more granularity, you can use this
variable. You can specify either a partial or a full version number. Valid values are,
for example, `14.25` or `14.27.29110`.

### VCPKG_LOAD_VCVARS_ENV

Determines whether vcpkg will search for and use an instance of Visual Studio as
part of the triplet environment.

By default, this is `ON` for Windows triplets that do not specify
[`VCPKG_CHAINLOAD_TOOLCHAIN_FILE`](#vcpkg_chainload_toolchain_file). For
non-Windows triplets and triplets specifying `VCPKG_CHAINLOAD_TOOLCHAIN_FILE`,
this defaults to `OFF`.

## Linux Variables

### VCPKG_FIXUP_ELF_RPATH

When this option is set to (true|1|on), vcpkg will add `$ORIGIN` and
`$ORIGIN/<path_relative_to_lib>` to the `RUNPATH` header of executables and
shared libraries. This allows packages to be relocated on Linux.

## MacOS Variables

### VCPKG_INSTALL_NAME_DIR

Sets the install name used when building macOS dynamic libraries. Default value
is `@rpath`. See the CMake documentation for
[CMAKE_INSTALL_NAME_DIR](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_NAME_DIR.html)
for more information.

### VCPKG_FIXUP_MACHO_RPATH

Ensures Mach-O binaries built by vcpkg are relocatable by using relative install names and run-paths.

When set to `ON`:

- Modifies absolute `LC_LC_ID_DYLIB` field to `@rpath/<library>` for shared library binaries;
- Modifies absolute `LC_RPATH` fields to relative `@loader_path/<relative/path/to/library>` for executable and shared library binaries.

> [!IMPORTANT]
> This functionality is enabled by default when `VCPKG_TARGET_IS_OSX` is `TRUE`. To disable, explicitly set `VCPKG_FIXUP_MACHO_RPATH` to `OFF` in a [triplet file](../concepts/triplets.md).

For more information about dynamic libraries on macOS, refer to the following links:

- [Dynamic Library Identification](https://forums.developer.apple.com/forums/thread/736719)
- [Dynamic Library Standard Setup for Apps](https://developer.apple.com/forums/thread/736728)
- [Dynamic Library Full Path Alternative](https://developer.apple.com/forums/thread/736737)
- [Embedding nonstandard code structures in a bundle](https://developer.apple.com/documentation/xcode/embedding-nonstandard-code-structures-in-a-bundle)

### VCPKG_OSX_DEPLOYMENT_TARGET

Sets the minimum macOS version for compiled binaries. This also changes what
versions of the macOS platform SDK CMake will search for. See the CMake
documentation for
[CMAKE_OSX_DEPLOYMENT_TARGET](https://cmake.org/cmake/help/latest/variable/CMAKE_OSX_DEPLOYMENT_TARGET.html)
for more information.

### VCPKG_OSX_SYSROOT

Set the name or path of the macOS platform SDK that will be used by CMake. See
the CMake documentation for
[CMAKE_OSX_SYSROOT](https://cmake.org/cmake/help/latest/variable/CMAKE_OSX_SYSROOT.html)
for more information.

### VCPKG_OSX_ARCHITECTURES

Set the macOS / iOS target architecture which will be used by CMake. See the
CMake documentation for
[CMAKE_OSX_ARCHITECTURES](https://cmake.org/cmake/help/latest/variable/CMAKE_OSX_ARCHITECTURES.html)
for more information.

## Per-port customization

The CMake variable `PORT` will be set when interpreting the triplet file. It can be
used to change settings (such as `VCPKG_LIBRARY_LINKAGE`) on a per-port basis.

Example:

```cmake
set(VCPKG_LIBRARY_LINKAGE static)
if(PORT MATCHES "qt5-")
    set(VCPKG_LIBRARY_LINKAGE dynamic)
endif()
```

This will build all the `qt5-*` ports as dynamic libraries, but every other port as a
static library.

For an example in a real project, see
<https://github.com/Intelight/vcpkg/blob/master/triplets/x86-windows-mixed.cmake>.
