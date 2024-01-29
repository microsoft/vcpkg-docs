---
title: vcpkg_backup_env_variables
description: Use vcpkg_backup_env_variables to backup a set of environment variables.
ms.date: 01/10/2024
---
# vcpkg_backup_env_variables

Backup a set of environment variables.

## Usage

```cmake
vcpkg_backup_env_variables(VARS [<environment-variable>...])
```

### VARS

The variables to backup.

## Notes

Variables are backed up into the current scope. You must call [`vcpkg_restore_env_variables()`](vcpkg_restore_env_variables.md from the same scope as `vcpkg_backup_env_variables()`.

`vcpkg_restore_env_variables()` does not destroy the variables backed up with `vcpkg_backup_env_variables()` and so can be called multiple times within the same scope to reset back to the original values.

## Source

[scripts/cmake/vcpkg\_backup\_restore\_env\_vars.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_backup_restore_env_vars.cmake)
