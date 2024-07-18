cmake_minimum_required(VERSION 3.15)

set(beison_VERSION 1.0.1)

project(beison
	VERSION ${beison_VERSION}
	DESCRIPTION "A simple json parsing and stringifying library"
	HOMEPAGE_URL "https://github.com/northwindtraders/beison"
	LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

find_package(beicode CONFIG REQUIRED)

add_library(beison src/beison.cpp include/beison.h)
add_library(beison::beison ALIAS beison)

if(BUILD_SHARED_LIBS AND WIN32)
	target_compile_definitions(beison
		PRIVATE "BEISON_EXPORT=__declspec(dllexport)"
		INTERFACE "BEISON_EXPORT=__declspec(dllimport)")
endif()

target_compile_features(beison PUBLIC cxx_std_17)
target_include_directories(beison
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(beison PRIVATE beicode::beicode)

configure_package_config_file(cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/beison-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/beison
	NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/beison-config-version.cmake
	VERSION ${beison_VERSION}
	COMPATIBILITY SameMajorVersion)
install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/beison-config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/beison-config-version.cmake
	DESTINATION
		${CMAKE_INSTALL_DATADIR}/beison)

install(TARGETS beison EXPORT beison-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT beison-targets
	NAMESPACE beison::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/beison)
