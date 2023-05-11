---
title: Handling Usage Files
description: Guidance for handling usage files.
ms.date: 05/11/2023
---

# Handling Usage Files

## Overview
Usage files play an important role in providing instructions, examples, or guidelines for utilizing packages within our repository. To ensure consistency and clarity, we have established the following guidelines for handling usage files:

**Using `configure_file()` Function**

The recommended approach for handling usage files is by utilizing the `configure_file()` function provided by CMake. This function allows you to specify the source usage file and the destination path where it should be copied. 

For example:

```
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

This code snippet will copy the `usage` file located in the current directory to the destination path. `${CURRENT_PACKAGES_DIR}/share/${PORT}/usage`.

**Naming Convention** 

To maintain consistency, it is advisable to name the usage files as `usage` within the respective packages directories. This naming convention will make it easier for contributors and users to locate and refer to the usage files.

**Content Format** 

The content of the usage files should provide clear instructions, examples, or guidelines on how to effectively use the package. It should be concise, well-structured, and include any necessary code snippets, command-line instructions, or configuration details. 

For example:
```
The package adios2 provides CMake targets:

    find_package(adios2 CONFIG REQUIRED)
    target_link_libraries(main PRIVATE adios2::adios2)
```

**Update and Synchronization** 

It is important to keep the usage files up to date with the latest changes and additions to the package. Whenever modifications are made to the usage instructions, ensure that the usage files are synchronized accordingly. 




