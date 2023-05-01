---
title: Microsoft Visual C++ for Windows
description: Building for Windows with the Microsoft Visual C++ Compiler
ms.date: 04/28/2023
---
# Microsoft Visual C++

## Triplets

vcpkg includes [triplets](https://github.com/microsoft/vcpkg/tree/master/triplets) for building Windows desktop applications using the MSVC ``cl.exe`` compiler.

| Architecture | vcpkg triplets     |
|--------------|--------------------|
| x64          | x64-windows        |
|              | x64-windows-static |
| x86          | x86-windows        |
| arm64        | arm64-windows      |

There are also a number of [community triplets](https://github.com/microsoft/vcpkg/tree/master/triplets/community) for the MSVC compiler toolset as well.

| Architecture | vcpkg community triplets |
|--------------|--------------------------|
| x64          | x64-windows-static-md    |
| x86          | x86-windows-static       |
|              | x86-windows-static-md    |
| arm          | arm-windows              |
|              | arm-windows-static       |
| arm64        | arm64-windows-static     |
|              | arm64-windows-static-md  |
|              | arm64ec-windows          |

> [!NOTE]
> Community triplets are not tested as part of vcpkg repository's CI process, so regressions can occur as part of library updates. PRs improving support are welcome!

The ``static`` linking triplets are set to use the MSVC Runtime as a static library (i.e. ``VCPKG_CRT_LINKAGE static``).

The ``static-md`` linking triplets are set to use the MSVC Runtime as a DLL (i.e. ``VCPKG_CRT_LINKAGE dynamic``). This is the recommended solution for redistributing the MSVC Runtime per [Microsoft Learn](/cpp/windows/deployment-in-visual-cpp).

## Selecting a Visual C++ Toolset

By default, vcpkg will use the latest version of Visual Studio installed on the system for building code. To select a specific version, create a custom triplet or triplet overlay to set it.

For examples, this would force the use of the VS 2017 toolset.

```
set(VCPKG_PLATFORM_TOOLSET v141)
```

See [Triplet files](../triplets.md) for more information.

## C/C++ Runtime

Keep in mind that the Microsoft Visual C/C++ Runtime is 'forward binary compatible'. This means you can build code with VS 2015 Update 3, VS 2017, VS 2019, and/or VS 2022 and link it all together. The key requirement is that the LINK must be done against the *newest* toolset in the mix. See [Microsoft Learn](/cpp/porting/binary-compat-2015-2017).

## Known issues

* If you get the following link failure, it is because you are building x64 native code with VS 2019 or later, but linking it with the VS 2017 toolset & CRT combination. Per the notes above, you should use the newest compiler toolset for your final link. This *specific* issue can be worked around as well with ``/d2FH4-`` (v142 or later) to use the older x64 C++ exception implementation.

```
error LNK2001: unresolved external symbol ___CxxFrameHandler4
```

See the [Visual C++ Team Blog](https://devblogs.microsoft.com/cppblog/making-cpp-exception-handling-smaller-x64/) for more information.

* Full C++20 support requires VS 2019 (16.11.14 or later) and/or VS 2022 (17.2 or later). See the [Visual C++ Team Blog](https://devblogs.microsoft.com/cppblog/msvc-cpp20-and-the-std-cpp20-switch/).

## Maintainer notes

CMake projects for these triplets are built using ``CMAKE_SYSTEM_NAME`` set to "Windows".

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
