---
title: clang/LLVM for Windows 
description: Use arm64 and x64 community triplets for clang/LLVM
ms.date: 04/27/2023
---
# clang/LLVM for Windows

> [!NOTE]
> Clang/LLVM is not tested as part of vcpkg repository's CI process, so regressions can occur as part of library updates. PRs improving support are welcome!

## Clang community triplets

vcpkg includes [community triplets](https://github.com/microsoft/vcpkg/tree/master/triplets/community) for building with clang/LLVM for Windows using the ``clang-cl.exe`` MSVC-like command-line driver.

There are two variants of each triplet, selecting between static and dynamic linking.

| Architecture | vcpkg community triplets   | Target parameter                   |
|--------------|----------------------------|------------------------------------|
| x64          | x64-clangcl-dynamic        | ``-m64``                           |
|              | x64-clangcl-static         |                                    |
|              | x64-clangcl-uwp            |                                    |
| arm64        | arm64-clangcl-dynamic      | ``--target=arm64-pc-windows-msvc`` |
|              | arm64-clangcl-static       |                                    |
|              | arm64-clangcl-uwp          |                                    |

In addition to triplets for the standard Windows and UWP platforms using clang/LLVM, there are four that target the Xbox platform as well.

| Console          | vcpkg community triplets     |
|------------------|------------------------------|
| Xbox One         | x64-clangcl-xboxone          |
|                  | x64-clangcl-xboxone-static   |
| Xbox Series X\|S | x64-clangcl-scarlett         |
|                  | x64-clangcl-scarlett-static  |

The ``static`` linking triplets are set to use the MSVC Runtime as a DLL (i.e. ``VCPKG_CRT_LINKAGE dynamic``) as that's the recommended usage scenario.

> The toolchain file can also support x86 and arm architectures, if needed, but those triplet files are left as an excercise to the reader to implement.

## Visual Studio integration of clang/LLVM for Windows

The easiest way to make use of clang/LLVM for Windows is to install it as part of the Visual Studio product. You can install them by opening the Visual Studio Installer and choosing **C++ Clang tools for Windows** under **Desktop development with C++** optional components (``Microsoft.VisualStudio.Component.VC.Llvm.Clang`` and/or ``Microsoft.VisualStudio.ComponentGroup.NativeDesktop.Llvm.Clang``). 

![Visual Studio 2022 Install UI](/cpp/build/media/clang-install-vs2022.png)

The vcpkg clangcl triplets will automatically make use of these tools by default.

## Selecting a clang/LLVM for Windows Toolset

If you wish to select a different clang/LLVM for Windows toolset, you can install it and then set the **LLVMInstallDir** environment variable to point to the toolset location.

```
set LLVMInstallDir="C:\Program Files\LLVM"
```

Note that MSBuild uses the same property variable to set a custom version of clang/LLVM as well. Be sure to also set the **LLVMToolsVersion** property variable to the correct value as well. See [Microsoft Learn](/cpp/build/clang-support-msbuild#custom_llvm_location).

The vcpkg triplets will make use of the toolset pointed to by this variable if present.

## C/C++ Runtime

The clang/LLVM for Windows toolset makes use of the Microsoft Visual C/C++ Runtime, so is highly compatible with the MSVC compiler. It will use the version of the CRT that matches the Visual Studio installation. This also ensures that code built with clang/LLVM for Windows and the MSVC compiler are link-compatible.

Keep in mind that the Microsoft Visual C/C++ Runtime is 'forward binary compatible'. This means you can build code with VS 2015 Update 3, VS 2017, VS 2019, and/or VS 2022 and link it all together. The key requirement is that the LINK must be done against the *newest* toolset in the mix. See [Microsoft Learn](/cpp/porting/binary-compat-2015-2017).

## Known Issues

* Starting with the Windows SDK (22621) or later, platform libraries will generate linker warnings if using clang/LLVM for Windows v14 or earlier. While this warning is harmless, it cannot be suppressed for older toolsets.

```
lld-link: warning/error: ignoring unknown debug$S subsection kind 0xFF in file libucrt.lib
```

## Maintainer notes

The ``clang-cl.exe`` MSVC-like front-end to clang/LLVM for Windows is designed to handle most existing MSVC ``cl.exe`` command-line options.

* In CMake the ``MSVC`` and ``WIN32`` variables will be set for both cl.exe and clang-cl.exe.

* ``/Qspectre`` and ``/analyze`` are not supported by the clang-cl.exe front-end. Only add one of these flags for ``CMAKE_CXX_COMPILER_ID`` being set to "MSVC".

* clang-cl.exe does not support the C++/CX language extensions (``/ZW``, ``/AI``, ``/FU``, ``/WINMD``, and ``#using``) supported by cl.exe. You can make use of [C++/WinRT language projections](/windows/uwp/cpp-and-winrt-apis/) with both MSVC and clang/LLVM for Windows with `set(CMAKE_CXX_STANDARD 17)`

* clang-cl.exe v12 added support for ``/guard:cf``. clang-cl.exe v13 added support for ``/guard:ehcont``.
