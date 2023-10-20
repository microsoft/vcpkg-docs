cmake_minimum_required(VERSION 3.10)

project(my_sample_lib)

# Find the fmt library
find_package(fmt CONFIG REQUIRED)

# Add your library
add_library(my_sample_lib STATIC my_sample_lib.cpp)

# Link your library to fmt
target_link_libraries(my_sample_lib PRIVATE fmt::fmt)

# Add include directories
target_include_directories(my_sample_lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   # for headers when building
    $<INSTALL_INTERFACE:include>  # for client in install mode
)
# Install the library and its headers
install(TARGETS my_sample_lib
        EXPORT my_sample_lib_targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES my_sample_lib.h DESTINATION include)


# Generate and install *-targets.cmake 
install(EXPORT my_sample_lib_targets
        FILE my_sample_lib-targets.cmake
        NAMESPACE my_sample_lib::
        DESTINATION share/my_sample_lib)

# Generate the config file in the current binary dir (this ensures it's not placed directly in source)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/my_sample_lib-config.cmake"
"include(CMakeFindDependencyMacro)\n"
"find_dependency(fmt CONFIG REQUIRED)\n"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/my_sample_lib-targets.cmake\")\n"
)

# Install the generated config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/my_sample_lib-config.cmake"
        DESTINATION share/my_sample_lib)
