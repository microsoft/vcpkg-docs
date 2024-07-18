cmake_minimum_required(VERSION 3.15)

set(beicode_VERSION 1.0.0)

project(beicode
	VERSION ${beicode_VERSION}
	DESCRIPTION "A simple utf-8 based unicode decoding and encoding library"
	HOMEPAGE_URL "https://github.com/northwindtraders/beicode"
	LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(beicode src/beicode.cpp include/beicode.h)
add_library(beicode::beicode ALIAS beicode)

if(BUILD_SHARED_LIBS AND WIN32)
	target_compile_definitions(beicode
		PRIVATE "BEICODE_EXPORT=__declspec(dllexport)"
		INTERFACE "BEICODE_EXPORT=__declspec(dllimport)")
endif()

target_compile_features(beicode PUBLIC cxx_std_17)
target_include_directories(beicode
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

configure_package_config_file(cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/beicode-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/beicode
	NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/beicode-config-version.cmake
	VERSION ${beicode_VERSION}
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/beicode-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/beicode-config-version.cmake
	DESTINATION
		${CMAKE_INSTALL_DATADIR}/beicode)

install(TARGETS beicode EXPORT beicode-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT beicode-targets
	NAMESPACE beicode::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/beicode)
