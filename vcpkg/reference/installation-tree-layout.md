---
title: Installation tree layout
description: This article describes the layout conventions used by vcpkg when copying build output into the installation directory.
author: vicroms
ms.author: viromer
ms.date: 9/21/2023
---
# Installation directory layout conventions

This article describes the layout conventions used by vcpkg for the installation directory. The
installation directory holds the files installed by each package. Port authors should ensure that
their packages follow the conventions described in this article.

In classic mode, the installation directory is located in `$VCPKG_ROOT/installed` (replace
`$VCPKG_ROOT` with your vcpkg installation path). In manifest mode, each manifest file has a
corresponding `vcpkg_installed` directory. Regardless of the operation mode, the layout of the
installation directory remains the same. The location of the installation directory can be changed
with the [`--x-install-root`](../commands/common-options.md#install-root) option.

The installation directory is created the first time a package is installed, if you don't see a
installation directory try [installing some packages first](../consume/manifest-mode.md).

The root level of the installation directory contains:

* A `vcpkg` directory which keeps track of installed packages and files
* A directory for each [triplet](../users/triplets.md) that has installed packages which contains the
  packages' build output

## `vcpkg` directory

The vcpkg directory serves as a database of installed package, it has files to keep track of
installed packages and ownership of installed files.

## Triplet directories

The output of each package installation is contained in a triplet-specific directory, for example,
packages installed for the `x64-windows` triplet are located in `installed/x64-windows` directory.

The layout for the subdirectories inside each triplet directory is the same:

| Subdirectory                                              | File type                                                       |
|-----------------------------------------------------------|-----------------------------------------------------------------|
| [`bin`](#layout-bin)                                      | Release `.dll` and `.pdb` files                                 |
| [`debug/bin`](#layout-bin)                                | Debug `.dll` and `.pdb` files                                   |
| [`debug/lib`](#layout-lib)                                | Debug `.lib`, `.so`, `.dylib`, and `.a` files                   |
| [`debug/lib/manual-link`](#layout-manual-link)            | Manually-linkable debug `.lib`, `.so`, `.dylib`, and `.a` files |
| [`debug/lib/pkgconfig`](#layout-pkgconfig)                | pkgconfig files (`.pc`)                                         |
| [`include`](#layout-include)                              | Header-files (`.h`, `.hpp`, `.hxx`)                             |
| [`lib`](#layout-lib)                                      | Release `.lib`, `.so`, and `.a` files                           |
| [`lib/manual-link`](#layout-manual-link)                  | Manually-linkable release `.dll` and `.pdb` files               |
| [`lib/pkgconfig`](#layout-pkgconfig)                      | Pkgconfig files (`.pc`)                                         |
| [`share/<port>`](#layout-share)                           | Additional configuration-independent files.                     |
| [`share/<port>/copyright`](#layout-copyright)             | The license text for the package.                               |
| [`share/<package>/<package>Config.cmake`](#layout-cmake)  | CMake integration files for `find_package(package)`.            |
| [`share/pkconfig`](#layout-pkgconfig)                     | configuration-independent pkgconfig files (`.pc`)               |
| [`tools/<port>`](#layout-tools)                           | Executable tools                                                |


### <a name="layout-bin"></a> `bin` and `debug/bin` directories

On Windows, these directories contain DLL and PDB files for release and debug configuration
respectively. Any executable file produced by a port should be moved to a `tools/<port>` directory.

### <a name="layout-include"></a> `include`

Contains header files (`.h`, `.hpp`, `.hxx`), the internal layout of this directory should reflect
upstream usage of the package's header files. For example, a `contoso` library that provides
`contoso/contoso.h` should create the necessary subdirectories to reflect the relative path, in this
case, the header file should be placed in `include/contoso/contoso.h`.

vcpkg forbids installing some reserved header file names in the root of the `include` directory, for
example: `err.h`, `user.h`, `time.h`, and others. Libraries that provide a forbidden header file name should
place their header files inside a `include/<port>` directory. If the library intends to replace a
system header file, it should set the `VCPKG_POLICY_ALLOW_RESTRICTED_HEADERS` policy in its
`portfile.cmake`.

### <a name="layout-lib"></a> `lib` and `debug/lib` directories

Contains static libraries, import libraries (on Windows), and shared libraries (on non-Windows).
configuration-dependent directories.

### <a name="layout-manual-link"></a> `lib/manual-link` and `debug/lib/manual-link` directories

Contains object files that must be manually linked, these directories are configuration dependent.
Each port must make sure to place manually-linkable files in these directories explicitly.

Files that may cause issues when linked automatically must be placed in the `lib/manual-link`
folders instead of the `lib` directory. For example, if a library is intended to define the
`main()` function for a program. 

### <a name="layout-pkgconfig"></a> `lib/pkgconfig` and `share/pkgconfig` directories

Contains pkgconfig integration files (`.pc`).

### <a name="layout-share"></a> `share/<port>`

Contains miscellaneous files installed by each port, at the very least, each port is required to
provide a `LICENSE` file in `share/<port>` (replace `<port>` with the port name). Some examples of
files contained in this directory are: CMake integration files,
[usage](../maintainers/handling-usage-files.md) files, SPDX files, etc.

### <a name="layout-copyright"></a> `share/<port>/copyright`

vcpkg expects ports to provide a `copyright` file that contains the license information for the
installed package. See the [maintainer
guide](../contributing/maintainer-guide.md#install-copyright-file) for more information.

### <a name="layout-cmake"></a> `share/<package>/<package>Config.cmake`, `share/<package>/<package>-config.cmake`

CMake integration files should be placed in the `share` folder and respect CMake's rules for
[`find_package(package)`](<https://cmake.org/cmake/help/latest/command/find_package.html#config-mode-search-procedure>)
in `CONFIG` mode.

### <a name="layout-tools"></a> `tools/<port>`

Contains executable tools produced by a port, it is not required, but is highly recommended that
each installed executable goes into a subdirectory matching the name of the port that produced it.
For example, a `contoso` port might install a `ContosoGenerator.exe` to
`tools/contoso/ContosoGenerator.exe`.
