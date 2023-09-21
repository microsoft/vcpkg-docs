---
title: "Tutorial: Install a dependency from a Git-based registry"
description: Learn to install packages from vcpkg Git-based registries.
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.date: 9/21/2023
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to install a dependency from a Git-based registry
---
# Tutorial: Install a dependency from a Git-based registry

vcpkg uses a concept called registries to manage repositories of packages. By default, vcpkg finds
libraries from the public curated registry at <https://github.com/Microsoft/vcpkg>. You can add
third-party or private registries to make additional packages available to install.

For information on creating your own registries, read the [registries](../maintainers/registries.md)
documentation.

Registries are configured using a
[`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md) file.

In this tutorial, you will learn to:

> [!div class="checklist"]
> * [Create a project]
> * [Create the manifest and configuration files]
> * [Add private registries to your vcpkg configuration file]
> * [Install packages from custom registries]

## Prerequisites

* A terminal
* A C++ compiiler
* vcpkg
* CMake

## 1 - Create a project

In a new folder, create the following project files:

A source file (`main.cpp`):

:::code language="cpp" source="snippets/git-registries/main.cpp":::

A CMake project file (`CMakeLists.txt`):

:::code language="cmake" source="snippets/git-registries/CMakeLists.txt":::

## 2 - Create the manifest and configuration files

Run the following command:

```bash
vcpkg new --application
```

The `new` commmand creates two files: a manifest (`vcpkg.json`) file and a
configuration(`vcpkg-configuration.json`) file. Usually, the command requires the `--name` and
`--version` arguments to be provided, but since this tutorial is an end-user application, we use the
`--application` option instead.

The generated files will have the following contents:

`vcpkg.json`

```json
{}
```

`vcpkg-configuration.json`

```json
{
  "default-registry": {
    "kind": "git",
    "baseline": "7476f0d4e77d3333fbb249657df8251c28c4faae",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "artifact",
      "location": "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
      "name": "microsoft"
    }
  ]
}
```

The generated configuration file includes two registry definitions. The `default-registry` which points to
the curated vcpkg registry at <https://github.com/Microsoft/vcpkg>, using the latest commit as the
[baseline](../users/versioning.md#baselines), and an additional `"artifacts"` registry, which for the
purpose of this tutorial is unnecessary and can be deleted. 

## 3 - Add private registries to your vcpkg configuration file

The source code references two libraries that are not available in the vcpkg curated registry. In
order to satisfy these dependencies we need to add
<https://github.com/NorthWindTraders/vcpkg-registry> as an additional registry.

Modify the contents of `vcpkg-configuration.json` to:

```json
{
  "default-registry": {
    "kind": "git",
    "baseline": "7476f0d4e77d3333fbb249657df8251c28c4faae",
    "repository": "https://github.com/microsoft/vcpkg"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/northwindtraders/vcpkg-registry",
      "baseline": "dacf4de488094a384ca2c202b923ccc097956e0c",
      "packages": [ "beicode", "beison" ]
    }
  ]
}
```

The configuration file adds an external registry as the source for the `beicode` and `beison`
packages. Additional registries must explicitly declare the packages they provide using the
`"packages"` list.  When vcpkg resolves packages names to a registry, any package name not found in
an additional registry will be defaulted to the `"default-registry"`. Learn more about [package name
resolution](../users/registries.md#package-name-resolution) in the registries documentation.

## 3 - Install packages from a registry

Once a registry has been added to the configuration file, nothing special needs to be done to
install packages from it. vcpkg will transparently resolve package names in your `vcpkg.json` to the
correct registry when following the usual installation machinery.

Add the `beicode` and `beison` dependencies in your `vcpkg.json` file:

```bash
vcpkg add port beicode beison
```

Build and run the project (substitute `%VCPKG_ROOT%` with your vcpkg installation path):

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
cmake --build build
./build/Debug/main.exe
```

The output should look like this:

```Console
{
    "name": "demo"
}
```

## Next steps

* Lock down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reuse binaries across local or continuous integration runs using [binary caching](../users/binarycaching.md)
* Manage your private libraries using custom [registries](../maintainers/registries.md)
