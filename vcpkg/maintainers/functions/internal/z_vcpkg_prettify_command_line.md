---
title: z_vcpkg_prettify_command_line
---

# z_vcpkg_prettify_command_line

[!INCLUDE [internal-helper](../../../../includes/internal-helper.md)]

Format a command line into a string.

## Usage

```cmake
z_vcpkg_prettify_command_line(<out-var> <argument>...)
```

## Parameters

### `<out-var>`
Variable to set to the formatted command line

### `<argument>...`

The command line to format.

This argument should be the same form as the [`COMMAND` argument of `vcpkg_execute_required_process()`](../vcpkg_execute_required_process.md#command).

## Examples
- [vcpkg_execute_build_process.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_execute_build_process.cmake)
- [vcpkg_execute_required_process.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_execute_required_process.cmake)
- [vcpkg_execute_required_process_repeat.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_execute_required_process_repeat.cmake)

## Source

[scripts/cmake/z\_vcpkg\_prettify\_command\_line.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/z_vcpkg_prettify_command_line.cmake)
