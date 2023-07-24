---
title: vcpkg edit
description: Command-line reference for the vcpkg edit command. Opens a port's folder in a new code editor window.
author: vicroms
ms.author: viromer
ms.date: 7/24/2023
ms.prod: vcpkg
---
# vcpkg edit

## Synopsis

```Console
vcpkg edit <ports> [--options]
```

## Description

Opens a port's corresponding `ports` folder in a new _Visual Studio Code_ window.

The `--buildtrees` option opens the package's `buildtrees` folder instead of its corresponding `ports`
folder. The `--all` option also opens the port's related `packages`, `installed`, and `buildtrees`
folders in the same window.

The command accepts multiple port names, in which case, all ports are opened in the same window.

By default, vcpkg will search for _Visual Studio Code_ in well-known installation paths, set the
`EDITOR` environment variable to add a _Visual Studio Code_ executable not in the usual installation
paths.

On Windows, vcpkg searches for a _Visual Studio Code_ or _Visual Studio Code - Insiders_ installation in:

* the _Program Files_ and _Program Files (x86)_ folders,
* the `%APPDATA%\Local\Programs` folder,
* the Windows Registry.

On Linux, vcpkg searches for the _Visual Studio Code_ executable:

* `/usr/bin/code`
* `/usr/share/code/bin/code`

On MacOS, vcpkg searches for a _Visual Studio Code_ or _Visual Studio Code - Insiders_ installation in the `Applications` folder.

## Options

### `--all`

Opens the port's related `buildtrees`, `packages`, and `installed` folders.

### `--buildtrees`

Opens the port's `buildtrees` folder instead of the port's `ports` folder. The `buildtrees` folder
is created during the installation process and contains the package's extracted source code and log files.

Use in combination with the `vcpkg install --editable <port>` command to get a clean copy of the
package's source code suitable for creating patch files.
