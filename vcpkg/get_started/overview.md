---
title: vcpkg overview
description: Learn about how the vcpkg C/C++ package manager can help you acquire and manage dependencies. 
ms.topic: overview
author: AugP
ms.author: aupopa
ms.date: 10/22/2023
---
# vcpkg overview

vcpkg (always spelled lowercase) is a free and open-source C/C++ package manager maintained by Microsoft and the C++ community. Initially launched in 2016 as a tool for assisting developers in migrating their projects to newer versions of Visual Studio, vcpkg has evolved into a cross-platform tool used by developers on Windows, macOS, and Linux with a large catalog of open-source libraries and enterprise-ready features. vcpkg is designed to not get in the way of your development process with support for a variety of build and project systems. As a C++ tool at heart, the vcpkg tool is written primarily in C++ and CMake. It is designed to address common pain points for C/C++ developers in ways generally ignored by other package managers.

## Why vcpkg?

- Over 2200 open-source libraries to choose from, or bring your own
- Consistent, cross-platform experience for Windows, macOS, and Linux
- Dependencies can be built from source as needed with over 70 default build configurations (or customize your own builds)
- Avoid ABI incompatibilities between packages and your consuming project
- Prevent version conflicts and diamond problems in your dependency graph
- Enterprise-ready tool relied by C/C++ developers across many industries, including: finance, gaming, embedded/IoT, semiconductors, defense, and at Microsoft
- For MSBuild and CMake users: automatic integration with your build environment for a seamless dependency acquisition experience

## vcpkg features

### Ports and triplets

A [port](../concepts/ports.md) in vcpkg is a versioned build recipe that produces a set of files (usually a C/C++ library, but not always).

A [triplet](../users/tripletsmd) captures the target build environment (cpu, os, compiler, runtime, etc.) in a single, convenient name. vcpkg provides over 70 triplets by default, but you can also define your own.

To install a package on your system, vcpkg runs the script specified in the port's *portfile.cmake* file. This can include downloading source code and running a build. To run the build, it will use the triplet information to make sure that the final package matches the desired configuration.

### Binary caching

While vcpkg builds packages from source whenever it's necessary, you can back up your built packages in a [binary cache](../consume/binary-caching-overview.md). This allows other developer machines or continuous integration runs to reference these prebuilt packages without running a new build every time. vcpkg intelligently determines if a rebuild is necessary by checking available package ABI hashes in the cache.  

### Manifests

You can declare your direct dependencies and add optional features or version constraints in a [manifest file](../users/manifests.md).

### Versioning

vcpkg has a unique way of handling [package versions](../users/versioning.concepts.md). Your manifest file can reference a single, baseline version set by default. The baseline contains a combination of packages at a specific version that have been tested for compatibility against each other. Furthermore, you can set a minimum version for a package or override a package version to an exact match.

### Registries

A [registry](../users/registries.md) is a catalog of ports and available versions that a vcpkg user can install. vcpkg provides a public registry of open-source libraries by default, but you can also define your own registries for custom libraries.

### Asset caching

[Asset caching](../users/assetcaching.md) allows vcpkg to use download-mirrors to upload and restore assets such as source code and build tools. This is useful for continuity of business, especially for open-source dependencies that have assets on third party servers. This also provides the developer protection in case upstream assets are tampered with unexpectedly.

## vcpkg compared to other package managers

### vcpkg compared to NuGet

NuGet is a .NET package manager that has often been used for C/C++ development, particularly for MSBuild solutions that also contain .NET projects. As a general rule, the Microsoft C++ team does not recommend using NuGet for C/C++ development, as NuGet has several limitations:

- **Compilation flavors**. As NuGet cannot build packages from source on the fly, it is necessary to provide prebuilt binaries to match all possible Application Binary Interface (ABI) restrictions for all users. The user is responsible for building packages themselves and ensuring this is done correctly. It is also difficult to distinguish between binaries due to the lack of relevant metadata, resulting in the user having to do quirky things like putting architecture, operating system, and compiler information in the package name, without being able to enforce such restrictions on package consumption.
- **Binary vs. source**. Very closely tied to the first point, NuGet is designed from the ground up to provide relatively small, prebuilt binaries. Due to the nature of native code, developers need to have access to the source code to ensure ABI compatibility, performance, integrity, and debuggability.
- **Versioning limitations**. While NuGet packages are versioned, it is up to the user to decide on appropriate versions of packages to take dependencies on. The pressure is on the user to solve version conflicts between dependencies and the consuming project.
- **No NuGet PackageReference support**. NuGet PackageReference is not supported for .vcxproj files and there is no plan to add it in the future due to technical and architectural differences between C++ and .NET MSBuild projects. This means NuGet C++ users won't benefit from features like the use of the global cache and referencing dependencies in simple MSBuild terms with access to conditional logic.
- **Environment changes**. As NuGet C++ packages must be manually compiled and packaged, any time the host or target environment changes this work must be repeated. On the other hand, vcpkg can build packages from source for you for a variety of operating systems and build systems.

