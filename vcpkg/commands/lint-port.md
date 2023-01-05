---
title: vcpkg x-lint-port
description: Command line reference for the vcpkg x-lint-port command. Searches and fixes common port bugs.
ms.date: 01/05/2023
---
# vcpkg x-lint-port

> [!NOTE]
> This command is experimental and may change or be removed at any time.

## Synopsis

```console
vcpkg x-update-baseline [options] [--fix] [--increase-version] [--all] [package]
```

## Description

Lints a port and fixes found problems if `--fix` was passed.

It performs the following checks:
- Check used version scheme (auto fix possible):  
    
  If `version-string` is used check if `version` or `version-date` would also be possible and suggest that

- Check for license expression (auto fix not possible):  
  
  Emits an warning if the port does not have a license expression

- Check for deprecated license expressions (auto fix often possible):  
  
  Warns when a deprecated license expressions is used and suggests a non deprecated one.

- Check for the use of deprecated functions in `portfile.cmake` (auto fix often possible):  
  
  The following checks are performed:
  - `vcpkg_build_msbuild` -> `vcpkg_install_msbuild` (auto fix not avaiable)
  - `vcpkg_configure_cmake` -> `vcpkg_cmake_configure` (auto fix avaiable)
  - `vcpkg_build_cmake` -> `vcpkg_cmake_build` (auto fix avaiable)
  - `vcpkg_install_cmake` -> `vcpkg_cmake_install` (auto fix avaiable)
  - `vcpkg_fixup_cmake_targets` -> `vcpkg_fixup_cmake_targets` (auto fix avaiable)
  - `vcpkg_extract_source_archive_ex` -> `vcpkg_extract_source_archive` (auto fix avaiable)
  - deprecated `vcpkg_check_features` call without `FEATURES` keyword (auto fix avaiable)


## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--fix`

The command tries to fix as many problems as possible.

### `--increase-version`

- **only when `--fix` is passed**

Increases the `port-version` of ports which where fixed.

### `--all`

Instead of linting the given ports lint all avaiable ports.
