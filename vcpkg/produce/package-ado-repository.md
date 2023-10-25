---
title: "Tutorial: Package a dependency from an Azure DevOps repository"
description: This tutorial shows how to create a vcpkg port to package a dependency from an Azure DevOps repository.
author: vicroms
ms.author: viromer
ms.date: 10/24/2023
ms.prod: vcpkg
ms.topic: tutorial
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections

#CustomerIntent: As a vcpkg user, I want to package a dependency hosted in an Azure DevOps repository
---

# Tutorial: Package a dependency from an Azure DevOps repository

This tutorial shows you how to create a vcpkg port to package a dependency
hosted in an Azure DevOps repository. We recommend that you read the [Tutorial:
Create and publish packages](../get_started/get-started-packaging.md) article
before proceeding.

In this tutorial we show you how to:

> [!div class="checklist"]
> * [Create a port directory](#1---create-a-port-directory)
> * [Create the port's manifest](#2---create-the-ports-manifest)
> * [Create the port's recipe script](#3---create-the-ports-recipe-script)

## Prerequisites

* A terminal
* A C++ compiler
* vcpkg
* Git
* A library hosted in an Azure DevOps repository

## 1 - Create a port directory

Create a new directory with the name you want to give to your port.

## 2 - Create the port's manifest

Inside the port's directory create a file named `vcpkg.json`, replace the
placeholders with the appropiate values for your package.

`vcpkg.json`

```json
{
    "name": "<your package's name>",
    "version": "<your package's version>",
    "description": "<your package's description>",
    "license": "<your package's license>",
    "dependencies": [ "<dependency1>", "<dependency2>" ]
}
```

See the [`vcpkg.json` reference](../reference/vcpkg-json.md) for detailed
descriptions of each property in the manifest file.

## 3 - Create the port's recipe script

Inside the port's directory, create a file named `portfile.cmake` and replace the
placeholders with the appropiate values for your package.

`portfile.cmake`

```cmake
vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    URL <your ado repository URL>
    REF "${VERSION}"
    HEAD_REF master
)

vcpkg_msbuild_install(
     SOURCE_PATH ${SOURCE_PATH}
     TARGET Build
)

file(GLOB HEADER_FILES "${SOURCE_PATH}/include/*.h")
file(INSTALL ${HEADER_FILES} DESTINATION "${CURRENT_PACKAGES_DIR}/include/${PORT}")
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/copyright" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
```

Creating a port to package an Azure DevOps follows the same process as packaging
any Git-hosted package.

The `portfile.cmake` provided here is a template to follow for your own packages. You may require additional commands to build your package and copy the output into your vcpkg installation folder.

## Next steps

And that's it! You have packaged a dependency from an Azure DevOps repository.

Here are some additional tasks to try next:

* [Set up your own private Git-based registry to use with vcpkg](publish-to-a-git-registry.md)
* [Install dependencies from a private registry](../users/registries.md)
* [Authenticate to registries using private Git repositories](../users/authentication.md)
