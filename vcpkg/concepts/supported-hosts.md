---
title:  Supported hosts
description: This article describes the platforms on which vcpkg is supported.
author: bion
ms.author: bion
ms.date: 06/24/2024
ms.topic: concept-article
---

# Supported Hosts

This document describes the architectures and operating systems supported by vcpkg.

## Dependencies

vcpkg also depends on a few system components to be installed before it can run.

To use vcpkg, a few components need to be installed on the system first.

* Windows: Visual Studio 2015 or later.
* macOS: Xcode Command Line Tools, tar, git, zip, unzip, and curl. Most ports also need `pkg-config`.
We recommend installing these dependencies via Homebrew, such as via a terminal command
`brew install zip unzip curl pkgconfig`.
* Linux: A C++ compiler to use, tar, git, zip, unzip, and curl. Most ports also need `pkg-config`.
We recommend installing these dependencies with your system package manager.
  * apt-based platforms: `apt install git curl zip unzip pkgconfig`
  * rpm-based platforms: `dnf install git curl zip unzip pkgconfig`
  * pacman-based platforms: `base-devel git curl zip unzip tar cmake ninja`

For Linux and macOS users we also recommend to install the following packages: `autoconf`, `autoconf-archive`, `automake`, `make` and `libtool`. While not required, these packages are used often by libraries available in vcpkg.

## Curated registry support levels

Support is broken into 4 distinct categories. They are:

* **Full support, tested**  
  The configurations used to test vcpkg's curated registry on a regular basis. Problems in these configurations
  are rapidly found and fixed by vcpkg's maintainers and community..
* **Full support, expected**  
  Configurations treated with the same priority as 'Full support, tested' but which are not part of the regular tests.
  In general, these configurations are substantially similar to one of the fully tested configurations.
* **Community support**  
  Configurations that are not actively supported by vcpkg's maintainers but have substantial community investment.
  vcpkg maintainers accept changes to fix problems for these platforms but don't make guarantees that these 
  configurations work. The vcpkg team is interested in moving as many platforms out of community support
  into full support as possible.
* **Unsupported**  
  Configurations that are known no to work for some reason. In general, PRs fixing only these platforms
  are not accepted.

## Full support, tested

The fully supported, tested platforms are:

### Windows

* The latest Windows / Windows Server release. These are Windows 11 and Windows Server 2022 as of this writing.
* The latest Visual Studio update, Visual Studio 2022 version 17.8 as of this writing.

### macOS

macOS is intended to track the latest version of macOS and contemporary version of Xcode Command Line tools. However,
updating macOS machines is a manual process, and macOS frequently changes things in ways that break vcpkg's testing. As of this writing, we are using:

* macOS 14.5
* XCode Command Line Tools 14.4

### Linux

* The latest LTS release of Ubuntu, currently 22.04.

### Android

* Linux 64-bit Android NDK version r26d

## Full support, expected

### Windows

* Windows 8.1 / Windows Server 2016 and later
* Visual Studio 2015 and later

### macOS

* The latest version of macOS, minus 2 major versions. For example, the current version of macOS is macOS 14 Sonoma,
so we expect vcpkg to work as far back as macOS 12 Monterey. This is intended to track with Apple's own support for
macOS.
* Contemporary versions of the Xcode Command Line Tools for a given release of macOS

### Linux

We intend to support AMD64 builds of glibc-based Linuxes still in support from their distribution vendor released within
the last 5 years. Examples:

* Ubuntu 22.04 and 20.04 are both in support from Canonical and released within the last 5 years, so they
are expected to work. Ubuntu 18.04 is still in support from Canonical but was released more than 5 years ago, so
we no longer consider it fully supported. 20.10 is newer than 20.04 but we do not consider it fully supported because
it is no longer in support from Canonical.
* Red Hat Enterprise Linux 9 is fully supported.
* CentOS and Red Hat Enterprise Linux 8 left support from Red Hat on May 31, 2024, and was released more than 5 years
ago, and is thus no longer supported.
* CentOS and RHEL 7 are out of support from Red Hat, and are not expected to work as they were released more than 5
years ago. The Oracle Linux fork now supported by Oracle is still in support from Oracle, but still not expected to
work as it was released more than 5 years ago.
* Fedora 40 and 39 are fully supported, but 38 is no longer supported by Fedora.
* Debian 12 "Bookworm", 11 "Bullseye", and 10 "Buster" are all supported by Debian and released in the last 5 years.
Debian 9 "Stretch" left support from the Debian project in July 2022 and is thus not supported by vcpkg.

We also assume that users' build systems will match the version of Linux they are using, and take care to ensure
components like our manifest mode CMake integration will work with the versions of these dependencies that come with
one of the above distros. As of this writing, those dependency versions and the associated distro(s) are:

* GCC 8.3.0 (Debian 10)
* CMake 3.13.4 (Debian 10)
* Ninja 1.8.2 (Debian 10)
* Curl 7.64 (Debian 10)
* zip 3.0 (All)
* unzip 6.0 (All)

## Community support

### Windows

* Windows 7 / Windows Server 2008 R2 and later

`vcpkg.exe` should work on Windows 7 flavors, but the vast majority of the curated registry requires MSYS2 components
like `pkg-config` which
[no longer support versions of Windows older than 8.1](https://www.msys2.org/docs/windows_support/#current-policy).

* MinGW or Clang as the build compiler.

Currently vcpkg has a number of Visual Studio assumptions; in particular our features that copy matching PDBs for
executables currently require dumpbin, a Visual Studio component. We have a number of dedicated community members who
care about MinGW support though, and do include MinGW toolchains in the box.

### Linux

* Non-AMD64 Linux. We are extremely interested in help from folks targeting arm64 Linux if it can be shown how
to deliver binaries that have a high likelihood of success running on all arm64 linux hardware.
* Non-glibc Linux. Several tools vcpkg normally provides for you are distributed in binary forms that expect the system
libc to be glibc. We do provide a muslc-based binary of vcpkg we have tested to work on Alpine, but are unable to
furnish muslc based copies of a number of dependencies like CMake.

### BSDs or other POSIX systems

On BSD targets, no vcpkg binary is provided for download. Instead, the bootstrap logic will download the
vcpkg source code and compile it locally.

#### Dependencies

vcpkg requires a number of software packages that are not part of the operating system and have to be installed using the package manager:

* FreeBSD: `pkg install -y bash cmake curl git ninja zip unzip`
* OpenBSD: `pkg_add -Iz bash cmake coreutils curl git ninja zip unzip-6.10-iconv`

As on Linux, it is recommended to also install some additional packages:

* FreeBSD: `pkg install -y autoconf autoconf-archive gmake pkgconf python`
* OpenBSD: `pkg_add -Iz autoconf-2.71 autoconf-archive gmake pkgconf python3`

#### Caveats

* vcpkg executes `unzip` from `PATH`. Eventually, this might cause vcpkg to execute the
operating system provided `unzip` despite an additional one having been installed using the package manager.
The `PATH` environment variable has to be configured to prefer the third party `unzip` over the operating system's one,
or vcpkg will fail to extract its binary cache.

## Unsupported

* 32-bit Windows. Targeting x86-windows or arm-windows works just fine, but vcpkg itself expects to run AMD64 copies
of support tooling like CMake.
* Visual Studio 2013 or earlier.
* Windows Vista or earlier.
* macOS or Linux hosts older than those in the *Fully supported* categories.
