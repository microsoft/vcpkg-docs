---
title: vcpkg_copy_tool_dependencies
description: Learn how to use vcpkg_copy_tool_dependencies.
ms.date: 11/30/2022
---
# vcpkg_copy_tool_dependencies

Copy all DLL dependencies of built tools into the tool folder.

## Usage

```cmake
vcpkg_copy_tool_dependencies(<${CURRENT_PACKAGES_DIR}/tools/${PORT}>)
```
## Parameters

The path to the directory containing the tools.

## Notes

This command should always be called by portfiles after they have finished rearranging the binary output, if they have any tools.

## Examples

- [glib](https://github.com/Microsoft/vcpkg/blob/master/ports/glib/portfile.cmake)
- [fltk](https://github.com/Microsoft/vcpkg/blob/master/ports/fltk/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_copy\_tool\_dependencies.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_copy_tool_dependencies.cmake)

