---
title: vcpkg overview
description: Learn about how the vcpkg C/C++ package manager can help you acquire and manage dependencies. 
ms.topic: overview
author: AugP
ms.author: aupopa
ms.date: 10/22/2023
---

# vcpkg overview

vcpkg is a free and open-source C/C++ package manager maintained by Microsoft and the C++ community. Initially launched in 2016 as a tool for assisting developers in migrating their projects to newer versions of Visual Studio, vcpkg has evolved into a cross-platform tool used by developers on Windows, macOS, and Linux. vcpkg has a large catalog of open-source libraries and enterprise-ready features designed to facilitate your development process with support for any build and project systems. vcpkg is a C++ tool at heart and is written in C++ with scripts in CMake. It is designed from the ground up to address the unique pain points C/C++ developers experience.

## Why vcpkg?

- Over 2200 open-source libraries to choose from, or bring your own
- Consistent, cross-platform experience for Windows, macOS, and Linux
- Dependencies can be built from source as needed with over 70 configurations in the box and infinite customization for your specific requirements
- Avoid ABI incompatibilities between packages and your consuming project
- Prevent version conflicts and diamond problems in your dependency graph
- Enterprise-ready tool relied by C/C++ developers across many industries, including: finance, gaming, embedded/IoT, semiconductors, defense, and at Microsoft
- For MSBuild and CMake users: automatic integration with your build environment for a seamless dependency acquisition experience

## vcpkg features

### Ports and triplets

A [port](../concepts/ports.md) in vcpkg is a versioned build recipe that produces a package. The most common type of package is a C/C++ library consisting of headers, source code, and binaries.

A [triplet](../users/triplets.md) captures the target build environment (cpu, os, compiler, runtime, etc.) in a single, convenient name. vcpkg provides over 70 triplets by default, but you can also define your own.

To install a package on your system, vcpkg runs the port's recipe file (a CMake script). The script may contain steps to download source code or run a build in your system. During builds, vcpkg uses the information in your triplet to make sure that the produced package matches your desired configuration.

### Binary caching

While vcpkg builds libraries from source whenever it's necessary, you can back up your built packages in a [binary cache](../consume/binary-caching-overview.md). This allows other developer machines or continuous integration runs to reference these prebuilt packages without running a new build every time. vcpkg intelligently determines if a rebuild is necessary by checking if the cache already contains a valid existing package when it attempts to install a package.

### Manifests

You can declare your direct dependencies and add optional features or version constraints in a [manifest file](../users/manifests.md).

### Versioning

vcpkg has a unique way of handling [package versions](../users/versioning.concepts.md). Your manifest file can reference a single, baseline version set by default. The baseline gives you hassle-free, conflict-free dependency management with full reproducibility. Furthermore, you can still have more advanced control by pinning individual package versions.

### Registries

A [registry](../concepts/registries.md) is a catalog of ports and available versions that a vcpkg user can install. vcpkg provides a public registry of open-source libraries by default and you can also create your own registries for customizations, patches, or private libraries.

### Asset caching

[Asset caching](../users/assetcaching.md) allows vcpkg to work in air-gapped and offline environments, ensuring continuity of business even if a remote host goes down or is compromised. It uses download-mirrors to upload and restore assets such as source code and build tools.

## vcpkg compared to other package managers

### vcpkg compared to NuGet

NuGet is a .NET package manager that has often been used for C/C++ development, particularly for MSBuild solutions that also contain .NET projects. As a general rule, the Microsoft C++ team does not recommend using NuGet for C/C++ development, as NuGet has several limitations:

- **Compilation flavors**. As NuGet cannot build packages from source on the fly, it is necessary to provide prebuilt binaries to match all possible Application Binary Interface (ABI) restrictions for all users. The user is responsible for building packages themselves and ensuring this is done correctly. It is also difficult to distinguish between binaries due to the lack of relevant metadata, resulting in the user having to do quirky things like putting architecture, operating system, and compiler information in the package name, without being able to enforce such constraints during package acquisition.
- **Binary vs. source**. Very closely tied to the first point, NuGet is designed from the ground up to provide relatively small, prebuilt binaries. Due to the nature of native code, developers need to have access to the source code to ensure ABI compatibility, performance, integrity, and debuggability.
- **No NuGet PackageReference support**. NuGet PackageReference is not supported for .vcxproj files and there is no plan to add it in the future due to technical and architectural differences between C++ and .NET MSBuild projects. This means NuGet C++ users won't benefit from features like the use of the global cache and referencing dependencies in simple MSBuild terms with access to conditional logic.
- **Transitive ABI tracking**. vcpkg rebuilds open-source dependencies impacted by a change in a particular package. For example, if a new update is released for Boost, vcpkg will rebuild all dependencies of Boost and libraries that depend on Boost to ensure they still work. PRs to update libraries in the vcpkg repo are not merged until such build-time conflicts are addressed.

### vcpkg compared to system package managers

There are a wide variety of system package managers for Linux, macOS, and Windows that can be used to acquire and manage C/C++ libraries. These package managers are typically excellent choices for managing applications. However, due to the generic nature of their support, they often fail to deliver features beneficial particularly for C/C++ developers. While your mileage will vary, and some system package managers do deliver some of these features, none have them all:

- **Redistributable developer assets**. vcpkg can acquire redistributable developer assets to help with debugging.
- **Prebuilt packages vs. build from source**. vcpkg can build packages from source based on your custom requirements. There is no need to fight with prebuilt, pre-compiled packages to get them to work.
- **Catalog-wide versioning**. vcpkg allows you to depend on a version set of compatible packages, rather than having to micromanage individual package versions. Of course you can still do so as needed, but the default experience is designed to be easy to get started with.
- **Multiple copies of the same library on one system**. You can have multiple copies of the same dependency installed on the same system with vcpkg, whereas system package managers may install one version to a single, system-wide location. This complicates things when you have multiple projects depending on different versions of a library.
- **Catalog size**. As vcpkg is specialized to C/C++, it has a very large C/C++ library catalog in comparison to system package managers and it is actively maintained. In general, you are more likely to find useful and up to date libraries for this type of development.
- **Cross-platform support**. System package managers provide packages locked to that particular system. However, if you ever need to target more than one operating system flavor, you will need to find a different package manager for the second system. In contrast, vcpkg is a cross-platform package manager, so you simply need to adjust your target builds accordingly.

With that said, there are places where a system package manager absolutely makes sense. For example, system package managers tend to do a good job providing and maintaining libraries specific to that system. Furthermore, prebuilt packages should work out of the box on that system if your consuming project has a simple build configuration. Lastly, if you don't intend to do any cross-platform development, you won't run into operating system compatibility issues with a system package manager. vcpkg is designed to work side-by-side with system package managers, so feel free to use the tool that makes the most sense to you for each dependency.

## Get started with vcpkg

Want to try out vcpkg? Check out one of our introductory tutorials:

- [Install and use packages with CMake](get-started.md)
- [Install and use packages with CMake in Visual Studio](get-started-vs.md)
- [Install and use packages with MSBuild in Visual Studio](get-started-msbuild.md)

If your build system is not CMake or MSBuild, don't worry, vcpkg supports [manual integration](../users/buildsystems/manual-integration.md) as well with your preferred environment.