### vcpkg compared to Conan

Conan.io is a publicly-federated, project-centric, cross-platform, C++ package manager written in python. Our primary differences are:

- **Public federation vs. private federation**. Conan relies on individuals publishing independent copies of each package. We believe this approach encourages a large number of packages that are all broken in different ways. We believe it is a waste of the user's time to pick through the list of 20+ public packages for Boost 1.56 to determine the handful that will work for their particular situation. In contrast, we believe there should be a single, collaboratively maintained version which works for the vast majority of cases and allow users to hack freely on their private versions. We believe this will result in a set of high quality packages that are heavily tested with each other and form a fantastic base for any private modifications you need.
- **Per-dll vs. per-application**. When dependencies are independently versioned on a library level, it encourages every build environment to be a completely unique, unable to take advantage of or contribute to a solid, well tested ecosystem. In contrast, by versioning all libraries together as a platform (similar to a system package manager), we hope to congregate testing and effort on very common sets of library versions to maximize the quality and stability of the ecosystem. This also completely designs out the ability for a library to ask for versions that conflict with the application's choices (I want openssl Z and boost X but X only claims to work with openssl Y).
- **Cross-platform vs. single-platform**. While being hosted on many platforms is an excellent north star, we believe the level of system integration and stability provided by apt-get, yum, and homebrew is well worth needing to exchange apt-get install libboost-all-dev with brew install boost in automated scripts. We chose to make our system as easy as possible to integrate into a world with these very successful system managers -- one more line for vcpkg install boost -- instead of attempting to replace them where they are already so successful and well-loved.
- **C++/CMake vs python**. While Python is an excellent language loved by many, we believe that transparency and familiarity are the most important factors when choosing a tool as important to your workflow as a package manager. Consequently, we chose to make the implementation languages be as universally accepted as possible: C++ should be used in a C++ package manager for C++ programmers. You should not be required to learn another programming language just to understand your package manager.

### vcpkg compared to system package managers

There are a wide variety of system package managers for Linux, macOS, and Windows that can be used to acquire and manage C/C++ libraries. These package managers are typically excellent choices for managing applications. However, due to the generic nature of their support, they often fail to deliver features beneficial particularly for C/C++ developers. While your mileage will vary, and some system package managers do deliver some of these features, none have them all:

- **Redistributable developer assets**. vcpkg can acquire redistributable developer assets to help with debugging.
- **Prebuilt packages vs. build from source**. vcpkg can build packages from source based on your custom requirements. There is no need to fight with prebuilt, pre-compiled packages to get them to work. 
- **Catalog-wide versioning**. vcpkg allows you to depend on a version set of compatible packages, rather than having to micromanage individual package versions. Of course you can still do so as needed, but the default experience is designed to be easy to get started with.
- **Catalog size**. As vcpkg is specialized to C/C++, it has a very large library catalog in comparison to system package managers that is actively maintained. In general, you are more likely to find useful and up to date libraries for this type of development.
- **Cross-platform support**. System package managers provide packages locked to that particular system. However, if you ever need to target more than one operating system flavor, you will need to find a different package manager for the second system. In contrast, vcpkg is a cross-platform package manager, so you simply need to adjust your target builds accordingly.

With that said, there are places where a system package manager absolutely makes sense. For example, system package managers tend to do a good job providing and maintaining libraries specific to that system. Furthermore, prebuilt packages should work out of the box on that system if your consuming project has a simple build configuration. Lastly, if you don't intend to do any cross-platform development, you won't run into operating system compatibility issues with a system package manager.

## Get started with vcpkg

Want to try out vcpkg? Check out one of our introductory tutorials:

- [Install and use packages with CMake](get-started.md)
- [Install and use packages with CMake in Visual Studio](get-started-vs.md)
- [Install and use packages with MSBuild in Visual Studio](get-started-msbuild.md)
- [Install and use packages with CMake in Visual Studio Code](get-started-vscode.md)

If your build system is not CMake or MSBuild, don't worry, vcpkg supports [manual integration](../users/buildsystems/manual-integration.md) as well with your preferred environment.
