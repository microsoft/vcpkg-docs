vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO Microsoft/vcpkg-docs
  REF 19a8bfd55570565dec49c87aa035e23714afbf88
  SHA512 ed5da17843b6ee9ff477c2b6cbc7dd709c6d881c4bd41b540536d0514d114da5bf0e7a584547aedb73448e0653ce933ae04f5ebcc5d39953ff0a09a1fbb13651
  HEAD_REF lib-beison
)

vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
