vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO Microsoft/vcpkg-docs
  REF b0456132ea9a6e23cf36710eabae478e54e71ec0
  SHA512 5e11038aeebd657ebdaf4d442faa303bc5fef45a3bbdb56fd5e3e872ba26c49917c45b2dc120ecbcec1b7e22e6a571b0f7aa5b1eab2273eb9274846329f09a34
  HEAD_REF lib-beicode
)

vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
