---
title: vcpkg_get_python_packages
description: Learn how to use vcpkg_get_python_packages.
ms.date: 11/30/2022
---
# x_vcpkg_get_python_packages

[!INCLUDE [experimental](../../../includes/experimental.md)]

Retrieve needed python packages.

## Usage

```cmake
x_vcpkg_get_python_packages(
    [PYTHON_VERSION (2|3)]
    PYTHON_EXECUTABLE <path to python binary>
    REQUIREMENTS_FILE <file-path>
    PACKAGES <packages to acquire>...
    [OUT_PYTHON_VAR some_var]
)
```

## Parameters

### PYTHON_VERSION

Python version to be used.

Must be either 2 or 3.

### PYTHON_EXECUTABLE

Full path to the python executable

### REQUIREMENTS_FILE

Requirement file with the list of python packages

### PACKAGES

List of python packages to acquire

### OUT_PYTHON_VAR

Variable to store the path to the python binary inside the virtual environment

## Source

[ports/vcpkg-get-python-packages/x\_vcpkg\_get\_python\_packages.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-get-python-packages/x_vcpkg_get_python_packages.cmake)
