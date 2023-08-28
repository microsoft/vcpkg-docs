---
title: Universal Windows Platform
description: Building for UWP with the Microsoft Visual C++ Compiler
ms.date: 04/28/2023
---
# Universal Windows Platform (UWP)

## Triplets

vcpkg includes [triplets](https://github.com/microsoft/vcpkg/tree/master/triplets) for building UWP applications using the MSVC ``cl.exe`` compiler.

| Architecture | vcpkg triplets      | Community |
|--------------|---------------------|-----------|
| x64          | x64-uwp             |           |
|              | x64-uwp-static-md   | Yes       |
| x86          | x86-uwp             | Yes       |
|              | x86-uwp-static-md   | Yes       |
| arm          | arm-uwp             |           |
|              | arm-uwp-static-md   | Yes       |
| arm64        | arm64-uwp           | Yes       |
|              | arm64-uwp-static-md | Yes       |

## C++/CX vs. C++/WinRT projections

UWP applications typically consume Windows Runtime APIs, and there are a number of solutions for using these from C++. The [C++/CX language extensions](/cpp/cppcx/visual-c-language-reference-c-cx) for the MSVC compiler (``/ZW``), the [C++/WinRT language projections](/windows/uwp/cpp-and-winrt-apis/) which works with C++17 compilers, or the [Windows Runtime Library](/cpp/cppcx/wrl/windows-runtime-cpp-template-library-wrl).

The UWP triplet toolchain leaves enabling C++/CX (``/ZW``) up to CMake, but does provide a ``/FU`` parameter to point to the proper ``platform.winmd`` file for the toolset being used.

## Maintainer notes

CMake projects for these triplets are built using `CMAKE_SYSTEM_NAME` set to "WindowsStore" and `CMAKE_SYSTEM_VERSION` set to "10.0"

For CMake 3.1 or later, you control the enabling of the MSVC C++/CX language extensions via the `VS_WINRT_COMPONENT` property for the *Visual Studio generator*.

The UWP triplets also build code using ``/DWINAPI_FAMILY=WINAPI_FAMILY_APP`` for the API partition, so libraries can fail to build if they are using unsupported versions of Win32 APIs. The general recommendation is to use the newer APIs in all cases, but if you need to build the same library for older versions of the Windows OS then you may need to use conditionally building code such as the following to support both scenarios.

```
    HANDLE hFile = nullptr;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
    hFile = CreateFile2(
                fileName,
                GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING,
                nullptr);
#else
    hFile = CreateFileW(
                fileName,
                GENERIC_READ, FILE_SHARE_READ,
                nullptr,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                nullptr);
#endif
```

UWP triplets also build with ``/DUNICODE /D_UNICODE`` as these are both strongly recommended for modern development. See the [UTF-8 Everywhere manifesto](https://utf8everywhere.org/) for more information.

## Library author notes

If using CMake for your library, consider using something similiar to the following:

```
target_compile_definitions(${PROJECT_NAME} PRIVATE _UNICODE UNICODE)

if(WINDOWS_STORE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WINAPI_FAMILY=WINAPI_FAMILY_APP)
endif()
```

You should set `` _WIN32_WINNT=0x0A00`` as well for either all ``WIN32`` platforms, or at least  for ``WINDOWS_STORE`` platform builds.

## Further reading

[Get started with Windows apps](/windows/uwp/get-started/)

[UWP on Xbox One](/windows/uwp/xbox-apps/)
