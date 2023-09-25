---
title: "Tutorial: Install a specific version of a package"
description: Learn to install specific versions of your dependencies using vcpkg in your projects.
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.date: 9/20/2023
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to learn how to install specific versions of my project dependencies using vcpkg
---
# Tutorial: Install a specific version of a package

> [!IMPORTANT]
> This feature is only available in [manifest mode](manifest-mode.md).

vcpkg allows you to control the precise versions of each dependency in your project. 

In this tutorial, you will learn to:

> [!div class="checklist"]
> * [Create a project with a manifest]
> * [Add version constraints using a baseline]
> * [Update the baseline versions]
> * [Add a minimum version constraint]
> * [Force a specific version]

## Prerequisites

* A terminal
* A code editor
* vcpkg
* CMake

## 1 - Create a project with a manifest

In an empty folder, create the following project files:

A source file (`main.cpp`):

:::code language="cpp" source="snippets/versioning-main.cpp":::

A CMake project file (`CMakeLists.txt`):

:::code language="cmake" source="snippets/versioning-cmakelists.txt":::

A vcpkg manifest (`vcpkg.json`):

```json
{
  "dependencies": [ "fmt", "zlib" ]
}
```

Build the project, replace `%VCPKG_ROOT%` with your vcpkg installation path:

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Run the program:

```Console
fmt version is 70103
zlib version is 1.2.11
```

It is likely, that when you run the program the versions of these libraries are different than the
output above. In the next step, we show you how to pin the versions of these dependencies so that
they stay consistent each time you build the project.

## 2 - Add version constraints using a baseline

A version baseline, establishes a minimum version floor for all the packages. Read the [vcpkg
concepts](../users/versioning.md#baselines) to learn about baselines.

To get the exact versions used in the previous step, modify the contents of `vcpkg.json` to:

```json
{
  "dependencies": [
    "fmt",
    "zlib"
  ],
  "builtin-baseline": "3426db05b996481ca31e95fff3734cf23e0f51bc"
}
```

Setting [`builtin-baseline`](../reference/vcpkg-json.md#builtin-baseline) to a specific commit SHA
of the vcpkg repository instructs vcpkg to use the package versions at that specific commit as the minimum
version for all the packages.

You can use Git to examine the versions for that particular baseline:

```bash
git show 3426db05b996481ca31e95fff3734cf23e0f51bc:versions/baseline.json | egrep -A 3 -e '"zlib"|"fmt"'
    "fmt": {
      "baseline": "7.1.3",
      "port-version": 1
    },
--
    "zlib": {
      "baseline": "1.2.11",
      "port-version": 9
    },
```

## 3 - Update the baseline versions

Baselines offer a convenient mechanism to update the versions of all your dependencies at once. To
update your baseline run the following command:

```bash
vcpkg x-update-baseline
```

The [`x-update-baseline`](../commands/update-baseline.md) command modifies your manifest file to set
[`builtin-baseline`](../reference/vcpkg-json.md#builtin-baseline) to the current Git commit of your
vcpkg instance.

You can use the `--add-initial-baseline` option to add a `builtin-baseline` to a manifest that does
not have one yet.

## 4 - Add a minimum version constraint

Baselines are not the only way to lock down a package's version. vcpkg also accepts minimum version
constraints in the form of `version>=`.

Modify the contents of `vcpkg.json` to:

```json
{
  "dependencies": [
    {
        "name": "fmt",
        "version>=": "10.1.1"
    },
    "zlib"
  ],
  "builtin-baseline": "3426db05b996481ca31e95fff3734cf23e0f51bc"
}
```

The manifest file above uses the [dependency object](../reference/vcpkg-json.md#dependency-fields)
notation to set a minimum version constraint (`version>=`) on `fmt`. In order to satisfy the
dependencies vcpkg needs to satisfy two constraints, one comfing from the baseline and one coming
from the minimum version constraint in the `dependencies` list.

* Baseline constraint, `"version>=": "7.1.3"`.
* Dependencies list constraint, `"version>=": "10.1.1"`.

Build and run the project, replace `%VCPKG_ROOT%` with your vcpkg installation path:

```bash
rm -r build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/main
```

The output should look like this:

```Console
fmt version is 100100
zlib version is 1.2.11
```

In this case, version `10.1.1` of `fmt` satisfies both constraints. Notice how `zlib` gets its
baseline version `1.2.11`.

## 5 - Force a specific version

In some cases, you may want to force a specific version of a package, for example: 

* To resolve version conflicts.
* To lock down versions older than the baseline.
* To lock down versions that are otherwise incomparable, for example: `vista`, `xp`. 

vcpkg allows you to solve these issues by using version overrides.

Modify the `vcpkg.json` contents to:

```json
{
  "dependencies": [
    {
        "name": "fmt",
        "version>=": "10.1.1"
    },
    "zlib"
  ],
  "builtin-baseline": "3426db05b996481ca31e95fff3734cf23e0f51bc", 
  "overrides": [
    { 
        "name": "zlib", 
        "version": "1.2.8"
    }
  ]
}
```

Any package included in the `"overrides"` list will use the specified version while ignoring all
other version constraints. In this example, the baseline `3426db05b996481ca31e95fff3734cf23e0f51bc`
adds a minimum version constraint on `zlib` of `1.2.11` but the override declaration forces version
`1.2.8` instead.

Build and run the project, replace `%VCPKG_ROOT%` with your vcpkg installation path:

```bash
rm -r build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/main
```

The output should look like this:

```Console
fmt version is 100100
zlib version is 1.2.8
```

## Next steps

In this tutorial, you learned the different mechanisms that vcpkg offers to lock down specific
package versions. Read the versioning [concepts](../users/versioning.concepts.md) and
[reference](../users/versioning.md) to learn more about how vcpkg handles version resolution.

Here are some additional tasks to try next:

* Reuse binaries across Continuous Integration runs using [binary caching](../users/binarycaching.md)
* Manage your private libraries using [custom registries](../maintainers/registries.md)

[Create a project with a manifest]: #1---create-a-project-with-a-manifest
[Add version constraints using a baseline]: #2---add-version-constraints-using-a-baseline
[Update the baseline versions]: #3---update-the-baseline-versions
[Add a minimum version constraint]: #4---add-a-minimum-version-constraint
[Force a specific version]: #5---force-a-specific-version
