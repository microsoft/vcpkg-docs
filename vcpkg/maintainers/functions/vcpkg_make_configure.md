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
    [AUTOCONFIG]
    [COPY_SOURCE]
    [DISABLE_MSVC_WRAPPERS]
    [NO_CPPFLAGS]
    [ADD_BIN_TO_PATH]
    [NO_DEFAULT_OPTIONS]
    [NO_MSVC_FLAG_ESCAPING]
    [USE_RESPONSE_FILES]
    [SOURCE_PATH <source-path>]
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

### AUTOCONFIG

Runs autoreconf when set.

### COPY_SOURCE

If specified, the source directory will be copied to the build directory before the configuration process begins. Useful for projects that modify the source tree during configuration or build.

### DISABLE_MSVC_WRAPPERS

Disables the use of wrapper scripts for MSVC tools (cl.exe, windres).
These wrappers translate and reorder command-line flags to accommodate the expected syntax and operational requirements of these tools. When this option is enabled, the `cl.exe` and `windres` tools will directly interpret command-line flags without any modifications.

### NO_CPPFLAGS

Disables the extraction of C preprocessor flags (`-D` and `-isystem`) from `CFLAGS` into `CPPFLAGS`.

By default (when `NO_CPPFLAGS` is not set or is false), vcpkg automatically identifies and extracts these flags from the `CFLAGS` and `CXXFLAGS` variables, which typically contain general compiler options. It then consolidates these specific flags into a separate `CPPFLAGS` variable, dedicated to preprocessor and include path settings.

However, if you explicitly set `NO_CPPFLAGS` to true, you override this default behavior. Vcpkg will then leave the `-D` and `-isystem` flags untouched in their original variables (`CFLAGS` and `CXXFLAGS`). 

If set, the `CPPFLAGS` environment variable, typically used for specifying C preprocessor flags, will not be automatically populated based on vcpkg's settings. This can be useful for projects that do not use standard environment variables or have specific preprocessor requirements.

### ADD_BIN_TO_PATH

Adds the configure dependent `(debug/)bin` directory to the system path. This is useful if configure builds and runs executables with vcpkg integrated dependencies.

### NO_DEFAULT_OPTIONS

When set, this flag disables the automatic inclusion of the following default configure options provided by `vcpkg_make_configure`. Use this flag if you need full control over the configure command line:

General Defaults:
* --disable-silent-rules - Makes the build process output more verbose.
* --verbose - Enables verbose output from the configure script.

Library Linkage:
* For dynamic library builds: --enable-shared and --disable-static
* For static library builds: --disable-shared and --enable-static

Installation Directories:
* Adjusts standard directories (bin, lib, share, etc.) to conform with vcpkg's layout, ensuring correct installation paths.

Platform-Specific Tweaks:
* On Windows: Ensures compatibility with Windows-specific filesystem and library behaviors.
* On Unix-like systems: Assures the script accommodates the typical filesystem layout and toolchain behaviors.

Removing these defaults gives you the responsibility to specify all necessary configuration options manually, ensuring the project is correctly configured for building with vcpkg.

### NO_MSVC_FLAG_ESCAPING

By default, escape characters (e.g., -Xcompiler, -Xlinker) are added before compiler and linker flags when using MSVC. This is done to ensure that these flags, which might contain spaces or other special characters, are correctly interpreted by the underlying MSVC tools (cl.exe, link.exe)

When you set `NO_MSVC_FLAG_ESCAPING`, you tell vcpkg not to perform this automatic escaping. This can be useful in situations where:

- You know your flags are formatted correctly: If you're confident that your compiler and linker flags don't contain problematic characters, you can avoid unnecessary escaping.
- You're using custom build scripts: If you have custom build scripts or tools that require specific flag formatting, disabling automatic escaping might be necessary for compatibility.

>[!NOTE]
>If you disable the MSVC wrappers using `DISABLE_MSVC_WRAPPERS`, this option (`NO_MSVC_FLAG_ESCAPING`) has no effect because the escape characters are not added in the first place.

### USE_RESPONSE_FILES

Enables the use of response files to pass arguments to the linker.

If set, the code generates response files for the linker flags (`CPPFLAGS`, `CFLAGS`, `CXXFLAGS`, `LDFLAGS`) and passes the response file path to the linker instead of the individual flags. If it's false, the flags are passed directly.

This can be helpful when dealing with long command lines that exceed the maximum allowed length.

### SOURCE_PATH

The directory containing the project's source files

This value is usually obtained as a result of calling a source acquisition command like [`vcpkg_from_github()`](vcpkg_from_github.md).

### OPTIONS

Additional options to pass to the configure script. Use these to specify any custom flags or settings required by the project.

### OPTIONS_RELEASE

Additional options to pass to the configure script. Use these to specify any custom flags or settings required by the project.

These are in addition to `OPTIONS`.

### OPTIONS_DEBUG

Additional options to pass to the configure script. Use these to specify any custom flags or settings required by the project.

These are in addition to `OPTIONS`.

### PRE_CONFIGURE_CMAKE_COMMANDS

Specifies a list of CMake commands to execute before the configure process. This can be used to set up the environment or prepare the build in a way that the configure script requires.

### LANGUAGES

Specifies the programming languages your project uses.

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
