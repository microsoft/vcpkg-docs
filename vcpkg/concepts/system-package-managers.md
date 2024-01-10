---
title: System package managers
description: Concepts on system package managers
author: data-queue
ms.author: danshaw2
ms.date: 01/10/2024
ms.prod: vcpkg
ms.topic: concept-article
---

# System package managers

C++ libraries are available in system package managers, such as apt, and vcpkg. How you choose to acquire your libraries depends on your specific usecase.

## System package managers vs vcpkg

Pros of using dependencies from system package manager:
- Overall deployment size will be reduced for customers as different programs can share the same share libraries.
- Your program will respond to policy decisions configured on that system. For example, DNS policy tends to be implemented in glibc, or trusted certificates implemented in OpenSSL, or UI / themeing bits from UI libraries, etc.
- It's usually faster to download binaries from a system package manager than it is to build them from source.

Pros of using dependencies through vcpkg:
- It will easier for development: vcpkg builds from source, meaning you will have debug information for libraries. Additionally, libraries build both release and debug versions.
- You have any control whatsoever over the versions of those dependencies. With a system package manager you're usually stuck with whatever versions it gives you which often forces folks to make different distributions of their code for each distro and version of distro. (In particular, this means you can get newer dependencies)
- The way you get dependencies might be the same for all platforms instead of needing carve outs for different systems.

Notably, vcpkg tries to not change how dependencies are bound to consuming projects, specifically to enable someone to get some dependencies from a system package manager if they want. 

## Scenario: Cross platform application with vcpkg and system dependencies

Suppose you have a C++ application that builds for Windows and Linux. On Windows, vcpkg contains all the libraries you need: libmad (an MPEG audio decoder) and `libpng` as well as a few others. On Linux, a similar but different set of libraries is required, including `libglvnd` (a GL vendor-neutral dispatch library) which is available on apt, but not in vcpkg.

In this case, we recommend having a single manifest for both Windows and Linux. `libglvnd` and other libraries specific to Linux are acquired through the system package manager. If you have a custom port that depends on `libglvnd`, you can use system package manager dependencies through vcpkg with an [empty port overlay](https://devblogs.microsoft.com/cppblog/using-system-package-manager-dependencies-with-vcpkg/).

To get the library added to the vcpkg catalog, open an issue in the [vcpkg repository](https://github.com/microsoft/vcpkg/issues).
