---
title: vcpkg_make_configure
description: Use vcpkg_make_configure to configure a Make-based project.
ms.date: 01/30/2024
---
# vcpkg_make_configure

Configure a Make-based project.

## Usage
        
```cmake
vcpkg_make_configure(
    SOURCE_PATH <source-path>
    [AUTORECONF]
    [COPY_SOURCE]
    [DISABLE_MSVC_WRAPPERS]
    [DISABLE_CPPFLAGS]
    [DISABLE_DEFAULT_OPTIONS]
    [DISABLE_MSVC_FLAG_ESCAPING]
    [OPTIONS <configure-setting>...]
    [OPTIONS_RELEASE <configure-setting>...]
    [OPTIONS_DEBUG <configure-setting>...]
    [PRE_CONFIGURE_CMAKE_COMMANDS <cmake-command>...]
    [LANGUAGES <language>...]
)
```

To use this function, you must depend on the helper port `vcpkg-make`:

```json
"dependencies": [
  {
    "name": "vcpkg-make",
    "host": true
  }
]
```

## Parameters

### SOURCE_PATH

The directory containing the project's source files.

This value is usually obtained as a result of calling a source acquisition command like [`vcpkg_from_github()`](vcpkg_from_github.md).

### AUTORECONF

Runs `autoreconf` when set.

### COPY_SOURCE

If specified, the source directory is copied to the build directory before the configuration process begins. Useful for projects that modify the source tree during configuration or build.

### DISABLE_MSVC_WRAPPERS

By default, `vcpkg_make_configure` supplies wrappers that translate and reorder 'Unix / GCC-style' flags into 'MSVC-style' flags. This accommodates build systems that assume Unix or GCC conventions.

Setting `DISABLE_MSVC_WRAPPERS` disables translation of flags, allowing tools like `cl` and `windres` to receive command-line flags without modifications.

### DISABLE_MSVC_TRANSFORMATIONS

By default, when using `MSVC`, libraries and flags are transformed to fit Unix-style conventions. This is necessary for compatibility with Unix-based build tools, such as `libtool`, and because `MSVC` is invoked via wrappers in cross-platform environments. These wrappers (like the `ar-lib` and `compile` scripts) ensure that `MSVC` behaves more like a Unix compiler, simplifying the build process and making it compatible with Unix-centric tools.

Library Transformation:
- Libraries like `libexample.lib` are transformed into linker-friendly flags by adding a `-l` prefix and stripping the extension (e.g., `libexample.lib` becomes `-llibexample`).

Flag Handling:
- Escape characters (such as `-Xcompiler` and `-Xlinker`) are added before compiler and linker flags (e.g., `-O2` becomes `-Xcompiler -O2`). These escape characters ensure proper handling by tools like `libtool`, which may otherwise misinterpret flags that include special characters or spaces.

When `DISABLE_MSVC_TRANSFORMATIONS` is set, the default transformations are disabled, and libraries and flags are passed in their original form to MSVC.

Example (Default Behavior):

```
libexample.lib  →  -llibexample
-O2             →  -Xcompiler -O2
```

Example (`DISABLE_MSVC_TRANSFORMATIONS`):

```
libexample.lib  →  libexample.lib
-O2             →  -O2
```

>[!NOTE]
>If you disable the MSVC wrappers using `DISABLE_MSVC_WRAPPERS`, this option (`DISABLE_MSVC_TRANSFORMATIONS`) has no effect.

### DISABLE_CPPFLAGS

Disables the extraction of C preprocessor flags (`-D` and `-isystem`) from `CFLAGS` and `CXXFLAGS` into `CPPFLAGS`.

By default, `vcpkg_make_configure` extracts preprocessor relevant flags from the `CFLAGS` and `CXXFLAGS` environment variables into `CPPFLAGS`, because preprocessor flags in vcpkg are normally supplied through compiler settings variables like `VCPKG_C_FLAGS` or `VCPKG_CXX_FLAGS`.

Setting `DISABLE_CPPFLAGS` disables that behavior, leaving preprocessor relevant flags in `CFLAGS` and `CXXFLAGS`.

This can be useful for projects that do not use standard environment variables or have specific preprocessor requirements.

### DISABLE_DEFAULT_OPTIONS

Setting this flag disables the following default configure options provided by `vcpkg_make_configure`. Use this flag only if you need complete control over the configure command line and want to specify all options manually.

General Defaults:
* --disable-silent-rules - Makes the build process output more verbose.
* --verbose - Enables verbose output from the configure script.

Library Linkage:
* For dynamic library builds: --enable-shared and --disable-static
* For static library builds: --disable-shared and --enable-static

Installation Directories:
* Adjusts standard directories (bin, lib, share, etc.) to conform with vcpkg's layout.
* Always Set:
  - `--prefix` : The base installation directory, adjusted per build type (release or debug).
  - `--bindir` : Subdirectory under the prefix for executable programs
  - `--sbindir`: Subdirectory under the prefix for system administration programs
  - `--libdir` : Subdirectory under the prefix for libraries (shared or static)
  - `--mandir` : Subdirectory under the prefix for manual pages (man files)
  - `--docdir` : Subdirectory under the prefix for documentation
  - `--datarootdir` : Subdirectory under the prefix for architecture-independent data

* Debug builds only:
  - `--includedir` : Subdirectory under the prefix for header files

