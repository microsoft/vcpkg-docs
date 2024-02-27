---
title: Frequently Asked Questions
description: Common questions and answers about vcpkg
ms.date: 02/09/2024
ms.topic: faq
---
# Frequently Asked Questions

## What are classic mode and manifest mode?

There are two ways to manage your dependencies with vcpkg:

1. **Manifest mode** lets you declare your direct dependencies, version constraints, and used registries in a file called [`vcpkg.json`](../reference/vcpkg-json.md). This file should be included in your code repository and can be checked in to your source control system. Dependencies are installed in a subfolder named `vcpkg_installed`. This way, each code project can have its own set of dependencies; nothing is installed system-wide. You can run vcpkg in manifest mode by running `vcpkg install` (with no other arguments), or by taking advantage of the automatic [integration with MSBuild](../users/buildsystems/msbuild-integration.md) and [CMake projects](../users/buildsystems/cmake-integration.md). We recommend using manifests for your projects over classic mode in most cases, as you have better control over your dependencies. See our [Manifest mode article](../concepts/manifest-mode.md) for more details.
2. **Classic mode** is the more traditional way of managing dependencies, which involves running vcpkg commands that specify each direct dependency to install, modify, or remove. Dependencies are stored within the vcpkg installation directory, so multiple consuming projects can reference the same set of dependencies. See our [Classic mode article](../users/classic-mode.md) for more details.

## Can I contribute a new library?

