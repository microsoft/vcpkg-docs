---
title: vcpkg_list
description: A replacement for CMake's list function which improves handling of elements with internal semicolons, such as other lists.
ms.date: 01/10/2024
---
# vcpkg_list

A replacement for CMake's `list()` function which improves handling of elements with internal semicolons, such as other lists.

## Usage

```cmake
vcpkg_list(SET <out-var> [<element>...])
vcpkg_list(<COMMAND> <list-var> [<other-arguments>...])
```

## Notes

Use `vcpkg_list()` instead of `list()` whenever it is a requirement to correctly handle lists of lists.

`vcpkg_list()` supports all the subcommands of the built-in `list()` function, with the following restrictions:

- `GET`, `REMOVE_ITEM`, and `REMOVE_AT` support only one index/value
- `POP_BACK` and `POP_FRONT` do not support getting the value into
  another out variable. Use C++ style `GET` then `POP_(BACK|FRONT)`.
- `FILTER` and `TRANSFORM` are unsupported.

`vcpkg_list` also adds a `vcpkg_list(SET)` subcommand. This subcommand supports correctly creating a list of lists.

See the [CMake documentation for `list()`](https://cmake.org/cmake/help/latest/command/list.html) for more information.

### Remarks on Zero-Element lists

The most major weirdness is due to `""` pulling double-duty as "list of zero elements",
and "list of one element, which is empty". `vcpkg_list` always uses the former understanding.
This can cause weird behavior, for example:

```cmake
set(lst "")
vcpkg_list(APPEND lst "" "")
# lst = ";"
```

This is because you're appending two elements to the empty list.
One very weird behavior that comes out of this would be:

```cmake
set(lst "")
vcpkg_list(APPEND lst "")
# lst = ""
```

since `""` is the empty list, we append the empty element and end up with a list
of one element, which is empty. This does not happen for non-empty lists;
for example:

```cmake
set(lst "a")
vcpkg_list(APPEND lst "")
# lst = "a;"
```

only the empty list has this odd behavior.

## Examples

### Creating a list

```cmake
vcpkg_list(SET foo_param)
if(DEFINED arg_FOO)
    vcpkg_list(SET foo_param FOO "${arg_FOO}")
endif()
```

### Appending to a list

```cmake
set(OPTIONS -DFOO=BAR)
if(VCPKG_TARGET_IS_WINDOWS)
    vcpkg_list(APPEND OPTIONS "-DOS=WINDOWS;FOO")
endif()
```

### Popping the end off a list

```cmake
if(NOT list STREQUAL "")
    vcpkg_list(GET list end -1)
    vcpkg_list(POP_BACK list)
endif()
```

## Source

[scripts/cmake/vcpkg\_list.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_list.cmake)