Example Paths:
* Release Configuration:
  - `--prefix=${current_installed_dir}`
  - `--bindir=${prefix}/tools/${PORT}/bin`
  - `--sbindir=${prefix}/tools/${PORT}/sbin`
  - `--libdir=${prefix}/lib`
  - `--mandir=${prefix}/share/${PORT}`
  - `--docdir=${prefix}/share/${PORT}`
  - `--datarootdir=${prefix}/share/${PORT}`

* Debug Configuration:
  - `--prefix=${current_installed_dir}${path_suffix_DEBUG}`
  - `--bindir=${prefix}/../tools/${PORT}${path_suffix_DEBUG}/bin`
  - `--sbindir=${prefix}/../tools/${PORT}${path_suffix_DEBUG}/sbin`
  - `--libdir=${prefix}/lib`
  - `--includedir=${prefix}/../include`
  - `--mandir=${prefix}/share/${PORT}`
  - `--docdir=${prefix}/share/${PORT}`
  - `--datarootdir=${prefix}/share/${PORT}`

Platform-Specific Tweaks:
* On Windows
  - Sets `gl_cv_double_slash_root=yes` to signal to the configure script that it is running on Windows, where paths can begin with double slashes (//).
  - Sets `ac_cv_func_memmove=yes` and `ac_cv_func_memset=yes` to indicate that the `memmove` and `memset` functions are available.
  - sets `gl_cv_host_cpu_c_abi=no` on ARM64 builds to indicate that the compiler does not use the standard C ABI for floating-point numbers.

* On Unix-like systems:
  - Adjusts paths to match the typical Unix filesystem hierarchy (e.g., using /usr/local).

>[!NOTE]
> These default options streamline the configuration process for most projects. If you need a highly customized build, you can set `DISABLE_DEFAULT_OPTIONS` and manually provide all the necessary configuration options.

### OPTIONS

Additional options to pass to the configure script regardless of the configuration type (debug or release). Use these to specify any custom flags or settings required by the project.

### OPTIONS_RELEASE

Additional options to pass to the configure script only during a release build. Use these to specify any custom flags or settings required by the project.
These are in addition to `OPTIONS`.

### OPTIONS_DEBUG

Additional options to pass to the configure script only during a debug build. Use these to specify any custom flags or settings required by the project.
These are in addition to `OPTIONS`.

### PRE_CONFIGURE_CMAKE_COMMANDS

Specifies a list of CMake commands to execute before the configure process for each build type. That is, if `VCPKG_BUILD_TYPE` is empty (the default), these commands are run once for debug and once for release. If `VCPKG_BUILD_TYPE` is `release`, these commands are only run once for release. This can be used to set up the environment or prepare the build in a way that the configure script requires.

>[!NOTE]
> When the `COPY_SOURCE` option is not set, the CMake commands in `PRE_CONFIGURE_CMAKE_COMMANDS` operate directly on the files in the original source directory.

Example

```cmake
function(my_preconfigure_command SRC BUILD_TYPE)
    # Each source directory gets different version.txt files
    if(BUILD_TYPE STREQUAL "release")
        file(WRITE "${SRC}/version.txt" "This is a release version!")
    else()
        file(WRITE "${SRC}/version.txt" "This is a debug version!")
    endif()
    # The environment variable has different values for each configuration
    set(ENV{SPECIAL} "${BUILD_TYPE} is magic")
endfunction()

vcpkg_make_configure(
    # ...
    PRE_CONFIGURE_CMAKE_COMMANDS my_preconfigure_command
)
```

In this example, `my_preconfigure_command` are executed twice (for "Debug" and "Release" if `VCPKG_BUILD_TYPE` is not set):

Debug build: Creates `version.txt` with the content "This is a debug version!" and sets the environment variable `SPECIAL` to "Debug is magic."

Release build: Creates `version.txt` with the content "This is a release version!" and sets the environment variable `SPECIAL` to "Release is magic."

### LANGUAGES

The `LANGUAGES` argument specifies the programming languages to be considered during the configuration and build process.

Valid values are `C`, `CXX`, `ASM`, `CUDA`, `Fortran`, `HIP`, `ISPC`, `Swift`, `OBJC`, `OBJCXX`.

Appends the specified languages to `VCPKG_CMAKE_CONFIGURE_OPTIONS` as `-DVCPKG_LANGUAGES=<languages>`. `vcpkg_cmake_get_vars` uses the options stored in `VCPKG_CMAKE_CONFIGURE_OPTIONS`, including the `LANGUAGES` argument, to configure a dummy CMake project and extract the necessary CMake variables. The `LANGUAGES` argument affects the configuration of the dummy CMake project used in `vcpkg_cmake_get_vars`.
The configuration extracts relevant CMake variables for the specified languages and re-exports them as `VCPKG_DETECTED_CMAKE_*`.

See, 
```cmake
vcpkg_make_configure(
    SOURCE_PATH "/path/to/source"
    LANGUAGES "C" "CXX" "Fortran"
)
```

## Examples

```cmake
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO example/examplelib
    REF v1.2.3
    SHA512 123456...
)

vcpkg_make_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        --disable-silent-rules
        --enable-foo
    OPTIONS_DEBUG
        --enable-debug-logs
)

vcpkg_make_install()
```

## Remarks

This command replaces [`vcpkg_configure_make()`](vcpkg_configure_make.md).
