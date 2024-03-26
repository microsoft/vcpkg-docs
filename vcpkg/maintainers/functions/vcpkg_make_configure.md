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
    [SHELL <shell>]
    [OPTIONS <configure-setting>...]
    [OPTIONS_RELEASE <configure-setting>...]
    [OPTIONS_DEBUG <configure-setting>...]
    [PRE_CONFIGURE_CMAKE_COMMANDS <cmake-command>...]
    [POST_CONFIGURE_CMAKE_COMMANDS <cmake-command>...]
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

Boolean value that runs autoreconf when set.

### COPY_SOURCE

If specified, the source directory will be copied to the build directory before the configuration process begins. Useful for projects that modify the source tree during configuration or build.

### DISABLE_MSVC_WRAPPERS

Disables the use of MSVC compiler wrappers. Useful for projects that are not compatible with the wrappers.

### NO_CPPFLAGS

Prevents the addition of C Preprocessor flags to the compiler command line. Use this if the project does not use or incorrectly handles these flags.

If set, the `CPPFLAGS` environment variable, typically used for specifying C preprocessor flags, will not be automatically populated based on vcpkg's settings. This can be useful for projects that do not use standard environment variables or have specific preprocessor requirements.

### ADD_BIN_TO_PATH

Adds the `bin` directory to the system path. This is useful for builds that require executables to be available on the path.

### NO_DEFAULT_OPTIONS

When set, this flag disables the automatic inclusion of the following default configure options provided by `vcpkg_make_configure`. Use this flag if you need full control over the configure command line:

General Defaults:
* --disable-silent-rules - Makes the build process output more verbose, which can be helpful for debugging.
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

Turns off the escaping of MSVC flags. This might be necessary for projects that do not expect or handle escaped compiler flags properly.

### USE_RESPONSE_FILES

Enables the use of response files to pass arguments to the configure script. This can help avoid command line length limitations.

### SOURCE_PATH

The directory containing the project's source files

This value is usually obtained as a result of calling a source acquisition command like [`vcpkg_from_github()`](vcpkg_from_github.md).

### SHELL

Specifies the shell to use for running the configuration scripts. Useful for ensuring compatibility in environments where the default shell might not behave as expected.

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

### POST_CONFIGURE_CMAKE_COMMANDS

Specifies a list of CMake commands to execute after the configure process. This can be used for cleanup or to prepare for the next steps in the build process.

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
