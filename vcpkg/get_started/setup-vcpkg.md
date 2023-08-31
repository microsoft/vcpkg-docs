---
title: Setup vckg on Linux
description: Tutorial guides the user through the process of setting up vcpkg on Linux.
author: JavierMatosD
ms.date: 08/31/2023
ms.author: javiermat
ms.prod: vcpkg
---
# Setup vcpkg on Linux

## Description

This tutorial provides a step-by-step guide on setting up vcpkg, a C++ package manager, on a Linux system. It covers everything from cloning the vcpkg repository and running the bootstrap script to integrating vcpkg into the bash shell for tab-completion features.

## Clone the repository

The first step is to clone the vcpkg repository. The repository contains scripts to acquire the vcpkg executable and a registry of open-source packages that is curated and maintained by the vcpkg team and it's community of contributors. To do this, run:

```bash
git clone https://github.com/microsoft/vcpkg.git
```

The curated registry is a set of libraries that have been vetted and packaged to work smoothly with vcpkg. vcpkg uses "ports" to package open-source projects and make them available to install. Ports typically contain:

1. `portfile.cmake`: A CMake script that contains instructions for downloading, building, and installing the library.

2. `vcpkg.json`: A JSON file including package metadata: package version, dependencies, supported platforms and features, etc.

3. Patches: Sometimes a port will contain patches to make the library work with vcpkg or to provide bug fixes in the library itself.

Once you have the repository, navigate to the `vcpkg` directory and execute the bootstrap script:

## Bootstrap and integrate

```bash
cd vcpkg && ./bootstrap-vcpkg.sh
```

The shell script performs prerequisite checks, downloads the vcpkg executable, and manages telemetry settings.

You'll want to integrate vcpkg into your bash shell to enjoy tab-completion features. To do this, run:

```bash
./vcpkg integrate bash
```

The `vcpkg integrate bash` command sets up bash shell integration for vcpkg, which adds vcpkg tab-completion support to the current user's `.bashrc`.

That's it! vcpkg is setup and ready to use.
