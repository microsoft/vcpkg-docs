---
title: Provide usage documentation for your packages
description: Guidance for adding usage documentation to packages
author: JavierMatosD
ms.author: javiermat
ms.date: 07/10/2023
---
# Provide usage documentation for your packages

## Overview

Providing usage documentation for packages allows users to easily adopt them in their
projects. We highly encourage providing a `usage` file within the ports directory (`ports/<port
name>/usage`) that describes the minimal steps necessary to integrate with a build system.

### Supplying a usage file

To provide usage documentation just put a text file named `usage` in the port's `share`
installation directory. The recommended method is to call the `configure_file()` function in
`portfile.cmake`.

For example:

```cmake
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

After installing packages, vcpkg detects files installed to `${CURRENT_PACKAGES_DIR}/share/${PORT}/usage` and prints their usage instructions.

### Content format

Provide clear instructions on how to use the package. The content should be concise, well-structured, and emphasize the minimum build system integration required to ensure the basic functionality of the library.

Be clear and concise about how to utilize the package effectively. Avoid overwhelming users with code snippets, command-line instructions, or configuration details. Instead, use the [`"documentation"` property in the port's `vcpkg.json` file](../users/manifests.md) so users can learn more about the specifics of your library.

Use the following templates as a pattern for your usage files:

Packages with CMake targets:

```text
<port> provides CMake targets:

    <instructions>
```

Header-only packages:

```text
<port> is header-only and can be used from CMake via:

    <instructions>
```
