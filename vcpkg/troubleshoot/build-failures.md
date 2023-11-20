---
title: Troubleshoot build failures
description: Troubleshooting guide for common build failures in vcpkg ports
author: vicroms
ms.author: viromer
ms.date: 11/20/2023
ms.prod: vcpkg
ms.topic: troubleshooting-general
---

# Troubleshoot build failures in vcpkg ports

This guide is intended for users experiencing issues while building ports using vcpkg.

<!-- 
## <a name="debug-output"></a> Enable vcpkg debugging information

It is highly recommended that you enable debug output when following this guide.

* [Classic mode](classic-mode.md): add `--debug` to your command invocation.
* CMake toolchain: add `-DVCPKG_INSTALL_OPTIONS="--debug"` in your CMake
  configure call or in your `CMakePresets.json` file.
* MSBuild/Visual Studio: set the property `VcpkgAdditionalInstallOptions` to
  `--debug`.
* Set the `VCPKG_INSTALL_OPTIONS` environment variable to `--debug`. 
-->

## <a name="install-logs"></a> Locate installation logs

Build failures can happen for an almost infinite number of reasons. When a build
failure occurs in the port's underlying build system the first step to diagnose
it is to read the log files.

vcpkg prints the location of the log file for the last step it executed before
the error ocurred. Look for the line "See logs for more infomation:" in the
vcpkg output.

Example: Log output

```Console
    See logs for more information:
      C:\Users\viromer\work\vcpkg\buildtrees\detect_compiler\config-x64-linux-rel-CMakeCache.txt.log
      C:\Users\viromer\work\vcpkg\buildtrees\detect_compiler\config-x64-linux-out.log
```

You can find all the produced log files by inspecting the port's `buildtrees`
directory located in `$VCPKG_ROOT/buildtrees/<port>/` (replace `<port>` with the
appropriate port name).

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
algorithm](../users/triplets.md#vcpkg_visual_studio_path) to learn more.

* Set the
  [`VCPKG_PLATFORM_TOOLSET`](../users/triplets.md#vcpkg_platform_toolset) to the
  appropriate version.
* Alternatively, set the
  [`VCPKG_VISUAL_STUDIO_PATH`](../users/triplets.md#vcpkg_visual_studio_path) to
  your desired Visual Studio instance installation path.

## Issue isn't listed here

If your issue isn't listed here, visit [our
repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