Yes! Start by reading our [contribution guidelines](https://github.com/Microsoft/vcpkg/blob/master/CONTRIBUTING.md). Also take a look at our [Maintainer Guide](../contributing/maintainer-guide.md) which goes into more details. We also have a [tutorial for adding a port to vcpkg](../get_started/get-started-adding-to-registry.md) to help you get started.

If you want to contribute but don't have a particular library in mind then take a look at the list
of [new port requests](https://github.com/Microsoft/vcpkg/issues?q=is%3Aissue+is%3Aopen+label%3Acategory%3Anew-port).

## Can vcpkg create pre-built binary packages? What is the binary format used by vcpkg?

Yes! See the [`export` command](../commands/export.md) if you wish to produce binaries for exporting into other environments.

Alternatively, if your goal is to preserve binaries produced by [`vcpkg install`](../commands/install.md) operations for later re-use, see the [binary caching feature](../users/binarycaching.md)

## How do I update libraries?

If you are using a manifest (vcpkg.json file) to manage your dependencies, you need to update that file. See the [versioning reference documentation](../users/versioning.md) for details.

If you are using vcpkg in classic mode (running commands to manage packages, no manifest file), see the [`vcpkg update` command documentation](../commands/update.md). This command lists all packages which are out-of-sync with your current portfiles. You will then need to run the [`vcpkg upgrade` command](../commands/upgrade.md) to confirm the changes.

## How do I get more libraries?

The list of libraries is enumerated from the [`ports\`](https://github.com/Microsoft/vcpkg/blob/master/ports) directory. By design, you can add and remove libraries from this directory as you see fit for yourself or your company -- see our examples on packaging [zipfiles](../examples/packaging-zipfiles.md) and [GitHub repos](../examples/packaging-github-repos.md).

We recommend cloning directly from [GitHub](https://github.com/microsoft/vcpkg) and using `git pull` to update the list of portfiles.

## Can I build a private library with this tool?

Yes. Follow [our packaging example](../get_started/get-started-packaging.md) to create your own port and see the [overlay ports](../concepts/overlay-ports.md) and [registries](../concepts/registries.md) documentation to learn how to manage your private ports.

You can take this further by publishing your private libraries into a registry. See the article on [Creating registries](../maintainers/registries.md). A registry is a catalog of ports, similar to the one provided with vcpkg that contains open source libraries.

## Can I use a prebuilt private library with this tool?

Yes. The `portfile.cmake` for a library is fundamentally a script that places the headers and binaries into the correct arrangement in the `${CURRENT_PACKAGES_DIR}`, so to pull in prebuilt binaries you can write a portfile which directly downloads and arranges the files.

To see an example of this, look at [`ports\opengl\portfile.cmake`](https://github.com/microsoft/vcpkg/blob/master/ports/opengl/portfile.cmake) which simply copies files out of the Windows SDK.

## Which platforms can I target with vcpkg?

Our built-in, continuous integration tested triplets are:

- Windows Desktop (x86, x64, x64-static, arm64)
- Universal Windows Platform (x64, and arm64)
- Mac OS X (x64-static)
- Linux (x64-static)
- Android (x64, arm64, arm-neon)

These targets are tested more rigorously for compatibility with each vcpkg release. However, we have a much larger number of community triplets available with more platforms and architectures, including for iOS, MinGW, WebAssembly, freeBSD, and openBSD.

You can also define your own triplets depending on your needs. vcpkg is very customizable.

See `vcpkg help triplet` for the current list and review our [triplets documentation](../concepts/triplets.md) for more details.

## Does vcpkg run on Linux/OS X?

Yes! We continuously test on OS X and Ubuntu 22.04, however we know users have been successful with Arch, Fedora, and FreeBSD. If you have trouble with your favorite Linux distribution, let us know in an issue and we'd be happy to help!

## How do I update vcpkg?

Execute `git pull` to get the latest sources, then run `bootstrap-vcpkg.bat` (Windows) or `./bootstrap-vcpkg.sh` (Unix) to update vcpkg. Or, if you are using a copy of vcpkg that ships with Visual Studio, simply update your Visual Studio version from the Visual Studio Installer.

## How do I use different versions of a library on one machine?

We suggest using [manifest files](../users/manifests.md) to manage dependencies for individual projects, which works even if multiple projects are on the same machine and allow you to easily manage package versions and which registries libraries are coming from.

However, if you are using classic mode instead, within a single instance of vcpkg (e.g. one set of `installed\`, `packages\`, `ports\` and so forth), you can only have one version of a library installed (otherwise, the headers would conflict with each other!). For those with experience with system-wide package managers, packages in vcpkg correspond to the `X-dev` or `X-devel` packages. In this case, to use different versions of a library for different projects, we recommend making separate instances of vcpkg and using the per-project integration mechanisms. The versions of each library are specified by the files in `ports\`, so they are easily manipulated using standard `git` commands. This makes it very easy to roll back the entire set of libraries to a consistent set of older versions which all work with each other. If you need to then pin a specific library forward, that is as easy as checking out the appropriate version of `ports\<package>\`. If your application is very sensitive to the versions of libraries, we recommend checking in the specific set of portfiles you need into your source control along with your project sources and using the `--vcpkg-root` option to redirect the working directory of `vcpkg.exe`.

## How does vcpkg protect my privacy?

See the [Privacy document](privacy.md) for all information regarding privacy.

## Can I use my own CMake toolchain file with vcpkg's toolchain file?

Yes. If you already have a CMake toolchain file, you will need to include our toolchain file at the end of yours. This should be as simple as an `include(<vcpkg_root>\scripts\buildsystems\vcpkg.cmake)` directive. Alternatively, you could copy the contents of our `scripts\buildsystems\vcpkg.cmake` into the end of your existing toolchain file.

## Can I use my own/specific flags for rebuilding libraries?

Yes. In the current version, there is not yet a standardized global way to change them, however you can edit individual portfiles and tweak the exact build process however you'd like.

By saving the changes to the portfile (and checking them in), you'll get the same results even if you're rebuilding from scratch in the future and forgot what exact settings you used.

## Can I get vcpkg integration for custom configurations?

Yes. While vcpkg will only produce the standard "Release" and "Debug" configurations when building a library, you can get integration support for your projects' custom configurations, in addition to your project's standard configurations.

First of all, vcpkg will automatically assume any custom configuration starting with "Release" (resp. "Debug") as a configuration that is compatible with the standard "Release" (resp. "Debug") configuration and will act accordingly.

For other configurations, you only need to override the MSBuild `$(VcpkgConfiguration)` macro in your project file (.vcxproj) to declare the compatibility between your configuration, and the target standard configuration. Unfortunately, due to the sequential nature of MSBuild, you'll need to add those settings much higher in your vcxproj so that it is declared before the vcpkg integration is loaded. It is recommend that the `$(VcpkgConfiguration)` macro is added to the "Globals" PropertyGroup.

For example, you can add support for your "MyRelease" configuration by adding in your project file:

```xml
<PropertyGroup Label="Globals">
  ...
  <VcpkgConfiguration Condition="'$(Configuration)' == 'MyRelease'">Release</VcpkgConfiguration>
</PropertyGroup>
```

Of course, this will only produce viable binaries if your custom configuration is compatible with the target configuration (e.g. they should both link with the same runtime library).

## I can't use user-wide integration. Can I use a per-project integration?

Yes. A NuGet package suitable for per-project use can be generated via either the `vcpkg integrate project` command (lightweight linking) or the `vcpkg export --nuget` command (shrinkwrapped).

A lower level mechanism to achieve the same as the `vcpkg integrate project` NuGet package is via the `<vcpkg_root>\scripts\buildsystems\msbuild\vcpkg.targets` file. All you need is to import it in your .vcxproj file, replacing `<vcpkg_root>` with the path where you installed vcpkg:

```xml
<Import Project="<vcpkg_root>\scripts\buildsystems\msbuild\vcpkg.targets" />
```

## How can I remove temporary files?

If you only care about the installed packages, it is safe to delete the
following directories inside the vcpkg root folder:

- `packages`,
- `buildtrees`,
- and `downloads`

You can use the [`--clean-after-build`
flag](../commands/install.md#clean-after-build) in your [`vcpkg install`
command](../commands/install.md) to make vcpkg delete the temporary files
automatically after the build is completed.

vcpkg also uses other temporary locations external to the vcpkg root folder. The
Visual Studio integration files, default binary cache, and registries cache; are
all located in the following path depending on your operative system:

On Windows:

- `%LocalAppData%/vcpkg`

On Linux/macOS:

- `$XDG_CACHE_HOME/vcpkg`
- `~/.cache/vcpkg` (only if `XDG_CACHE_HOME` is not defined)

If you have configured local binary or asset caches then you may want to
periodically clean those too as needed.

## How is CMake used internally by vcpkg?

vcpkg uses CMake internally as a build scripting language. This is because CMake is already an extremely common build system for cross-platform open source libraries and is becoming very popular for C++ projects in general. It is easy to acquire on Windows, does not require system-wide installation, and legible for unfamiliar users.

## Does vcpkg support downloading compiled binaries from a public or private server?

We recommend building your libraries once with your preferred build configurations and using the [binary caching](../users/binarycaching.md) feature to re-use binaries without having to re-build every time. This is useful when working on a team project or when you are building both locally and in a continuous integration environment across multiple machines, containers, virtual machines, etc.

## What MSVC toolsets are supported?

We support Visual Studio 2015 Update 3 and above.

## Why does Visual Studio not use my libraries with user-wide integration enabled?

Enabling user-wide integration (`vcpkg integrate install`) changes the default for some project properties. In particular, "C/C++/General/Additional Include Directories" and "Linker/General/Additional Library Directories" are normally blank *without* user-wide integration. *With* integration, a blank value means that the augmented default supplied by vcpkg is overridden, and headers/libraries will not be found. To reinstate the default, set the properties to inherit from parent.

## Why not NuGet?

NuGet is a package manager for .NET libraries with a strong dependency on MSBuild. It does not meet the specific needs of Native C++ customers in at least three ways.

- **Compilation Flavors**. With so many possible combinations of compilation options, the task of providing a truly complete set of options is intrinsically impossible. Furthermore, the download size for reasonably complete binary packages becomes enormous. This makes it a requirement to split the results into multiple packages, but then searching becomes very difficult.

- **Binary vs Source**. Very closely tied to the first point, NuGet is designed from the ground up to provide relatively small, prebuilt binaries. Due to the nature of native code, developers need to have access to the source code to ensure ABI compatibility, performance, integrity, and debuggability.

- **Per-dll vs Per-application**. NuGet is highly project centric. This works well in managed languages with naturally stable ABIs, because base libraries can continue to evolve without breaking those higher up. However, in native languages where the ABI is much more fragile, the only robust strategy is to explicitly build each library against the exact dependencies that will be included in the final application. This is difficult to ensure in NuGet and leads to a highly disconnected and independently versioned ecosystem.

## Why not Conan?

Conan.io is a publicly-federated, project-centric, cross-platform, C++ package manager written in python. Our primary differences are:

- **Public federation vs private federation**. Conan relies on individuals publishing independent copies of each package. We believe this approach encourages a large number of packages that are all broken in different ways. We believe it is a waste of user's time to pick through the list of 20+ public packages for Boost 1.56 to determine the handful that will work for their particular situation. In contrast, we believe there should be a single, collaboratively maintained version which works for the vast majority of cases and allow users to hack freely on their private versions. We believe this will result in a set of high quality packages that are heavily tested with each other and form a fantastic base for any private modifications you need.

- **Per-dll vs Per-application**. When dependencies are independently versioned on a library level, it encourages every build environment to be a completely unique, unable to take advantage of or contribute to a solid, well tested ecosystem. In contrast, by versioning all libraries together as a platform (similar to a system package manager), we hope to congregate testing and effort on very common sets of library versions to maximize the quality and stability of the ecosystem. This also completely designs out the ability for a library to ask for versions that conflict with the application's choices (I want openssl Z and boost X but X only claims to work with openssl Y).

- **Cross-platform vs single-platform**. While being hosted on many platforms is an excellent north star, we believe the level of system integration and stability provided by apt-get, yum, and homebrew is well worth needing to exchange `apt-get install libboost-all-dev` with `brew install boost` in automated scripts. We chose to make our system as easy as possible to integrate into a world with these very successful system managers -- one more line for `vcpkg install boost` -- instead of attempting to replace them where they are already so successful and well-loved.

- **C++/CMake vs python**. While Python is an excellent language loved by many, we believe that transparency and familiarity are the most important factors when choosing a tool as important to your workflow as a package manager. Consequently, we chose to make the implementation languages be as universally accepted as possible: C++ should be used in a C++ package manager for C++ programmers. You should not be required to learn another programming language just to understand your package manager.

## Why not Chocolatey?

Chocolatey is an excellent system for managing applications. However, it is not currently designed to acquire redistributable developer assets and help you with debugging. vcpkg, in comparison, is designed to get you the libraries you need to build your application and help you deliver through any platform you'd like (including Chocolatey!).
