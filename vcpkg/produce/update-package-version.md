---
title: "Tutorial: Update an existing vcpkg dependency"
description: This tutorial shows you how to update the version of an existing vcpkg dependency
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: tutorial
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections

#CustomerIntent: As a vcpkg user, I want to learn how to update the version of an existing vcpkg dependency
---

# Tutorial: Update an existing vcpkg dependency

This tutorial guides you on updating the version of an existing vcpkg
dependency.  We recommend that you read the tutorial on [publishing a
library](../get_started/get-started-adding-to-registry.md) before proceeding.

In this tutorial, you will learn to:

> [!div class="checklist"]
> * [Create an overlay port](#1---create-an-overlay-port)
> * [Modify the port's version](#2---modify-the-ports-version)
> * [Modify `porfile.cmake`](#3---modify-portfilecmake)
> * [Install your overlay port](#4---install-your-overlay-port)
> * [Update the built-in registry port](#5---update-the-built-in-registry-port)
> * [Open a Pull Request](#6---open-a-pull-request)

## Prerequisites

* A terminal
* vcpkg
* [Git](<https://git-scm.com/downloads>)
* A C++ compiler
* Completion of the [packaging
  tutorial](../get_started/get-started-packaging.md)

## 1 - Create an overlay port

The first step is to create an [overlay port](../concepts/overlay-ports.md) of
the package you want to modify.

### Create a directory to hold your overlay ports

The overlay ports directory can be created in any filesystem location of your
choosing. In any step of this tutorial, replace `$OVERLAY_LOCATION` with your
chosen location.

::: zone pivot="shell-powershell, shell-cmd"

```console
mkdir "$OVERLAY_LOCATION"
```

::: zone-end
::: zone pivot="shell-bash"

```bash
mkdir "$OVERLAY_LOCATION"
```

::: zone-end

### Copy the contents of the port into your overlay ports directory

For this tutorial, you'll update the `vcpkg-sample-library` port in the
[publishing a package
tutorial](../get_started/get-started-adding-to-registry.md) to a version that
has dynamic library support.

::: zone pivot="shell-powershell"

```powershell
Copy-Item -Path <path/to/vcpkg-sample-library> -Destination "$OVERLAY_LOCATION" -Recurse
```

::: zone-end
::: zone pivot="shell-cmd"

```console
xcopy <path/to/vcpkg-sample-library> "$OVERLAY_LOCATION" /E
```

::: zone-end
::: zone pivot="shell-bash"

```bash
cp -R <path/to/vcpkg-sample-library> "$OVERLAY_LOCATION"
```

::: zone-end

## 2 - Modify the ports version

Change the version in `vcpkg.json` to `1.0.1`.

`vcpkg.json`

```json
{
  "name": "vcpkg-sample-library",
  "version": "1.0.1",
  "homepage": "https://github.com/microsoft/vcpkg-docs/tree/cmake-sample-lib",
  "description": "A sample C++ library designed to serve as a foundational example for a tutorial on packaging libraries with vcpkg.",
  "license": "MIT",
  "dependencies": [
    {
      "name" : "vcpkg-cmake",
      "host" : true
    },
    {
      "name" : "vcpkg-cmake-config",
      "host" : true
    },
    "fmt"
  ]
}
```

## 3 - Modify `portfile.cmake`

### Obtain the package SHA512

Run `vcpkg install --overlay-ports=$OVERLAY_LOCATION vcpkg-sample-library`, you
will get an error about the SHA512 of the package. Copy the value of the actual
hash in your portfile.

Example output:

```Console
Expected hash: 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
Actual hash: 1290bdba0f55fcdd702a3292ce00798b173fdffa0c0cc005e15dc88c6d2e7a1143c16f29fde0647b8bddd01a7c97299cc845f7dff22811a3326cf7c69f5957f3
```

### Modify `portfile.cmake`

Update the package's SHA512 with the correct value and make sure to remove the
`ONLY_STATIC_LIBRARY` limitation, since the new version of
`vcpkg-sample-library` adds support for building it as a dynamic library.

Your `portfile.cmake` file should look similar to:

`portfile.cmake`

```cmake
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Microsoft/vcpkg-docs
    REF e5c3f12d5e6ac6450f43aee898f7697a06280521
    SHA512 1290bdba0f55fcdd702a3292ce00798b173fdffa0c0cc005e15dc88c6d2e7a1143c16f29fde0647b8bddd01a7c97299cc845f7dff22811a3326cf7c69f5957f3
    HEAD_REF cmake-sample-lib
)
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)
vcpkg_cmake_install()
vcpkg_copy_pdbs()
vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" "${CURRENT_PACKAGES_DIR}/share/${PORT}/usage" COPYONLY)
```

## 4 - Install your overlay port

To verify that your port works, run the following command:

`vcpkg install "--overlay-ports=$OVERLAY_LOCATION" vcpkg-sample-library`

## 5 - Update the built-in registry port

### Replace the contents of the port

Replace the contents of `vcpkg-sample-library` in the `ports` directory with
your updated files. Then commit your changes by running the following commands
in your local clone of the vpckg repository:

```Console
git checkout -b vcpkg-sample-library-1.0.1
git add ports/vcpkg-sample-library
git commit -m "Update vcpkg-sample-library to version 1.0.1"
```

### Update the versions database

Run the [`vcpkg x-add-version`] command to update the versions database files.

```Console
vcpkg x-add-version vcpkg-sample-library
```

### Push your changes to a fork

Run the following commands to update the versions database and push your changes
to your fork of <https://github.com/Microsoft/vcpkg>.

```Console
git add versions/.
git commit -m "Update vcpkg-sample-library to version 1.0.1" --amend
git push --set-upstream <fork remote> vcpkg-sample-library-1.0.1
```

## 6 - Open a Pull Request

1. Navigate to your forked repository on GitHub.
2. Click the "Compare & pull request" button.
   1. Verify your changes
   2. Add a descriptive title and comments
   3. Fill out the [PR review checklist](../contributing/pr-review-checklist.md)
3. Click "Create pull request."

That's it! You've successfully updated a port in the vcpkg's curated registry.

## See also

For more information, see:

* [CMake guidelines](../contributing/cmake-guidelines.md)
* [Maintainer guide](../contributing/maintainer-guide.md)
* [Ports](../concepts/ports.md)
* [Publishing to a private git registry](../produce/publish-to-a-git-registry.md)
