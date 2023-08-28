---
title: Microsoft Visual C++ for Windows
description: Building for Windows with the Microsoft Visual C++ Compiler
ms.date: 04/28/2023
---
# Microsoft Visual C++

## Triplets

vcpkg includes [triplets](https://github.com/microsoft/vcpkg/tree/master/triplets) for building Windows desktop applications using the MSVC ``cl.exe`` compiler.

| Architecture | vcpkg triplets               | Community |
|--------------|------------------------------|-----------|
| x64          | x64-windows                  |           |
|              | x64-windows-release          | Yes       |
|              | x64-windows-static           |           |
|              | x64-windows-static-md        | Yes       |
|              | x64-windows-static-release   | Yes       |
| x86          | x86-windows                  |           |
|              | x86-windows-static           | Yes       |
|              | x86-windows-static-md        | Yes       |
| arm          | arm-windows                  | Yes       |
|              | arm-windows-static           | Yes       |
| arm64        | arm64-windows                |           |
|              | arm64-windows-static         | Yes       |
|              | arm64-windows-static-md      | Yes       |
|              | arm64-windows-static-release | Yes       |
| arm64ec      | arm64ec-windows              | Yes       |

The ``static`` linking triplets are set to use the MSVC Runtime as a static library (i.e. ``VCPKG_CRT_LINKAGE static``).

The ``static-md`` linking triplets are set to use the MSVC Runtime as a DLL (i.e. ``VCPKG_CRT_LINKAGE dynamic``). This is the recommended solution for redistributing the MSVC Runtime per [Microsoft Learn](/cpp/windows/deployment-in-visual-cpp).

## Selecting a Visual C++ Toolset

By default, vcpkg will use the latest version of Visual Studio installed on the system for building code. To select a specific version, create a custom triplet or triplet overlay to set [`VCPKG_PLATFORM_TOOLSET`](../triplets.md#vcpkg_platform_toolset).

For examples, this would force the use of the VS 2017 toolset.

```
set(VCPKG_PLATFORM_TOOLSET v141)
```

## C/C++ Runtime Compatibility

The Microsoft Visual C/C++ Runtime is 'forward binary compatible'. This means you can build code with VS 2015 Update 3, VS 2017, VS 2019, and/or VS 2022 and link it all together. The key requirement is that the LINK must be done against the *newest* toolset in the mix. See [Microsoft Learn](/cpp/porting/binary-compat-2015-2017).

## Maintainer notes

CMake projects for these triplets are built using `CMAKE_SYSTEM_NAME` set to "Windows".

## Library author notes

* "Just My Code" debugging can usually be disabled in a library to save code space.

```
if(MSVC)
    target_compile_options(mytarget PRIVATE /JMC-)
endif()
```

* MSBuild will automatically add some build flags that are not on-by-default in the MSVC compiler itself. To ensure the same behavior with Ninja or other generators, add these build settings.

```
if(MSVC)
    target_compile_options(mytarget PRIVATE /Zc:inline)
endif()
```

* Recommended build settings for newer versions of Visual C++ are encouraged for improved code security.

```
if(MSVC)
    target_compile_options(mytarget PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:cf>")
    target_link_options(mytarget PRIVATE /DYNAMICBASE /NXCOMPAT)

    if((CMAKE_SIZEOF_VOID_P EQUAL 4)
       AND (NOT (${VCPKG_TARGET_ARCHITECTURE} MATCHES "^arm")))
      target_link_options(mytarget PRIVATE /SAFESEH)
    endif()

    if((MSVC_VERSION GREATER_EQUAL 1928)
       AND (CMAKE_SIZEOF_VOID_P EQUAL 8)
       AND ((NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
            OR (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13.0)))
      target_compile_options(mytarget PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
      target_link_options(mytarget PRIVATE "$<$<NOT:$<CONFIG:DEBUG>>:/guard:ehcont>")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(mytarget PRIVATE /sdl)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.24)
        target_compile_options(mytarget PRIVATE /ZH:SHA_256)
    endif()

    if((CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.27)
       AND (NOT (${VCPKG_TARGET_ARCHITECTURE} MATCHES "^arm")))
        target_link_options(mytarget PRIVATE /CETCOMPAT)
    endif()
endif()
```

* For improved Standard C/C++ Conformance, use the latest switch settings.

```
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(mytarget PRIVATE /permissive- /Zc:__cplusplus /Zc:inline)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.26)
        target_compile_options(mytarget PRIVATE /Zc:preprocessor)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.28)
        target_compile_options(mytarget PRIVATE /Zc:lambda)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.35)
        target_compile_options(mytarget PRIVATE /Zc:templateScope)
    endif()
endif()
```

* To support the use of Whole Program Optimization / Link-Time Code Generation, recommended build settings are as follows:

```
if((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") AND CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    target_compile_options(${PROJECT_NAME} PRIVATE /Gy /Gw)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.35)
        target_compile_options(mytarget PRIVATE /Zc:checkGwOdr)
    endif()
endif()
```

* If enabling Spectre migations, use the following guards.

```
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if((MSVC_VERSION GREATER_EQUAL 1913) AND (NOT WINDOWS_STORE))
      target_compile_options(mytarget PRIVATE "/Qspectre")
    endif()
endif()
```

Note you may want to also provide an explicit CMake build option to control this as well.

See [Microsoft Learn](/cpp/build/reference/qspectre).
