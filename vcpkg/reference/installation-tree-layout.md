---
title: Installation directory layout
description: This article describes the layout conventions used by vcpkg when copying build output into the installation directory.
author: vicroms
ms.author: viromer
ms.date: 10/17/2023
---
# Installation directory layout conventions

This article describes the layout conventions used by vcpkg for the installation
directory. The installation directory holds the files installed by each package.
Port authors should ensure that their packages follow the conventions described
in this article.

In [classic mode](../users/classic-mode.md), the installation directory is
located in `$VCPKG_ROOT/installed` (where `$VCPKG_ROOT` is your vcpkg
installation path). In [manifest mode](../users/manifests.md), each manifest
file has a corresponding `vcpkg_installed` directory. The location of the
installation directory can be changed with the
[`--x-install-root`](../commands/common-options.md#install-root) option.
Regardless of the operation mode, the layout of the installation directory
remains the same.

The installation directory is created the first time a package is installed,
if you don't see an installation directory try 
[installing some packages first](../consume/manifest-mode.md).

The root level of the installation directory contains:

* A `vcpkg` directory which keeps track of installed packages and files
* A directory for each [triplet](../users/triplets.md). Each triplet directory
  contains files installed by each package.

## Triplet directories

The output of each package installation is contained in a triplet-specific directory.
For example, packages installed for the `x64-windows` triplet are located in
`installed/x64-windows` directory.

The layout for the subdirectories inside each triplet directory is the same:

> [!NOTE]
> Some packages may produce files that don't match the conventions described here.
> Port authors should determine the final location of the produced files based
> on the purpose each file serves.


| Subdirectory                                                       | File type                                                         |
| ------------------------------------------------------------------ | ----------------------------------------------------------------- |
| [`bin`](#layout-bin)                                               | Release `.dll` and `.pdb` files                                   |
| [`debug/bin`](#layout-bin)                                         | Debug `.dll` and `.pdb` files                                     |
| [`debug/lib`](#layout-lib)                                         | Debug `.lib`, `.so`, `.dylib`, and `.a` files                     |
| [`debug/lib/manual-link`](#layout-manual-link)                     | Manually-linkable debug `.lib`, `.so`, `.dylib`, and `.a` files   |
| [`debug/plugins/<group>`](#layout-plugins)                         | Runtime-load debug `.dll` files                                   |
| [`debug/lib/pkgconfig`](#layout-pkgconfig)                         | Debug pkgconfig files (`.pc`)                                     |
| [`include`](#layout-include)                                       | Header-files (`.h`, `.hpp`, `.hxx`)                               |
| [`lib`](#layout-lib)                                               | Release `.lib`, `.so`, `.dylib` and `.a` files                    |
| [`lib/manual-link`](#layout-manual-link)                           | Manually-linkable release `.lib`, `.so`, `.dylib`, and `.a` files |
| [`lib/pkgconfig`](#layout-pkgconfig)                               | Pkgconfig files (`.pc`)                                           |
| [`plugins/<group>`](#layout-plugins)                               | Runtime-load release `.dll` files                                 |
| [`share/<port>`](#layout-share)                                    | Additional configuration-independent files                        |
| [`share/<port>/copyright`](#layout-copyright)                      | The license text for the package                                  |
| [`share/<port>/usage`](#layout-usage)                              | Buildsystem integration instructions file                         |
| `share/<port>/vcpkg-port-config.cmake`                             | Port-defined CMake functions and variables                        |
| [`share/<lowercase-package>/<package>Config.cmake`](#layout-cmake) | CMake integration files for `find_package(package)`               |
| `share/<cmakepackagename>/vcpkg-cmake-wrapper.cmake`               | CMake `find_package(<cmakepackagename>)` override                 |
| [`share/pkgconfig`](#layout-pkgconfig)                             | Configuration-independent pkgconfig files (`.pc`)                 |
| [`tools/<port>`](#layout-tools)                                    | Executable tools                                                  |


### <a name="layout-bin"></a> `bin` and `debug/bin` directories

On Windows, these directories contain DLL and PDB files for release and debug configuration
respectively. Any executable file produced by a port should be moved to a
[`tools/<port>`](#layout-tools) directory.

### <a name="layout-include"></a> `include`

Contains header files (`.h`, `.hpp`, `.hxx`). The layout under this directory
should reflect the intended usage of the package's header files. For example, a `contoso`
library that intends to use `#include <contoso/contoso.h>` should provide the
header file `include/contoso/contoso.h`.

vcpkg forbids installing some reserved header file names in the root of the
`include` directory, for example: `err.h`, `user.h`, `time.h`, and others.
Libraries that provide a forbidden header file name should place their header
files inside a `include/<port>` directory. If the library intends to replace a
system header file, it should set the `VCPKG_POLICY_ALLOW_RESTRICTED_HEADERS`
policy in its `portfile.cmake`.

### <a name="layout-lib"></a> `lib` and `debug/lib` directories

Contains static libraries, import libraries (on Windows), and shared libraries
(on non-Windows).

### <a name="layout-manual-link"></a> `lib/manual-link` and `debug/lib/manual-link` directories

Contains libraries that must be manually linked.

Files that may cause issues when linked automatically must be placed in the
`lib/manual-link` folders instead of the `lib` directory. For example, if a
library is intended to define the `main()` function for a program.

### <a name="layout-pkgconfig"></a> `lib/pkgconfig`, `debug/lib/pkgconfig` and `share/pkgconfig` directories

Contains pkgconfig integration files (`.pc`). A library should not provide
configuration-dependent and configuration-independent files at the same time.
For example: don't install `lib/pkgconfig/contoso.pc` and `share/pkgconfig/contoso.pc`.

### <a name="layout-plugins"></a> `plugins/<group>` and `debug/plugins/<group>`

Contains shared libraries that are meant to be loaded during runtime by consuming
applications.

### <a name="layout-share"></a> `share/<port>`

Contains miscellaneous files installed by each port.
For example, SPDX files, scripts, etc.

### <a name="layout-copyright"></a> `share/<port>/copyright`

vcpkg expects ports to provide a `copyright` file that contains the license
information of the installed package. See the [maintainer
guide](../contributing/maintainer-guide.md#install-copyright-file)
for more information.

### <a name="layout-usage"></a> `share/<port>/usage`

A text file with instructions to integrate a library within a project.
See the [guide to provide usage documentation for packages](../maintainers/handling-usage-files.md)
for more information.

### <a name="layout-cmake"></a> `share/<lowercase-package>/<package>Config.cmake`, `share/<package>/<package>-config.cmake`

CMake integration files should be placed in the `share` folder and respect 
CMake's rules for
[`find_package(package)`](<https://cmake.org/cmake/help/latest/command/find_package.html#config-mode-search-procedure>)
in `CONFIG` mode.

For example, if a port expect to provide `find_package(MyPackage REQUIRED)`, it
must provide either `share/mypackage/MyPackageConfig.cmake` or `share/mypackage/MyPackage-config.cmake`.

If a package provides CMake integration files, the
[`vcpkg_cmake_config_fixup()`](../maintainers/functions/vcpkg_cmake_config_fixup.md)
helper function should be invoked to fix any non-relocatable paths and to merge
build configurations.

### <a name="layout-tools"></a> `tools/<port>`

> [!IMPORTANT]
> vcpkg is first and foremost a C++ library dependency manager. Port authors should
> be deliberate when deciding to include tools in the installation output. For example:
> consider installing only a release executable when the debug tool is not needed.

Contains executable tools produced by a port. It is highly recommended,
but not required, that each installed executable goes into a subdirectory
matching the name of the port that produced it. For example, a `contoso`
port might install a `ContosoGenerator.exe` to `tools/contoso/ContosoGenerator.exe`.

Some ports require that their executables go into a `bin` subdirectory, in which
case the recommended pattern is `tools/<port>/bin`.
