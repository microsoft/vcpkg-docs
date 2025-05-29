---
title: "Tutorial: Install a locally modified dependency"
description: This tutorial shows you how to modify a vcpkg port and install it using overlays"
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: tutorial
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections

#CustomerIntent: As a vcpkg user, I want to learn how to modify a vcpkg provided dependency and install it
---

# Tutorial: Install a locally modified dependency

This tutorial guides you on modifying a vcpkg port using custom overlays.
We recommend that you read the tutorial on [packaging a
library](../get_started/get-started-packaging.md) before proceeding.

In this tutorial, you will learn to:

> [!div class="checklist"]
> * [Create an overlay port](#1---create-an-overlay-port)
> * [Get the port's source code](#2---get-the-ports-source-code)
> * [Create a temporary Git registry](#3---create-a-temporary-git-registry)
> * [Modify the necessary files](#4---modify-the-necessary-files)
> * [Generate a patch file](#5---generate-a-patch-file)
> * [Modify `portfile.cmake` to apply the patch file](#6---modify-portfilecmake-to-apply-the-patch)
> * [Install your overlay port](#7---install-your-overlay-port)

## Prerequisites

* A terminal
* vcpkg
* [Git](<https://git-scm.com/downloads>)
* A C++ compiler
* Completion of the [packaging tutorial](../get_started/get-started-packaging.md)

## 1 - Create an overlay port

The first step is to create an [overlay port](../concepts/overlay-ports.md) of
the package you want to modify.

### Create a directory to hold your overlay ports

The overlay ports directory can be created in any filesystem location of your choosing. In any step of this tutorial, replace `$OVERLAY_LOCATION` with your chosen location.

::: zone pivot="shell-powershell, shell-cmd"

```console
mkdir "$OVERLAY_LOCATION"
```

::: zone-end
::: zone pivot="shell-bash"

```bash
mkdir "$OVERLAY_LOCATION"
```

::: zone-end

### Copy the contents of the port into your overlay ports directory

For this tutorial, you'll modify the `vcpkg-sample-library` port in the
[packaging tutorial](../get_started/get-started-packaging.md) to add dynamic
library support.

::: zone pivot="shell-powershell"

```powershell
Copy-Item -Path <path/to/vcpkg-sample-library> -Destination "$OVERLAY_LOCATION" -Recurse
```

::: zone-end
::: zone pivot="shell-cmd"

```console
xcopy <path/to/vcpkg-sample-library> "$OVERLAY_LOCATION" /E
```

::: zone-end
::: zone pivot="shell-bash"

```bash
cp -R <path/to/vcpkg-sample-library> "$OVERLAY_LOCATION"
```

::: zone-end

## 2 - Get the port's source code

To obtain the source code for the port you want to modify, run the following command:

`vcpkg install "--overlay-ports=$OVERLAY_LOCATION" vcpkg-sample-library --editable`

The output should contain a line similar to:

```Console
-- Using source at path/to/vcpkg/buildtrees/vcpkg-sample-library/src/1.0.0-b54c55c215
```

This is the location of the port's source code.

## 3 - Create a temporary Git registry

Initialize a temporary Git repository in the source code location. This is in order
to use Git to generate patch files that you can apply in the `portfile.cmake`
script. Replace `$SOURCE_PATH` with the location obtained in the previous step.

```console
cd "$SOURCE_PATH"
git init
git add .
git commit -m "Initial commit"
```

## 4 - Modify the necessary files

Modify the following source code files to add dynamic library support to
`vcpkg-sample-library`.

`my_sample_lib.h`

```cpp
#pragma once

#include <string>

#if MYLIB_EXPORTS
__declspec(dllexport)
#endif
std::string greet(const std::string& name);
```

`CMakeLists.txt`

Remove `STATIC` from the `add_library()` call.

```cmake
add_library(my_sample_lib my_sample_lib.cpp)
```

Add a compile definition when building as a shared library.

```cmake
if (BUILD_SHARED_LIBS)
    target_compile_definitions(my_sample_lib PRIVATE MYLIB_EXPORTS)
endif()
```

## 5 - Generate a patch file

In the source code directory, run the following command to generate a patch file.

```Console
git diff --output "$OVERLAY_LOCATION/vcpkg-sample-library/add-dynamic-lib-support.patch"
```

This creates a file named `add-dynamic-lib-support.patch` in your overlay port,
with contents similar to:

```diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 2b42f71..378aac7 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -2,26 +2,33 @@ cmake_minimum_required(VERSION 3.10)
 
 project(my_sample_lib)
 
+include(GNUInstallDirs)
+
 # Find the fmt library
 find_package(fmt CONFIG REQUIRED)
 
 # Add your library
-add_library(my_sample_lib STATIC my_sample_lib.cpp)
+add_library(my_sample_lib my_sample_lib.cpp)
+
+if (BUILD_SHARED_LIBS AND MSVC)
+    target_compile_definitions(my_sample_lib PRIVATE MYLIB_EXPORTS)
+endif()
 
 # Link your library to fmt
 target_link_libraries(my_sample_lib PRIVATE fmt::fmt)
 
+
 # Add include directories
 target_include_directories(my_sample_lib PUBLIC 
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   # for headers when building
-    $<INSTALL_INTERFACE:include>  # for client in install mode
+    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # for client in install mode
 )
 # Install the library and its headers
 install(TARGETS my_sample_lib
         EXPORT my_sample_lib_targets
-        ARCHIVE DESTINATION lib
-        LIBRARY DESTINATION lib
-        RUNTIME DESTINATION bin)
+        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
+        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
+        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
 
 install(FILES my_sample_lib.h DESTINATION include)
 
diff --git a/my_sample_lib.h b/my_sample_lib.h
index d6d70b8..0b62141 100644
--- a/my_sample_lib.h
+++ b/my_sample_lib.h
@@ -2,4 +2,7 @@
 
 #include <string>
 
+#if MYLIB_EXPORTS
+__declspec(dllexport)
+#endif
 std::string greet(const std::string& name);

```

## 6 - Modify `portfile.cmake` to apply the patch

Change `portfile.cmake` to remove the `ONLY_STATIC_LIBRARY` restriction and
apply your patch to the source code.

`portfile.cmake`

```cmake
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO MicrosoftDocs/vcpkg-docs
    REF 1.0.0 # This is for demonstration purposes as '1.0.2' of the sample library already has dynamic library support
    SHA512 3f206cc2fe61d9c97c82b30852e1e4e6df299d93f6159edd1e56c644fa03ccc4670f7681e356d0e3db898a74e099a1ec531821df5430a7b14d61c743c5aa8c30
    HEAD_REF cmake-sample-lib
    PATCHES
        "add-dynamic-lib-support.patch"
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
```

## 7 - Install your overlay port

Run the following command:

`vcpkg install "--overlay-ports=$OVERLAY_LOCATION" vcpkg-sample-library`

Make sure to remove the `--editable` flag, so that the patch file applies to a
clean copy of the source code. You should see a line about applying your patch
file to the source code in the output:

```Console
-- Cleaning sources at D:/Work/vcpkg/buildtrees/vcpkg-sample-library/src/1.0.0-8f646312ed.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source D:/Work/vcpkg/downloads/Microsoft-vcpkg-docs-1.0.0.tar.gz
-- Applying patch add-dynamic-lib-support.patch
```

## Next steps

And that's it! You have installed a locally modified dependency using a patch
file.

Here are some tasks to try next:

* [Create a custom Git registry](../produce/publish-to-a-git-registry.md)
