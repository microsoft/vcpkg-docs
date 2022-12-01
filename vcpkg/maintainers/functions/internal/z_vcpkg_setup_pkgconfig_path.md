---
title: z_vcpkg_setup_pkgconfig_path
---

# z_vcpkg_setup_pkgconfig_path

[!INCLUDE [internal-helper](../../../../includes/internal-helper.md)]

Set up the environment to use `pkgconfig`.

## Usage
```cmake
z_vcpkg_setup_pkgconfig_path(BASE_DIRS <base-dirs>)
```

## Parameters
### BASE_DIRS
List of base directories to use.

Typically `${CURRENT_INSTALLED_DIR}` or `${CURRENT_INSTALLED_DIR}/debug`, but not both.

## Notes
Acquires and sets `PKG_CONFIG` to a valid `pkg-config` program.

For each entry of `BASE_DIRS`, prepends `<entry>/lib` and `<entry>/share` to `PKG_CONFIG_PATH` in an unspecified order.

See also [`z_vcpkg_restore_pkgconfig_path`](z_vcpkg_restore_pkgconfig_path.md).

## Examples
```cmake
z_vcpkg_setup_pkgconfig_path(BASE_DIRS "${CURRENT_INSTALLED_DIR}")
# Build process that may internally invoke pkgconfig
vcpkg_execute_build_process(COMMAND my-buildsystem.py -release ...)
z_vcpkg_restore_pkgconfig_path()
if(NOT VCPKG_BUILD_TYPE)
  z_vcpkg_setup_pkgconfig_path(BASE_DIRS "${CURRENT_INSTALLED_DIR}/debug")
  # Build process that may internally invoke pkgconfig
  vcpkg_execute_build_process(COMMAND my-buildsystem.py -debug ...)
  z_vcpkg_restore_pkgconfig_path()
endi()
```

## Source
[scripts/cmake/z\_vcpkg\_setup\_pkgconfig\_path.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/z_vcpkg_setup_pkgconfig_path.cmake)
