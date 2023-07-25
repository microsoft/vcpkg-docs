---
title: vcpkg edit
description: Command-line reference for the vcpkg edit command. Opens a port's contents in a text editor window.
author: vicroms
ms.author: viromer
ms.date: 7/24/2023
ms.prod: vcpkg
---
# vcpkg edit

## Synopsis

```Console
vcpkg edit <ports> [--all] [--buildtrees] [options]
```

## Description

Opens a port for editing in a text editor window (defaults to Visual Studio Code).

If multiple port names are provided, all ports are opened in the same window.
This command does not work with ports from external registries, only ports from the built-in
registry can be opened.

The `--buildtrees` option opens the package's `buildtrees` folder instead of the port's contents.
The `--all` option also opens the port's related `packages`, and `buildtrees` folders in the same window.

By default, vcpkg will search for Visual Studio Code in well-known installation paths.
Set the `EDITOR` environment variable to specify a text editor program to use.

On Windows, vcpkg searches for a Visual Studio Code or Visual Studio Code Insiders installation in:

* the `%ProgramFiles%` and `%ProgramFiles(x86)%` folders
* the `%APPDATA%\Local\Programs` folder
* the Windows Registry

On Linux, vcpkg searches for the Visual Studio Code executable:

* `/usr/bin/code`
* `/usr/share/code/bin/code`

On MacOS, vcpkg searches for a Visual Studio Code or Visual Studio Code Insiders installation in the `Applications` folder.

If Visual Studio Code is not found, vcpkg attempts to use the default text editor configured for
your system.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--all`

Opens the port's related `buildtrees`, and `packages` folders.

### `--buildtrees`

Opens the port's `buildtrees` folder instead of the port's contents. The `buildtrees` folder
is created during the installation process and contains the package's extracted source code and log files.

Use in combination with the `vcpkg install --editable <port>` command to get a clean copy of the
package's source code suitable for debugging and creating patch files.
