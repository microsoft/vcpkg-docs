---
title: Supplying Usage
description: Guidance for handling usage files.
ms.date: 05/11/2023
---

# Supplying Usage for Packages

## Overview
Providing usage documentation for vcpkg ports is highly valued as it allows users to easily adopt and utilize the packages within their projects. To this end, we highly encourage providing a text file named "usage" within the ports directory (i.e., `vcpkg/ports/<port-name>/usage`) describing the minimum buildsystem integration necessary to make the library function at all. It is intended for this file to be installed with the library.

### Supplying Usage Files

The recommended approach for handling usage files is by utilizing the `configure_file()` function provided by CMake. This function should be called from within the port's `portfile.cmake` and allows you to specify the source usage file and the destination path where it should be copied. 

For example:

```
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

This code snippet will copy the `usage` file to `${CURRENT_PACKAGES_DIR}/share/${PORT}/usage`.

### Content Format

When structuring the content of the usage files, it is important to focus on providing clear instructions, examples, or guidelines that cover the essential aspects of using the package. The content should be concise, well-structured, and emphasize the minimum build system integration required to ensure the basic functionality of the library.

While it can be tempting to include extensive code snippets, command-line instructions, or configuration details, it is advisable to avoid overwhelming users with excessive information. Instead, prioritize clarity and conciseness in explaining the fundamental steps necessary to utilize the package effectively.

For more comprehensive and detailed information, users can refer to the [documentation](../users/manifests.md).

The usage file should follow this template:

```
To use <package_name> from CMake:

    <instructions>
```

Here is an example using ZLIB:
```
To use ZLIB from CMake:

    find_package(ZLIB REQUIRED)
    target_link_libraries(main PRIVATE ZLIB::ZLIB)
```