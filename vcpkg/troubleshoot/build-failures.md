---
title: Troubleshoot build failures
description: Troubleshooting guide for common build failures in vcpkg ports
author: vicroms
ms.author: viromer
ms.date: 12/12/2023
ms.prod: vcpkg
ms.topic: troubleshooting-general
---

# Troubleshoot build failures in vcpkg ports

This guide is intended for users experiencing issues while [installing 
ports](../commands/install.md) using vcpkg.

## <a name="failure-logs"></a> Locate failure logs

Build failures can happen for an almost infinite number of reasons. When vcpkg
fails to build a port, the first step to diagnose the reason is to read the log files.

vcpkg generates a log file for each external process that gets invoked while
building a port. When an error occurs, vcpkg prints the location of the log file
for the last process that ran before the error ocurred. Look for the line "See
logs for more infomation:" in the vcpkg output.

Example: Log files location output

```Console
    See logs for more information:
      C:\Users\viromer\work\vcpkg\buildtrees\detect_compiler\config-x64-linux-rel-CMakeCache.txt.log
      C:\Users\viromer\work\vcpkg\buildtrees\detect_compiler\config-x64-linux-out.log
```

You can find all the produced log files by inspecting the port's `buildtrees`
directory located in `$VCPKG_ROOT/buildtrees/<port>/` (replace `<port>` with the
appropriate port name).

## Failed to download the port's assets

> [!NOTE]
> Using [asset caching](../concepts/asset-caching.md) can help you mitigate this
> kind of problems by ensuring continued availability of cached assets.

When installing a port an error occurs while downloading an asset from the
network.

```Console
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:32 (message):

  Failed to download file with error: 1
```

### Cause 1: The download URL is no longer valid
<!-- 
Steps to reproduce:
1. Modify a port's download URL
2. Attempt to install
-->

By reasons outside vcpkg's control URLs can become invalid. Broken links can be
diagnosed by using a web browser to navigate to the download URL, a broken link
will produce a 404 status code.

Steps to resolve:

1 - Modify the port to use an alternative download URL for the asset.

### Cause 2: The file's hash does not match the expected SHA512
<!-- 
Steps to reproduce:
1. Modify a port's download SHA512
2. Attempt to install
-->

```Console
error: Failed to download from mirror set
error: File does not have the expected hash:
url: https://github.com/OpenImageIO/oiio/archive/v2.4.13.0.tar.gz
File: /home/user/vcpkg/downloads/OpenImageIO-oiio-v2-9325beef.4.13.0.tar.gz.1925416.part
Expected hash: 9325beefce55b66a58fcfc2ce93e1406558ed5f6cc37cb1e8e04aee470c4f30a14483bebfb311c329f7868afb6c508a052661c6b12d819a69f707c1a30cd9549
Actual hash: 9e887c7039995ce7c41556e09a7eed940863260a522ecf7d9bec300189026ed507da560620dfa4a619deeb679be7adf42fe3a7020ff3094df777c7934c771227
```

This error occurs when the upstream file has been changed in any way by the
server but the download URL is kept the same. As a security measure, vcpkg will
reject files with SHA512 that don't match the expected SHA512 for the asset.

Steps to resolve:

1 - Verify that the downloaded file is secure
2 - Modify the port to use the new file's SHA512

## Visual Studio toolchain is not found

When installing a port vcpkg is not able to locate an appropriate toolchain:

```Console
error: in triplet arm-windows: Unable to find a valid toolchain for requested target architecture arm.
The selected Visual Studio instance is at: C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools
The available toolchain combinations are: x86, amd64, x86_amd64, amd64_x86
```

### Cause 1: The appropriate toolchain for the target architecture is not installed
<!-- 
Steps to reproduce:
1. Uninstall a required toolchain from Visual Studio (e.g.: ARM)
2. Attempt to install for a triplet that requires that toolchain (for example: arm-windows)
-->

If the Visual Studio instance matches your desired vcpkg version and you get
this error, the most likely cause is that the appropriate toolchain is not installed.

* Open the Visual Studio Installer and install the appropriate toolchain.

### Cause 2: The selected Visual Studio version is incorrect
<!--
Steps to reproduce:
1. Uninstall a required toolchain from your latest Visual Studio version (e.g.: ARM)
2. Attempt to install for a triplet that requires that toolchain (for example: arm-windows)
-->

If the required toolchain is installed, vcpkg may have selected an incorrect
version of Visual Studio where the toolchain is not installed. See the
[documentation for the Visual Studio selection
algorithm](../users/triplets.md#VCPKG_VISUAL_STUDIO_PATH) to learn more.

Steps to resolve:

1 - Set the
  [`VCPKG_PLATFORM_TOOLSET`](../users/triplets.md#vcpkg_platform_toolset) to the
  appropriate version.
2 - Alternatively, set the
  [`VCPKG_VISUAL_STUDIO_PATH`](../users/triplets.md#VCPKG_VISUAL_STUDIO_PATH) to
  your desired Visual Studio instance installation path.

## Missing system dependencies

A required build tool or system dependency is not installed in the environment.

Example: Port requires system dependencies

```
alsa currently requires the following programs from the system package manager:
    autoconf autoheader aclocal automake libtoolize
On Debian and Ubuntu derivatives:
    sudo apt install autoconf libtool
On recent Red Hat and Fedora derivatives:
    sudo dnf install autoconf libtool
On Arch Linux and derivatives:
    sudo pacman -S autoconf automake libtool
On Alpine:
    apk add autoconf automake libtool
```

Most vcpkg ports that require system dependencies print a message during installation.
In some cases, the list of required system dependencies may be incomplete. Diagnosing
this type of issues depends on the underlying built system, [check the error 
logs](#failure-logs) to determine the cause of the failure.

Steps to resolve:

1 - Follow the appropriate steps to install the missing system dependency in your environment.

## Issue isn't listed here

If your issue isn't listed here, visit [our
repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
