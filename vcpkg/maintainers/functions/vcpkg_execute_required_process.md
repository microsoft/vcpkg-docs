---
title: vcpkg_execute_required_process
description: Execute a process with logging and fail the build if the command fails.
ms.date: 06/27/2024
---
# vcpkg_execute_required_process

Execute a process with logging and fail the build if the command fails.

## Usage

```cmake
vcpkg_execute_required_process(
    COMMAND <${PERL}> [<arguments>...]
    WORKING_DIRECTORY <${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-dbg>
    LOGNAME <build-${TARGET_TRIPLET}-dbg>
    [TIMEOUT <seconds>]
    [OUTPUT_VARIABLE <var>]
    [ERROR_VARIABLE <var>]
    [SAVE_LOG_FILES [<relative-path> [ALIAS <unique-alias>]]...]
)
```

## Parameters

### ALLOW_IN_DOWNLOAD_MODE

Allows the command to execute in Download Mode.

### COMMAND

The command to be executed, along with its arguments.

### WORKING_DIRECTORY

The directory to execute the command in.

### LOGNAME

The prefix to use for the log files. This should be a unique name for different triplets so that the logs don't conflict when building multiple at once.

### TIMEOUT

Optional timeout after which to terminate the command.

### OUTPUT_VARIABLE

Optional variable to receive stdout of the command.

### OUTPUT_STRIP_TRAILING_WHITESPACE

Removes trailing whitespace before setting `OUTPUT_VARIABLE`.

### ERROR_VARIABLE

Optional variable to receive stderr of the command.

### ERROR_STRIP_TRAILING_WHITESPACE

Removes trailing whitespace before setting `ERROR_VARIABLE`.

### SAVE_LOG_FILES

Optional files to be moved from the working directory to `${CURRENT_BUILDTREES_DIR}`.

This helps to collect relevant log files in CI setups. The files are copied even if the process failed.
The target file names are constructed from the `LOGNAME` parameter and the source filename.
If the target file name doesn't end in `.log`, this suffix is appended.

_Added in vcpkg version 2023.01.10_

The `ALIAS` parameter after a relative path name replaces the target file name generation with the value of `<unique-alias>`.

## Examples

- [boost-build](https://github.com/Microsoft/vcpkg/blob/master/ports/boost-build/portfile.cmake)
- [ffmpeg](https://github.com/Microsoft/vcpkg/blob/master/ports/ffmpeg/portfile.cmake)
- [vcpkg-cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-cmake/vcpkg_cmake_configure.cmake)

## Source

[scripts/cmake/vcpkg\_execute\_required\_process.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_execute_required_process.cmake)
