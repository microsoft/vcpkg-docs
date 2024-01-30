---
title:  Supported hosts
description: This article describes the platforms on which vcpkg is supported.
author: bion
ms.author: bion
ms.date: 01/16/2024
ms.topic: concept-article
---

# Supported Hosts

This document describes the architectures and operating systems supported by vcpkg.

## Dependencies

vcpkg also depends on a few system components to be installed before it can run.

To use vcpkg, a few components need to be installed on the system first.

* Windows: Visual Studio 2015 or later.
* macOS: XCode Console Tools, tar, git, zip, unzip, and curl. Most ports also need pkg-config.
We recommend installing these dependencies via Homebrew, such as via a terminal command
`brew install zip unzip curl pkgconfig`.
* Linux: A C++ compiler to use, tar, git, zip, unzip, and curl. Most ports also need pkg-config.
We recommend installing these dependencies with your system package manager.
  * apt-based platforms: `apt install git curl zip unzip pkgconfig`
  * rpm-based platforms: `dnf install git curl zip unzip pkgconfig`

For Linux and macOS users we also recommend to install the following packages: `autoconf`, `autoconf-archive`, `automake`, `make` and `libtool`. While not required, these packages are used often by libraries available in vcpkg.

## Curated registry support levels

Support is broken into 4 distinct categories. They are:

* Full support, tested  
  The configurations used to test vcpkg's curated registry on a regular basis. Problems in these configurations
  are rapidly found and fixed by vcpkg's maintainers and community..
* Full support, expected  
  Configurations treated with the same priority as 'Full support, tested' but which are not part of the regular tests.
  In general, these configurations are substantially similar to one of the fully tested configurations.
* Community support  
  Configurations that are not actively supported by vcpkg's maintainers but have substantial community investment.
  vcpkg maintainers accept changes to fix problems for these platforms but don't make guarantees that these 
  configurations work. The vcpkg team is interested in moving as many platforms out of community support
  into full support as possible.
* Unsupported  
  Configurations that are known no to work for some reason. In general, PRs fixing only these platforms
  are not accepted.

## Full support, tested

The fully supported, tested platforms are:

### Windows

* The latest Windows / Windows Server release. These are 11 and Server 2022 as of this writing.
* The latest Visual Studio update, Visual Studio 2022 17.8 as of this writing.

### macOS

macOS is intended to track the latest version of macOS and contemporary version of XCode Command Line tools. However,
updating macOS machines is a manual process, and macOS frequently changes things in ways that break vcpkg's testing. 
As of this writing, we are using:

* macOS 13.5.2
* XCode Command Line Tools 14.3.1

### Linux

* The latest LTS release of Ubuntu, currently 22.04.

### Android
* Linux 64-bit Android NDK version 25c

## Full support, expected

### Windows

* Windows 7 / Server 2008 R2 and later
* Visual Studio 2015 and later

### macOS

* The latest version of macOS, minus 2 major versions. For example, the current version of macOS is macOS 14 Sonoma,
so we expect vcpkg to work as far back as macOS 12 Monterey. This is intended to track with Apple's own support for
macOS.
* Contemporary versions of the XCode Console Tools for a given release of macOS

### Linux

We intend to support AMD64 builds of glibc-based Linuxes still in support from their distribution vendor released within
the last 5 years. Examples:

* Ubuntu 22.04 and 20.04 are both in support from Canonical and released within the last 5 years, so they
are expected to work. Ubuntu 18.04 is still in support from Canonical but was released more than 5 years ago, so
we no longer consider it fully supported. 20.10 is newer than 20.04 but we do not consider it fully supported because
it is no longer in support from Canonical.
* CentOS and Red Hat Enterprise Linux 8 is fully supported. CentOS and RHEL 7 are in support from Red Hat, but are
not expected to work as they were released more than 5 years ago.
* Fedora 39 and 38 are fully supported, but 37 is no longer supported by Fedora.
* Debian 12 "Bookworm", 11 "Bullseye", and 10 "Buster" are all supported by Debian and released in the last 5 years.
Debian 9 "Stretch" left support from the Debian project in July 2020 and is thus not supported by vcpkg.

We also assume that customers' build systems will match the version of Linux they are using, and take care to ensure
components like our Manifest Mode CMake integration will work with the versions of these dependencies that come with
one of the above distros. As of this writing, those dependency versions and the associated distro(s) are:

* GCC 8.3.0 (Debian 10)
* CMake 3.13.4 (Debian 10)
* Ninja 1.8.2 (Debian 10)
* Curl 7.64 (Debian 10)
* zip 3.0 (All)
* unzip 6.0 (All)

## Community support

### Windows

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

## Unsupported

* 32-bit Windows. Targeting x86-windows or arm-windows works just fine, but vcpkg itself expects to run AMD64 copies
of support tooling like CMake.
* Visual Studio 2013 or earlier.
* Windows Vista or earlier.
* macOS or Linux hosts older than those in the *Fully supported* categories.
