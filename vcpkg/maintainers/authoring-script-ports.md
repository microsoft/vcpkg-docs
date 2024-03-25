---
title: Authoring script ports
description: Learn to create a script port to reuse functions in multiple vcpkg portfiles.
author: viromer
ms.author: vicroms
ms.date: 03/20/2024
ms.topic: concept-article
---
# Author helper ports

Helper ports, also called script ports, expose functions for other ports to
consume during their build process. For instance, the `vcpkg-cmake` port defines
the `vcpkg_cmake_configure()` function for other ports to consume. By packaging
common scripts into a helper port, maintenance becomes more streamlined as
updates can be made in a single location. Additionally, helper ports can be
versioned and depended upon using the same mechanisms as regular ports.

## How do they work?

Helper ports are implemented via the `vcpkg-port-config.cmake` extension
mechanism.

Before a port is executed, vcpkg will import any `vcpkg-port-config.cmake` file
that has been exported by the direct dependencies of the port about to be
executed.

If a helper port depends on a different helper port, it must explicity import the
`vcpkg-port-config.cmake` file of its dependency. Helper-to-helper port
dependencies should not be marked as [host
dependencies](../reference/vcpkg-json.md#dependency-host), this ensures that one
script can depend upon the other being in the same install directory.

Ports that depend on a helper port should mark the dependency as a [host
dependency](../reference/vcpkg-json.md#dependency-host).

Helper ports must always install their `vcpkg-port-config.cmake` file in a
`share/${PORT}` subdirectory in the [installation
tree](../reference/installation-tree-layout.md). 

## Example: Write a simple helper port

1 - Create a CMake file that defines the helper function.

`my-helper/my_helper_function.cmake`

```cmake
include_guard(GLOBAL)

function(my_helper_function)
  message(STATUS "my_helper_function() was called")
  my_other_helper_function()
endfunction()
```

The `include_guard(GLOBAL)` at the top of the file protect against redefining
this function when the file is included multiple times.

The following lines declare a function named `my_helper_function` that displays
a message and calls the `my_other_helper_function` that is defined in a
different helper port. 

2 - Create the helper port's `porftile.cmake` file

`my-helper/portfile.cmake`

```cmake
set(VCPKG_POLICY_HELPER_PORT enabled)

file(INSTALL
  "${CMAKE_CURRENT_LIST_DIR}/vcpkg-port-config.cmake"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

file(INSTALL "${VCPKG_ROOT_DIR}/LICENSE.txt" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
```

By enabling the `VCPKG_POLICY_HELPER_PORT` policy, vcpkg disables post-build checks
that warn when a port does not produce any binary output.

The next lines install the required `vcpkg-port-config.cmake` and `copyright`
files in their correct location (`share/${PORT}`).

3 - Create the helper port's `vcpkg.json` file

`my-helper/vcpkg.json`

```json
{
  "name": "my-helper",
  "version-date": "2024-03-20",
  "description": "Provide my_helper_function()",
  "license": "MIT",
  "dependencies": [ 
    { "name": "my-other-helper" } 
  ]
}
```

We recommend using `version-date` as the [versioning
scheme](../users/versioning.md#version-schemes) for helper ports. 

The `dependencies` in this example contain a reference to another helper port
named `my-other-helper`. The dependency is purposefuly not marked as a host
dependency since this is a helper-to-helper port dependency.

4 - Create the helper port's `vcpkg-port-config.cmake`file

`my-helper/vcpkg-port-config.cmake`

```cmake
include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/../my-other-helper/vcpkg-port-config.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/my_helper_function.cmake")
```

The `vcpkg-port-config.cmake` file consists of three lines, the first is a
global include guard that prevents the file from being included multiple times. 

The second line includes `my-other-helper/vcpkg-port-config.cmake` to make the
functions in `my-other-helper` available to ports that depend on `my-helper`
without them having to include `my-other-helper` in their list of direct
dependencies.

Finally, the `my_helper_function.cmake` file which contains the `my_helper_function`
definition is included.

5 - Consume `my-helper` in a manifest

Any consumer of `my-helper` only needs to include a direct dependency to
`my-helper` itself, no dependency to `my-other-helper` is needed. The consuming
manifest should mark the dependency as a host dependency.

`my-port/vcpkg.json`

```json
{
  "name": "my-port",
  "version": "1.0.0",
  "dependencies": [
    {
      "name": "my-helper",
      "host": true
    }
  ]
}
```

This makes `my_helper_function` available in `my-port/portfile.cmake`.

`my-port/portfile.cmake`

```cmake
my_helper_function()
```
