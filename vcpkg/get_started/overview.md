---
title: vcpkg overview
description: Learn about how the vcpkg C/C++ package manager can help you acquire and manage dependencies. 
ms.topic: overview
author: AugP
ms.author: aupopa
ms.date: 07/11/2024
---

# vcpkg overview

vcpkg is a free and open-source C/C++ package manager maintained by Microsoft and the C++ community. vcpkg runs on Windows, macOS, and Linux (see [Supported hosts](../concepts/supported-hosts.md)) and includes a large, curated registry of open-source libraries and enterprise-ready features designed to facilitate your development process with support for [any build and project system](../concepts/build-system-integration.md). With vcpkg, you can build libraries from source to target virtually any platform or architecture using the customizable [triplets experience](../concepts/triplets.md). vcpkg is a C++ tool at heart and is written in C++ and CMake scripts. It is designed from the ground up to address the unique pain points of the C/C++ developer experience.

## Why vcpkg?

- Over 2300 open-source libraries to choose from, or bring your own
- Consistent, cross-platform experience for Windows, macOS, and Linux
- Build dependencies from source as needed, with over 70 configurations in the box, and infinite customization for your specific requirements
- Avoid ABI incompatibilities between packages and your project
- Prevent version conflicts and diamond problems in your dependency graph
- It's enterprise-ready. C/C++ developers across many industries rely on it, including: finance, gaming, embedded/IoT, semiconductors, defense, and at Microsoft
- For MSBuild and CMake users: automatic integration with your build environment for a seamless dependency acquisition experience

## vcpkg features

### Ports and triplets

A vcpkg [port](../concepts/ports.md) is a versioned build recipe that produces a package. The most common type of package is a C/C++ library consisting of headers, source code, and binaries.

A [triplet](../users/triplets.md) captures the target build environment (cpu, os, compiler, runtime, etc.) in a single, convenient name. vcpkg provides over 70 triplets by default, but you can also define your own.

To install a package on your system, vcpkg runs the port's recipe file, which is a CMake script. The script might contain steps to download source code or to run a build on your system. During builds, vcpkg uses the information in your triplet to ensure that the produced package matches your desired configuration.

### Binary caching

While vcpkg builds libraries from source whenever it's necessary, you can back up your built packages in a [binary cache](../consume/binary-caching-overview.md). This allows other developer machines or continuous integration runs to reference these prebuilt packages without running a new build every time. vcpkg determines if a rebuild is necessary by checking if the cache already contains a valid existing package with appropriate binaries.

### Manifests

You can declare your direct dependencies and add optional features or version constraints in a [manifest file](../concepts/manifest-mode.md). Manifest files can be checked into your source control system and shared with your team.

### Versioning

vcpkg has a unique way of handling [package versions](../users/versioning.concepts.md). Your manifest file can reference a single, baseline version set by default. The baseline gives you hassle-free, conflict-free dependency management with full reproducibility. Furthermore, you can still have more advanced control by pinning individual package versions.

### Registries

A [registry](../concepts/registries.md) is a collection of ports and available versions that a vcpkg user can install. vcpkg provides a curated registry of open-source libraries by default. You can also create your own registries for customizations, patches, or private libraries.

### Asset caching

[Asset caching](../users/assetcaching.md) allows vcpkg to work in air-gapped and offline environments, ensuring continuity of business even if a remote host goes down or is compromised. It uses download-mirrors to upload and restore assets like source code and build tools.

## vcpkg compared to other package managers

### vcpkg compared to NuGet

NuGet is a .NET package manager that is often used for C/C++ development, particularly for MSBuild solutions that contain .NET projects. As a general rule, the Microsoft C++ team does not recommend using NuGet for C/C++ development because NuGet has several limitations:

- **Compilation flavors**. Because NuGet cannot build packages from source on the fly, it is necessary to provide prebuilt binaries to match all possible Application Binary Interface (ABI) restrictions for all users. The user is responsible for correctly building packages. It's also difficult to distinguish between binaries due to the lack of relevant metadata. This results in the user putting architecture, operating system, and compiler information in the package name. This is undesirable because constraints on architecture, OS, and compiler information can't be enforced during package acquisition.
- **Binary vs. source**. NuGet is designed from the ground up to provide relatively small, prebuilt binaries. Developers need to have access to the source code to ensure ABI compatibility, performance, integrity, and debuggability.
- **No NuGet PackageReference support**. NuGet PackageReference is not supported for `.vcxproj` files and there isn't a plan to add it in the future due to technical and architectural differences between C++ and .NET MSBuild projects. This means NuGet C++ users won't benefit from features like the use of the global cache and referencing dependencies in simple MSBuild terms with access to conditional logic.
- **Transitive ABI tracking**. vcpkg rebuilds open-source dependencies impacted by a change in a particular package. For example, if a new update is released for Boost, vcpkg rebuilds all of the dependencies of Boost, and libraries that depend on Boost, to ensure they still work. Pull requests to update libraries in the vcpkg repo are not merged until build-time conflicts are addressed.

### vcpkg compared to system package managers

There are a wide variety of system package managers for Linux, macOS, and Windows that can be used to acquire and manage C/C++ libraries. These package managers are typically excellent choices for managing applications. But due to the generic nature of their support, they often fail to deliver features beneficial particularly for C/C++ developers. While your mileage will vary, and some system package managers do deliver some of these features, none have all of the following:

- **Redistributable developer assets**: vcpkg can acquire redistributable developer assets to help with debugging.
- **Prebuilt packages vs. build from source**: vcpkg can build packages from source based on your custom requirements. There is no need to deal with with prebuilt, pre-compiled packages to get them to work.
- **Catalog-wide versioning**: vcpkg allows you to depend on a version set of compatible packages, rather than having to micromanage individual package versions. You can still do so as needed, but the default experience is designed to be easy to get started with.
- **Multiple copies of the same library on one system**: You can have multiple copies of the same dependency installed on the same system with vcpkg, whereas system package managers may install one version to a single, system-wide location. This complicates things when you have multiple projects depending on different versions of a library.
- **Catalog size**: Because vcpkg is specialized for C/C++, it has a very large C/C++ library collection in comparison to system package managers, and it is actively maintained. In general, you are more likely to find useful and up to date libraries for C++ development.
- **Cross-platform support**: System package managers provide packages locked to a particular system. If you need to target more than one operating system flavor, you'll need to find a different package manager for the second system. In contrast, vcpkg is a cross-platform package manager. So, you simply need to adjust your target builds accordingly.

There are situations where a system package manager absolutely makes sense:

- System package managers tend to do a good job providing and maintaining libraries specific to that system
- Prebuilt packages should work out of the box on that system if your consuming project has a simple build configuration.
- If you don't intend to do any cross-platform development, you won't run into operating system compatibility issues with a system package manager.

vcpkg is designed to work side-by-side with system package managers, so feel free to use the tool that makes the most sense to you for each dependency.

## Get started with vcpkg

To try out vcpkg, check out our introductory tutorials:

- [Install and use packages with CMake](get-started.md)
- [Install and use packages with CMake in Visual Studio](get-started-vs.md)
- [Install and use packages with MSBuild in Visual Studio](get-started-msbuild.md)
- [Install and use packages with CMake in Visual Studio Code](get-started-vscode.md)
- [Package a library with vcpkg](get-started-packaging.md)
- [Add a port to the vcpkg curated registry](get-started-adding-to-registry.md)

If your build system is not CMake or MSBuild, vcpkg also supports [manual integration](../users/buildsystems/manual-integration.md) with your preferred environment.
