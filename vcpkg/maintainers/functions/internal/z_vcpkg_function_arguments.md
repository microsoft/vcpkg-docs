---
title: z_vcpkg_function_arguments
---

# z_vcpkg_function_arguments

[!INCLUDE [internal-helper](../../../../includes/internal-helper.md)]

Get a list of function arguments suitable for perfect forwarding to another function.

## Usage

```cmake
z_vcpkg_function_arguments(<out-var> [<N>])
```

## Parameters
### `<out-var>`
The variable to set to the list of arguments

### `<N>`
The index to start accumulating arguments.

Defaults to `0` so all arguments will be gathered.

## Notes

This macro improves over `ARGV` by making it possible to distinguish list arguments.

|              | `ARGV`        | `z_vcpkg_function_arguments()` |
|--------------|---------------|--------------------------------|
| `fun(A B)`   | `"A;B"`       | `"A;B"`                        |
| `fun("A;B")` | `"A;B"`       | `"A\;B"`                       |

## Examples
```cmake
function(fun)
    z_vcpkg_function_arguments(ARGS)
    message("fun() is deprecated, use fun_replacement()")
    fun_replacement(${ARGS})
endfunction()
```

## Source
[scripts/cmake/z\_vcpkg\_function\_arguments.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/z_vcpkg_function_arguments.cmake)
