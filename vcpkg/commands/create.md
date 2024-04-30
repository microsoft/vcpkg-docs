---
title: vcpkg create
description: Reference for the vcpkg create command. Generates a port to package a source code project.
author: JavierMatosD
ms.author: javiermat
ms.date: 01/26/2024
ms.service: vcpkg
---

# vcpkg create

## Synopsis

```no-highlight
vcpkg create <port-name> <url-to-source> [archive-file-name]
```

## Description

Generates a port to package a source code project. Requires a port name and a URL for the source code to package.

This command downloads the source code from the provided URL and then creates a `portfile.cmake` file and a `vcpkg.json` file in a new port directory.

The command can save the downloaded source code using a specific file name, provided as an optional argument. If the archive file name isn't specified, the command derives a file name from its URL.

It's important to understand that the port created by the `vcpkg create` command serves merely as a starting point and, in most cases, further edits are necessary for a successful build. For more guidance on adding ports to the vcpkg curated catalog, we recommend referring to one of our [tutorials](../examples/packaging-zipfiles.md)

## Example

```no-highlight
vcpkg create zlib2 https://github.com/madler/zlib/archive/v1.2.11.tar.gz zlib-1.2.11.tar.gz

-- Downloading https://github.com/madler/zlib/archive/v1.2.11.tar.gz -> zlib-1.2.11.tar.gz...
-- Generated portfile: C:\src\vcpkg\ports\zlib2\portfile.cmake
-- Generated manifest: C:\src\vcpkg\ports\zlib2\vcpkg.json
-- To launch an editor for these new files, run
--     .\vcpkg edit zlib2
```

The command performs the following actions:

1. Creates a port folder named `zlib2` containing a `portfile.cmake` file and a `vcpkg.json` file.
2. Downloads `https://github.com/madler/zlib/archive/v1.2.11.tar.gz` as `zlib-1.2.11.tar.gz` in the `downloads` directory.

The new `portfile.cmake` has the content:

```
# Common Ambient Variables:
#   CURRENT_BUILDTREES_DIR    = ${VCPKG_ROOT_DIR}\buildtrees\${PORT}
#   CURRENT_PACKAGES_DIR      = ${VCPKG_ROOT_DIR}\packages\${PORT}_${TARGET_TRIPLET}
#   CURRENT_PORT_DIR          = ${VCPKG_ROOT_DIR}\ports\${PORT}
#   CURRENT_INSTALLED_DIR     = ${VCPKG_ROOT_DIR}\installed\${TRIPLET}
#   DOWNLOADS                 = ${VCPKG_ROOT_DIR}\downloads
#   PORT                      = current port name (zlib, etc)
#   TARGET_TRIPLET            = current triplet (x86-windows, x64-windows-static, etc)
#   VCPKG_CRT_LINKAGE         = C runtime linkage type (static, dynamic)
#   VCPKG_LIBRARY_LINKAGE     = target library linkage type (static, dynamic)
#   VCPKG_ROOT_DIR            = <C:\path\to\current\vcpkg>
#   VCPKG_TARGET_ARCHITECTURE = target architecture (x64, x86, arm)
#   VCPKG_TOOLCHAIN           = ON OFF
#   TRIPLET_SYSTEM_ARCH       = arm x86 x64
#   BUILD_ARCH                = "Win32" "x64" "ARM"
#   DEBUG_CONFIG              = "Debug Static" "Debug Dll"
#   RELEASE_CONFIG            = "Release Static"" "Release DLL"
#   VCPKG_TARGET_IS_WINDOWS
#   VCPKG_TARGET_IS_UWP
#   VCPKG_TARGET_IS_LINUX
#   VCPKG_TARGET_IS_OSX
#   VCPKG_TARGET_IS_FREEBSD
#   VCPKG_TARGET_IS_ANDROID
#   VCPKG_TARGET_IS_MINGW
#   VCPKG_TARGET_EXECUTABLE_SUFFIX
#   VCPKG_TARGET_STATIC_LIBRARY_SUFFIX
#   VCPKG_TARGET_SHARED_LIBRARY_SUFFIX
#
#   See additional helpful variables in /docs/maintainers/vcpkg_common_definitions.md

# Also consider vcpkg_from_* functions if you can; the generated code here is for any web accessable
# source archive.
#  vcpkg_from_github
#  vcpkg_from_gitlab
#  vcpkg_from_bitbucket
#  vcpkg_from_sourceforge
vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/madler/zlib/archive/v1.2.11.tar.gz"
    FILENAME "zlib-1.2.11.tar.gz"
    SHA512 104c62ed1228b5f1199bc037081861576900eb0697a226cafa62a35c4c890b5cb46622e399f9aad82ee5dfb475bae26ae75e2bd6da3d261361b1c8b996970faf
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
    # (Optional) A friendly name to use instead of the filename of the archive (e.g.: a version number or tag).
    # REF 1.0.0
    # (Optional) Read the docs for how to generate patches at:
    # https://github.com/microsoft/vcpkg-docs/blob/main/vcpkg/examples/patching.md
    # PATCHES
    #   001_port_fixes.patch
    #   002_more_port_fixes.patch
)

# # Check if one or more features are a part of a package installation.
# # See /docs/maintainers/vcpkg_check_features.md for more details
# vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
#   FEATURES # <- Keyword FEATURES is required because INVERTED_FEATURES are being used
#     tbb   WITH_TBB
#   INVERTED_FEATURES
#     tbb   ROCKSDB_IGNORE_PACKAGE_TBB
# )

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    # OPTIONS -DUSE_THIS_IN_ALL_BUILDS=1 -DUSE_THIS_TOO=2
    # OPTIONS_RELEASE -DOPTIMIZE=1
    # OPTIONS_DEBUG -DDEBUGGABLE=1
)

vcpkg_cmake_install()

# # Moves all .cmake files from /debug/share/zlib2/ to /share/zlib2/
# # See /docs/maintainers/ports/vcpkg-cmake-config/vcpkg_cmake_config_fixup.md for more details
# When you uncomment "vcpkg_cmake_config_fixup()", you need to add the following to "dependencies" vcpkg.json:
#{
#    "name": "vcpkg-cmake-config",
#    "host": true
#}
# vcpkg_cmake_config_fixup()

# Uncomment the line below if necessary to install the license file for the port
# as a file named `copyright` to the directory `${CURRENT_PACKAGES_DIR}/share/${PORT}`
# vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

```

The new `vcpkg.json` has the content:

```
{
  "name": "zlib2",
  "version": "",
  "homepage": "",
  "description": "",
  "dependencies": [
    {
      "name": "vcpkg-cmake",
      "host": true
    }
  ],

  "default-features": [],
  "features": [
    {
      "name": "",
      "description": "",
      "dependencies": []
    }
  ]
}
```

You can use the [`edit`](edit.md) command to modify the port files:

```no-highlight
vcpkg edit zlib2
```
