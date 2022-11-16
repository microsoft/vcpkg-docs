# z_vcpkg_setup_pkgconfig_path

**Only for internal use in vcpkg helpers. Behavior and arguments will change without notice.**

`z_vcpkg_setup_pkgconfig_path` sets up environment variables to use `pkgconfig`, such as `PKG_CONFIG` and `PKG_CONFIG_PATH`.
The original values are restored with `z_vcpkg_restore_pkgconfig_path`. `BASE_DIRS` indicates the base directories to find `.pc` files; typically `${CURRENT_INSTALLED_DIR}`, or `${CURRENT_INSTALLED_DIR}/debug`.

```cmake
z_vcpkg_setup_pkgconfig_path(BASE_DIRS <"${CURRENT_INSTALLED_DIR}" ...>)
# Build process that may transitively invoke pkgconfig
z_vcpkg_restore_pkgconfig_path()
```


## Source
[scripts/cmake/z\_vcpkg\_setup\_pkgconfig\_path.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/z_vcpkg_setup_pkgconfig_path.cmake)
