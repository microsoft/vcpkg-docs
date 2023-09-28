---
title: Installation tree layout
description: This article describes the layout conventions used by vcpkg when copying build output into the installation directory.
author: vicroms
ms.author: viromer
ms.date: 9/21/2023
---
# Installation tree layout

This article describes the layout conventions used by vcpkg for the packages' installation directory. After each
package installation, vcpkg copies the produced output into a specific location depending on the
file's type.

In classic mode, the installation directory is located in `$VCPKG_ROOT/installed` (replace
`$VCPKG_ROOT` with your vcpkg installation path). In manifest mode, each manifest file has a
corresponding `vcpkg_installed` directory. Regardless of the operation mode, the layout of the
installation directory remains the same.

The installation directory is created the first time a package is installed, if you don't see a
installation directory try installing some packages first.

The root level of the installation directory contains:

* A `vcpkg` directory which keeps track of installed packages and files
* A directory for each [triplet](../users/triplets.md) that has installed packages which contains the
  packages' build output

## `vcpkg` directory

The vcpkg directory contains two subfolders (`info` and `updates`) and a file named `status`:

### `info` directory

Contains the list of files installed by each port, the naming convention for these files is: 
`<port name>_<version>_<triplet>.list`. The primary purpose of these files is to avoid conflicts
between ports attempting to install files with the same name. The [`vcpkg
owns`](../commands/owns.md) can be used to query the information in these files.

### `status` file

The vcpkg database, this file contains an entry for every package installed for every triplet. This
file is automatically updated by vcpkg and should not be modified manually.

## Triplet directories

The output of each package installation is contained in a triplet-specific directory, for example,
packages installed for the `x64-windows` triplet are located in `installed/x64-windows` directory.

The layout for the subdirectories inside each triplet directory is the same:

| Subdirectory            | File type                                         | 
| ----------------------- | ------------------------------------------------- |
| [`bin`](#layout-bin)                   | Release `.dll` and `.pdb` files                   |
| `debug/bin`             | Debug `.dll` and `.pdb` files                     |
| `debug/lib`             | Debug `.lib`, `.so`, `.dylib`, and `.a` files               |
| `debug/lib/manual-link` | Manually-linkable debug `.lib`, `.so`, `.dylib`, and `.a` files   |
| `debug/lib/pkgconfig`   | pkgconfig files (`.pc`)                           |
| `include`               | Header-files (`.h`, `.hpp`, `.hxx`)               |
| `lib`                   | Release `.lib`, `.so`, and `.a` files             |
| `lib/manual-link`       | Manually-linkable release `.dll` and `.pdb` files |
| `lib/pkgconfig`         | Pkgconfig files (`.pc`)                           |
| `share/<port>`          | Additional configuration-independent files.                              |
| `share/<port>/copyright`          | The license text for the package.                              |
| `share/<cmakepackagename>/<CMakePackageName>Config.cmake`          | CMake integration files for `find_package(CMakePackageName)`.                              |
| `share/pkconfig`        | configuration-independent pkgconfig files (`.pc`)                           |
| `tools/<port>`          | Executable tools                                  |


### <a name="layout-bin></a> `bin` and `debug/bin` directories

On Windows, these directories contain DLL and PDB files for release and debug configuration
respectively.

### `lib` and `debug/lib` directories

Contains static libraries, import libraries (on Windows), and shared libraries (on non-Windows).
configuration-dependent directories.

### `lib/manual-link` and `debug/lib/manual-link` directories

Contain object files that must be manually linked, these directories are configuration dependent.
Each port must make sure to place manually-linkable files in these directories explicitly.

### `lib/pkgconfig` and `share/pkgconfig` directories

Contain pkgconfig integration files (`.pc`).

### `share/<port>`

Contains miscellaneous files installed by each port, at the very least, each port is required to
provide a `LICENSE` file in `share/<port>` (replace `<port>` with the port name). Some examples of
files contained in this directory are: CMake integration files,
[usage](../maintainers/handling-usage-files.md) files, SPDX files, etc.

### `tools/<port>`

Contains executable tools produced by a port, it is not required, but is highly recommended that
each installed executable goes into a subdirectory matching the name of the port that produced it.
For example, the `minizip` ports installs `minizip.exe` to
`tools/minizip/minizip.exe`.
