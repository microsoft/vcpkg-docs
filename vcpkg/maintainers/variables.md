---
title: Portfile Variable Reference
---

# Portfile Variable Reference

These are variables set automatically by vcpkg for use in any port. They should not be modified by the port.

### PORT
The name of the port that is currently being built

### VERSION
The version of the port that is currently being built

### VCPKG_TARGET_IS_\<system\>
Defined to a true value if building for *\<system\>*.

Supported systems:
- `VCPKG_TARGET_IS_WINDOWS` - Also true for UWP and MINGW
- `VCPKG_TARGET_IS_UWP`
- `VCPKG_TARGET_IS_MINGW`
- `VCPKG_TARGET_IS_LINUX`
- `VCPKG_TARGET_IS_OSX`
- `VCPKG_TARGET_IS_ANDROID`
- `VCPKG_TARGET_IS_FREEBSD`
- `VCPKG_TARGET_IS_OPENBSD`
- `VCPKG_TARGET_IS_EMSCRIPTEN`

### VCPKG_HOST_IS_\<system\>
Defined to a true value if building on *\<system\>*.

Supported systems:
- `VCPKG_HOST_IS_WINDOWS`
- `VCPKG_HOST_IS_OSX`
- `VCPKG_HOST_IS_LINUX`
- `VCPKG_HOST_IS_FREEBSD`
- `VCPKG_HOST_IS_OPENBSD`

### VCPKG_HOST_PATH_SEPARATOR
Host specific path separator.

Always use quotes when expanding `VCPKG_HOST_PATH_SEPARATOR` since it may contain a semicolon. For example:
```cmake
set(ENV{SOMEPATH} "$ENV{SOMEPATH}${VCPKG_HOST_PATH_SEPARATOR}${my_extra_path}")
```

### VCPKG_HOST_EXECUTABLE_SUFFIX
Executable suffix for the building system.

This is set to `".exe"` on Windows and `""` on other platforms.

### VCPKG_TARGET_EXECUTABLE_SUFFIX
Executable suffix for the built-for system.

This is set to `".exe"` when [`VCPKG_TARGET_IS_WINDOWS`](#VCPKG_TARGET_IS_\<system\>) and `""` otherwise.

### VCPKG_HOST_BUNDLE_SUFFIX
Bundle suffix of the building system.

### VCPKG_TARGET_BUNDLE_SUFFIX
Bundle suffix of the built-for system.

### VCPKG_TARGET_STATIC_LIBRARY_PREFIX
Static library prefix for the built-for system.

See also [`CMAKE_STATIC_LIBRARY_PREFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_STATIC_LIBRARY_PREFIX.html).

### VCPKG_TARGET_STATIC_LIBRARY_SUFFIX
Static library suffix for the built-for system.

See also [`CMAKE_STATIC_LIBRARY_SUFFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_STATIC_LIBRARY_SUFFIX.html).

### VCPKG_TARGET_SHARED_LIBRARY_PREFIX
Shared (dynamic) library prefix for the built-for system.

See also [`CMAKE_SHARED_LIBRARY_PREFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_SHARED_LIBRARY_PREFIX.html).

### VCPKG_TARGET_SHARED_LIBRARY_SUFFIX
Shared (dynamic) library suffix for the built-for system.

See also [`CMAKE_SHARED_LIBRARY_SUFFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_SHARED_LIBRARY_SUFFIX.html).

### VCPKG_TARGET_IMPORT_LIBRARY_PREFIX
Import library prefix for the built-for system.

See also [`CMAKE_IMPORT_LIBRARY_PREFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_IMPORT_LIBRARY_PREFIX.html).

### VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX
Import library suffix for the built-for system.

See also [`CMAKE_IMPORT_LIBRARY_SUFFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_IMPORT_LIBRARY_SUFFIX.html).

### VCPKG_FIND_LIBRARY_PREFIXES
Prefixes used for [`find_library()`][] calls in portfiles.

These are set to 

### VCPKG_FIND_LIBRARY_SUFFIXES
Suffixes used for [`find_library()`][] calls in portfiles.

[`find_library()`]: https://cmake.org/cmake/help/latest/command/find_library.html

### TARGET_TRIPLET
The name of the [triplet](../users/triplets.md) being built for.

### CURRENT_INSTALLED_DIR
The absolute path to the installed files for the [target triplet](#target_triplet).

### HOST_TRIPLET
The name of the [triplet](../users/triplets.md) that matches the building system.

### CURRENT_HOST_INSTALLED_DIR
The absolute path to the installed files for the [host triplet](#host_triplet).

### VCPKG_CROSSCOMPILING
Whether vcpkg is compiling for a different system than the building machine.

This is the same as comparing [`TARGET_TRIPLET`](#target_triplet) and [`HOST_TRIPLET`](#host_triplet) for equality. If the triplets are different, ports must not assume that they can run the results of a build.

For example, when compiling for Android from MacOS, `VCPKG_CROSSCOMPILING` will be set to a true value.
