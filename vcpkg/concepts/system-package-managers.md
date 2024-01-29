---
title: Using vcpkg compared to system package managers
description: Concepts on system package managers
author: data-queue
ms.author: danshaw2
ms.date: 01/10/2024
ms.topic: concept-article
---

# System package managers

C++ libraries are available in system package managers such as apt and vcpkg. How you choose to acquire your libraries depends on your specific use case. You can also have vcpkg dependencies [depend on system package manager dependencies](https://devblogs.microsoft.com/cppblog/using-system-package-manager-dependencies-with-vcpkg/).

## Reasons to use dependencies from a system package manager
- Overall deployment size will be reduced, as different programs can share the same shared libraries.
- Your program will respond to policy decisions configured on that system. For example, DNS policy tends to be implemented in glibc, or trusted certificates implemented in OpenSSL, or UI / themeing bits from UI libraries, etc.
- It's usually faster to download binaries from a system package manager than it is to build them from source.

## Reasons to use dependencies from vcpkg
- It will easier for development: vcpkg builds from source, meaning you will have debug information for libraries. Additionally, libraries build both release and debug versions.
- You have complete control over the versions of those dependencies. With a system package manager, you may be limited to only certain versions.
- The way you get dependencies can be the same for all platforms that you target.
