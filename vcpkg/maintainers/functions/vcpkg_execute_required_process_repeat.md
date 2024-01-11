---
title: vcpkg_execute_required_process_repeat
description: Learn how to use vcpkg_execute_required_process_repeat.
ms.date: 01/10/2024
---
# vcpkg_execute_required_process_repeat

Execute a process until the command succeeds, or until the COUNT is reached.

## Usage

```cmake
vcpkg_execute_required_process_repeat(
    COMMAND <cmd> [<arguments>]
    COUNT <num>
    WORKING_DIRECTORY <directory>
    LOGNAME <name>
    [ALLOW_IN_DOWNLOAD_MODE]
)
```

## Source

[scripts/cmake/vcpkg\_execute\_required\_process\_repeat.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_execute_required_process_repeat.cmake)
