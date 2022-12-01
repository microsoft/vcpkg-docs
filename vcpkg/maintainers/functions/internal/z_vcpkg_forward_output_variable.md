---
title: z_vcpkg_forward_output_variable
---

# z_vcpkg_forward_output_variable

[!INCLUDE [internal-helper](../../../../includes/internal-helper.md)]

Forward values from inner function calls into pointer out parameters through a local function scope.

## Usage

```cmake
z_vcpkg_forward_output_variable(<ptr_to_parent_var> <value_var>)
```

## Parameters
### `<ptr_to_parent_var>`

The name of a local variable set to the desired parent variable name.

Most commonly, this local is the result of a pointer-out parameter to a function.

### `<value_var>`
The name of a local variable that will be propagated out.

## Notes
The following two snippets are equivalent:
```cmake
z_vcpkg_forward_output_variable(ptr_to_parent_var value_var)
```
```cmake
if(DEFINED ptr_to_parent_var)
    if(DEFINED value_var)
        set("${ptr_to_parent_var}" "${value_var}" PARENT_SCOPE)
    else()
        unset("${ptr_to_parent_var}" PARENT_SCOPE)
    endif()
endif()
```

## Examples
```cmake
function(my_function out_var)
    file(SHA512 "somefile.txt" local_var)
    z_vcpkg_forward_output_variable(out_var local_var)
endfunction()
```

## Source
[scripts/cmake/z\_vcpkg\_forward\_output\_variable.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/z_vcpkg_forward_output_variable.cmake)
