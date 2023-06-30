---
title: Supplying usage for ports
description: Guidance for handling usage files.
author: JavierMatosD
ms.author: javiermat
ms.date: 06/30/2023
---
# Suppyling usage for ports

## Overview

Providing usage documentation for ports allows users to easily adopt them in their
projects. We highly encourage providing a `usage` file within the ports directory (`ports/<port
name>/usage`) that describes the minimal steps necessary to integrate with a buildsystem.

### Supplying a usage file

To provide usage documentation put a `usage` text file in the port's `share`
installation directory. The recommended approach is to call the `configure_file()` function in
`portfile.cmake`.

For example:

```cmake
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

This installs `${CURRENT_PACKAGES_DIR}/share/${PORT}/usage` which vcpkg will use to print usage
instructions for the port whenever it gets installed.

### Content format

When structuring the content of the usage files, it is important to focus on providing clear instructions that cover the essential aspects of using the package. The content should be concise, well-structured, and emphasize the minimum build system integration required to ensure the basic functionality of the library.

While it can be tempting to include extensive code snippets, command-line instructions, or configuration details, it is advisable to avoid overwhelming users with excessive information. Instead, prioritize clarity and conciseness in explaining the fundamental steps to utilize the package effectively.

For comprehensive and detailed information, users can refer to the package's [documentation](../users/manifests.md).

Usage files should follow a pattern similar to the following templates:

Packages with CMake targets:

```no-highlight
<port> provides CMake targets:

    <instructions>
```

Header-only packages:

```no-highlight
<port> is header-only and can be used from CMake via:

    <instructions>
```
