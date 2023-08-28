---
title: vcpkg x-add-version
description: Reference for the vcpkg x-add-version command. Update version database of vcpkg ports.
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

The `x-add-version` command updates the [version database](..\users\versioning.concepts.md#acquiring-port-versions) for vcpkg ports. By default, it operates on a specified port. With the appropriate switches, users can choose to process all ports or change the default behavior regarding formatting checks and version updates.

To use the command:
1. After making changes to a port, navigate to the vcpkg directory. 
2. Run `vcpkg x-add-version <port-name>`, replacing `<port-name>` with the name of the port you've edited.

This will add or update the version entry for your port in the version database.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `[port-name]`

Specifies the name of the port to be updated. If not provided, the user should use the `--all` option to process all ports.

### `--all`
Processes all the ports in the [built-in](..\maintainers\registries.md#builtin-registries) `ports` directory.

### `--overwrite-version`

Allows overwriting an existing version in the database.

Without this flag, vcpkg will refuse to overwrite existing versions.

> [!NOTE]
> This option is designed for ongoing development when preparing a version for registry inclusion. For example, consider addressing an issue in the `foo` port and executing `vcpkg x-add-version foo`. For subsequent modifications to the port, utilize `vcpkg x-add-version foo --overwrite-version` to refresh the version entry prior to registry inclusion.

### `--skip-formatting-check`

Skips the check for proper formatting in the manifest file (`vcpkg.json`) of the port. By default, proper formatting is checked and required.

### `--skip-version-format-check`

Skips the version format check. By default, versions are checked to ensure they adhere to a specific [format](..\users\versioning.md#version-schemes).

### `--verbose`

Provides verbose output, giving more details about the operations being carried out.


