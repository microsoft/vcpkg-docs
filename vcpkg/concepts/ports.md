---
title: Ports
description: Ports as a vcpkg concept
author: JavierMatosD
ms.author: javiermat
ms.date: 09/21/2023
ms.prod: vcpkg
ms.topic: concept-article
---

# Concept: Ports

A port in vcpkg is a versioned recipe that produces a set of files. It also imposes constraints on the install graph, ensuring compatible dependency resolution. At a minimum, a port contains:

* Metadata about the package: name, version, supported features, dependencies, etc.
* Instructions to acquire, build if necessary, and install the package.

Additionally, ports may include:

* Patches to apply to the package's source code
* Instructions to integrate with your project's build system

## Portfile (`portfile.cmake`)

All ports must contain a portfile (a file named `portfile.cmake`). A portfile is a script that contains instructions on how to build and install a package within the vcpkg environment. Some common tasks performed by portfiles are:

* Download a package's source code.
* Build the package using the upstream build system.
* Copy the build output to the vcpkg installation tree.
* Install build system integration files.

## vcpkg.json

The `vcpkg.json` file describes metadata about the package contained in the vcpkg port. This file defines attributes like the package’s name, version, description, dependencies, supported features and platforms, etc.
See the [`vcpkg.json`](../reference/vcpkg-json.md) file reference to learn more.

## Patch files

In certain scenarios, a port might include patch files. These files carry modifications to the library's source code, which are essential to either fix existing issues or to maintain compatibility with other components. Before the build process, patches are applied as part of the portfile execution steps, aiding in a seamless and error-free library integration.

## Port categories

### Standard port

A standard port serves as a recipe for building libraries from source or providing header files for header-only libraries. It contains the instructions and metadata needed to acquire, build, and install the library. The vast majority of the libraries in the curated registry are standard ports.

### Meta port

A meta port serves to impose constraints on the install graph without having its own build files. These ports are often used to group a set of related libraries under a single name or to serve as a layer of indirection for replaceable components. For example, `boost` is a meta-port that groups together various Boost libraries under a single name for easier dependency management.

### Script port

A script port contains utility scripts that are primarily intended for internal tasks within vcpkg or for consumption by other ports. These ports enable versioning of the utility scripts, thereby maintaining compatibility and standardization. For example, `vcpkg-cmake` is a script port that exposes  the `vcpkg_cmake_configure()`
helper function.

### Tool port

A tool port consists of recipes for building specialized tools or executables. While these are primarily consumed by other ports, they may sometimes have external dependencies and come with additional configuration files for customization.

## Next steps

This document provides an overview of ports in vcpkg, detailing their essential elements and various categories. For more advanced topics and detailed guides, refer to the following:

* [Script ports](..\maintainers\authoring-script-ports)
* [Tool ports]()
