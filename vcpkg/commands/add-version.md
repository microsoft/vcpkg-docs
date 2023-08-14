---
title: vcpkg x-add-version
description: Command line reference for the vcpkg x-add-version command. Update version database of vcpkg ports.
author: JavierMatosD
ms.author: javiermat
ms.date: 08/14/2023
ms.prod: vcpkg
---

# vcpkg x-add-version

[!INCLUDE [experimental](../../includes/experimental.md)]

## Synopsis

```console
vcpkg x-add-version [port-name] [options] [--all] [--overwrite-version] [--skip-formatting-check] [--skip-version-format-check] [--verbose]
```

## Description

The `x-add-version` command updates the version database for vcpkg ports. By default, it operates on a specified port. With the appropriate switches, users can choose to process all ports or change the default behavior regarding formatting checks and version updates.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `[port-name]`

Specifies the name of the port to be updated. If not provided, the user should use the `--all` option to process all ports.

### `--all`

Processes all the ports in the built-in ports directory. If this option is used in conjunction with a specified port name, a warning will be issued, and the specific port will be ignored. 

### `--overwrite-version`

Overwrites an existing version in the database. Useful when updating an existing version entry. 

### `--skip-formatting-check`

Skips the check for proper formatting in the manifest file (`vcpkg.json`) of the port. By default, proper formatting is checked and required.

### `--skip-version-format-check`

Skips the version format check. By default, versions are checked to ensure they adhere to a specific format.

### `--verbose`

Provides verbose output, giving more details about the operations being carried out.


