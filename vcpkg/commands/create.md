---
title: vcpkg create
description: Command line reference for the vcpkg create command. The `vcpkg create` command is used to generate a new vcpkg port from a specified URL.
ms.date: 08/01/2023
---

# vcpkg create

## Synopsis

```no-highlight
vcpkg create <port-name> <url-to-source> [archive-file-name]
```

## Description

The `vcpkg create` command is a tool that facilitates the generation of new ports for libraries within the vcpkg ports tree. To do this, it requires a port name and a URL that points to the source code of the library in question.

Upon execution, this command initiates a download of the source code from the provided URL. It then creates an initial "portfile.cmake" and "vcpkg.json" in a newly formed directory.

The command has the capability to save the downloaded source code using a specific file name, provided as an optional argument. If this archive file name isn't specified, the command automatically derives the file name from URL.

Consider the example below:
```no-highlight
vcpkg create zlib2 https://github.com/madler/zlib/archive/v1.2.11.tar.gz zlib-1.2.11.tar.gz
```

In this case, the `vcpkg create` command performs the following actions:

1. Creates a port named `zlib2` accompanied by a "portfile.cmake" and a "vcpkg.json"
2. Downloads the  `v1.2.11.tar.gz` file from the given URL
3. Saves the source files as `zlib-1.2.11.tar.gz` in the downloads directory.

portfile.cmake:
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
# 	See additional helpful variables in /docs/maintainers/vcpkg_common_definitions.md

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
    # https://github.com/Microsoft/vcpkg/blob/master/docs/examples/patching.md
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

vcpkg.json:
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

To begin editing the newly created files, execute the following command:

```no-highlight
vcpkg edit zlib2
```

It's important to understand that the port created by the `vcpkg create` command serves merely as a starting point and, in most cases, further edits are necessary for a successful build.

Be aware that the `vcpkg create` command doesn't verify the uniqueness of th eport name within the vcpkg ports tree. The purpose of this command is to provide a foundation for port creation. If you require more guidance on adding ports to the vcpkg curated catalog, we recommend referring to one of our [tutorials](..\examples\packaging-zipfiles.md)


