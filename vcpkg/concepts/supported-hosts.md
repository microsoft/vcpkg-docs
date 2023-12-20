---
title:  Supported Hosts
description: This article explains the concept of triplets in vcpkg and their capabilities.
author: bion
ms.author: bion
ms.date: 12/19/2023
ms.topic: concept-article
---

# Supported Hosts - vcpkg dependencies

To use vcpkg, a few components need to be installed on the system first.

* Prerequisites
** Windows
*** Visual Studio
** macOS
*** xcode console tools
*** git
*** brew: zip, unzip, curl. Most ports will also need pkgconfig
** Linux
*** git, zip, unzip, curl
*** a compiler
** Android
*** ???

* Full support, tested
These configurations are those in vcpkg's own labs used to validate changes and submissions to
github.com/microsoft/vcpkg.

** Windows
*** Windows 11 / Server 2022
*** Latest Visual Studio Update
** macOS
*** Most recent version
*** Most recent XCode console tools
** Linux
*** Ubuntu's latest LTS
** Android

* Full support, expected
These are platforms we care about and intend to be 100% functional, but do not test every day. We consider problems with any of these platforms to be serious bugs when they are reported. Generally, we only keep platforms under 'full support' if we know users will report problems to us in a timely manner.

** Windows
*** Windows 7 / Server 2008 R2 and later
*** Visual Studio 2015 and later
** macOS
*** Latest major version minus 2 or later
*** Contemporary XCode console tools for that macOS release
** Linux
*** amd64
*** Ubuntu, CentOS, Debian, Fedora, RHEL, released within the last 5 years, and still in support by their vendor. Example: Debian 9 is not included because while it is still in support from Debian, it was released more than 5 years ago at the time of this writing. Ubuntu 20.10 is not included because while it is newer than 5 years old, it is no longer in support from Canonical. (Ubuntu 20.04 is in support from Canonical at time of writing, so it is intended to be supported by vcpkg)
** Android

* Community support
These are platforms we are interested in targeting but for which we currently rely entirely on community contributions.

** MinGW Windows. Several community members do use this though.
** Clang as the build compiler on Windows. Several community members do use this though.
** Non-amd64 Linux. We are extremely interested in help from folks targeting arm64 linux if it can be shown how to deliver binaries that have a high likelihood of success.
** Non-glibc Linux, such as Alpine
** Not one of the above distros Linux
** BSDs

* Unsupported or known not to work
** 32 bit versions of Windows
** Visual Studio 2013 or earlier
** Windows Vista or earlier
