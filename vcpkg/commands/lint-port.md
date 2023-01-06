---
title: vcpkg x-lint-port
description: Command line reference for the vcpkg x-lint-port command. Searches and fixes common port bugs.
ms.date: 01/05/2023
---
# vcpkg x-lint-port

[!INCLUDE [experimental](../../includes/experimental.md)]

## Synopsis

```console
vcpkg x-update-baseline [options] [--fix] [--increase-version] [--all | port...]
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

- Check for the use of deprecated functions in `portfile.cmake`: 

  - `vcpkg_build_msbuild` -> `vcpkg_install_msbuild` (auto fix not available)
  - `vcpkg_configure_cmake` -> `vcpkg_cmake_configure` (auto fix available)
  - `vcpkg_build_cmake` -> `vcpkg_cmake_build` (auto fix available)
  - `vcpkg_install_cmake` -> `vcpkg_cmake_install` (auto fix available)
  - `vcpkg_fixup_cmake_targets` -> `vcpkg_fixup_cmake_targets` (auto fix available)
  - `vcpkg_extract_source_archive_ex` -> `vcpkg_extract_source_archive` (auto fix available)

- `vcpkg_check_features` call without `FEATURES` keyword (auto fix available)


## Options

All vcpkg commands support a set of [common options](common-options.md).

### `port...`

The list of ports to lint and fix.

Required if `--all` was not passed.

### `--fix`

Apply all automatic fixes to the port files.

### `--increase-version`

Increases the `port-version` of ports which where fixed.

Requires `--fix` to also be passed.

### `--all`

Instead of linting the given ports lint all available ports.
