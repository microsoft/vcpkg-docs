---
title: "Tutorial: Install native dependencies in a CLR application"
description: Learn to use vcpkg in your C++/CLI libraries and applications.
author: vicroms
ms.author: viromer
ms.date: 9/2/2024
ms.topic: tutorial
#CustomerIntent: As a C++/CLI developer, I want to use vcpkg to install dependencies for my native libraries and code
---
# Tutorial: Install native dependencies in a CLR application

C++/CLI is a technology that lets you combine .NET classes with native C++ types
to create libraries and applications that take C++ code and make it accessible
to .NET programs.

You can use vcpkg in combination with C++/CLI to install and consume C++
dependencies in your projects targetting the [Common Language Runtime
(CLR)](/dotnet/standard/clr).

In this tutorial, you will learn how to:

> [!div class="checklist"]
> - [Build a sample C++/CLI project](#build-sample)
> - [Add a C++ native dependency](#add-dependency)
> - [Build the application](#build-application)

## Prerequisites

- vcpkg
- Git
- Developer Powershell for Visual Studio
- [Visual Studio](<https://visualstudio.microsoft.com/downloads/>) with the
  following components:
  - [C++ development
    workload](/cpp/build/vscpp-step-0-installation)
  - [C++/CLI support
    component](/cpp/dotnet/dotnet-programming-with-cpp-cli-visual-cpp#install-ccli-support-in-visual-studio-2022)

## <a name="build-sample"></a>Build a sample C++/CLI

In this tutorial, we will start from an existing C++/CLI application and add a
C++ dependeny installed with vcpkg. The commands in this tutorial assume that
you're running them in a [Developer PowerShell for Visual
Studio](/visualstudio/ide/reference/command-prompt-powershell#start-from-windows-menu).

1 - Clone the sample repository

The first step is to get a copy of the C++/CLI sample application in the [.NET
samples repository](<https://github.com/dotnet/samples>). The C++/CLI sample
application is located in the `core/interop/cpp-cli` folder.

```powershell
git clone https://github.com/dotnet/samples
```

2 - Navigate to the sample application folder

```powershell
cd samples/core/interpo/cpp-cli
```

2 - Check that the project builds and runs correctly 

The sample repository contains a solution with four projects:
- ManagedLibrary: A C# library for .NET
- MixedLibrary: A library mixing native C++ code and .NET code from ManagedLibrary
- NativeApp: A C++ application that consumes .NET code from MixedLibrary
- ManagedApp: A C# application that consumes C++ code from MixedLibrary

Run the following command to build the solution's projects:

```powershell
msbuild CPP-CLI.sln -restore
```

If the build fails make sure that you have installed the required components for
Visual Studio listed in the [Prerequisites](#prerequisites) section, and that
you meet the minimum requirements of [.NET 5.0
SDK](https://dotnet.microsoft.com/download) or later and [Visual Studio 2019
16.8](https://visualstudio.microsoft.com/downloads/) or later] of the [sample
application](https://github.com/dotnet/samples/tree/main/core/interop/cpp-cli).

Once built, you can run `ManagedApp.exe`

```powershell
./bin/Debug/x64/ManagedApp.exe
```

The program produces the following output:

```
=== Managed class ===
Hello from ManagedClass in MixedLibrary
Hello from NativeClass in MixedLibrary
-- message: from managed app!

=== P/Invoke ===
Hello from NativeEntryPoint_CallNative in MixedLibrary
Hello from NativeClass in MixedLibrary
-- message: from managed app!
```

3 - Open the project in Visual Studio

In the next steps we will modify the library to use `fmt` to print messages to
the console. The `fmt` library will be installed via vcpkg and linked in the
consuming projects.

To edit the source files, open the CPP-CLI.sln solution in Visual Studio:

```
start CPP-CLI.sln
```

Once the solution has been opened in Visual Studio you may get a prompt to
retarget the project to the latest versions. You can click OK to upgrade the
Windows SDK Version and Platform Toolset to latest.

:::image type="complex" source="../resources/clr-retarget-project.png" alt-text="Retarget existing project":::
    Screenshot of a Visual Studio window prompting the user to upgrade a
    project's target Windows SDK Version and Platform Toolset version to the latest version.
:::image-end:::

## <a name="add-dependency"></a>Add a C++ native dependency

Next, we will be making the following changes to the MixedLibrary project.

- Add a vcpkg manifest to acquire `fmt`.
- Enable vcpkg in the project.
- Modify the `NativeClass::Hello` to use `fmt` to print messages.

1 - Create a vcpkg manifest

Right-click on the NativeLibrary project and click on Add > New Item in the
context menu.

Name the new item `vcpkg.json`, this is the vcpkg manifest file, and make sure
that the file is created in the root of the project's folder.

2 - Add `fmt` as a depedency

Open the `vcpkg.json` file and edit its content to match the following:

```json
{
  "dependencies": [ "fmt" ]
}
```

If the vcpkg manifest file is located in the right location and you try to build
the project, you will get the following warning:

```
The vcpkg manifest was disabled, but we found a manifest file in samples\core\interop\cpp-cli\MixedLibrary\. You may want to enable vcpkg manifests in your properties page or pass /p:VcpkgEnableManifest=true to the msbuild invocation.
```

3 - Enable vcpkg in MixedLibrary's properties

Open the MixedLibrary's properties page by right-clicking on the project and
clicking the Properties option.

Change the following properties in the vcpkg section:

- **Use Vcpkg** set to **Yes**
- **Use Vcpkg Manifest** set to **Yes**
- **Instal Vcpkg Dependencies** set to **Yes**
- **Use AutoLink** set to **Yes**
- **App-locally deploy DLLs** set to **Yes**

:::image type="complex" source="../resources/mixedlibrary-properties.png" alt-text="MixedLibrary project properties":::
    Screenshot of the MixedLibrary project's Properties page showing the options
    required to enable vcpkg
:::image-end:::

With these changes Visual Studio will now read the `vcpkg.json` file and
automatcally install the dependencies contained in the manifest before building
the project.

While in the Properties page, we also want to enable the `/utf-8` flag to make
`fmt` build correctly. 

In the Command Line subsection of the C/C++ settings edit the Additional Options
to include `/utf-8` in the 

Finally, click OK to close the Properties page.

4 - Verify vcpkg is working

If everything is correctly configured, Visual Studio will invoke vcpkg to
install your dependencies before building the MixedLibrary project.

```
1>Installing vcpkg dependencies to  C:\path\to\samples\core\interop\cpp-cli\MixedLibrary\vcpkg_installed\x64-windows\
1>"C:\path\to\vcpkg\vcpkg.exe" install --x-wait-for-lock --triplet "x64-windows" --vcpkg-root "C:\path\to\vcpkg\" "--x-manifest-root=C:\path\to\samples\core\interop\cpp-cli\MixedLibrary\" "--x-install-root=C:\path\to\samples\core\interop\cpp-cli\MixedLibrary\vcpkg_installed\x64-windows\"
```

If you don't see vcpkg output, or if `fmt` fails to build, make sure that you
followed the steps above correctly, including adding `/utf-8` in the Additional
Options in C/C++ > Command Line.

4 - Modify the project's source code

Finally, we want to modify the `MixedLibrary.cpp` file to use `fmt` to print
messages to the console. Make the following changes to the source code:

  1 - Include the `fmt/printf.h` header (on line 5).

  ```C++
  #include <iostream>
  #include <vcclr.h>
  #include <fmt/printf.h>
  ```

  2 - Modify the `NativeClass::Hello` function to use `fmt::println` (on line 44).

  ```C++
  void MixedLibrary::NativeClass::Hello(const wchar_t *msg)
  {
      auto ws = std::wstring(msg);
      auto str = std::string(ws.length(), 0);
      std::transform(ws.begin(), ws.end(), std::back_inserter(str), [](wchar_t c) { return static_cast<char>(c); });
      fmt::println("Hello from NativeClass in MixedLibrary");
      fmt::println("-- message: {}", str);
      fmt::println("-- printed using FMT version {}", FMT_VERSION);
  }
  ```

## <a name="build-application"></a>Build the application

The `NativeClass::Hello` function is used in the ManagedApp project to print a
message to the console using C++ code. The changes above make it so that the
`fmt` library is consumed in the CLR application.

No changs to the application's project are needed, just build and run the
ManagedApp project. 

The output of the program should look similar to this:

```
=== Managed class ===
Hello from ManagedClass in MixedLibrary
Hello from NativeClass in MixedLibrary
-- message: from managed app!
-- printed using FMT version 110002

=== P/Invoke ===
Hello from NativeEntryPoint_CallNative in MixedLibrary
Hello from NativeClass in MixedLibrary
-- message: from managed app!
-- printed using FMT version 110002
```

## Next steps

<!--The steps to get vcpkg dependencies in your C++/CLI projects targetting the CLR
are not too different from other types of C++ projects. However, there are some
considerations to make when mixing dependencies from NuGet and vcpkg. See the
[vcpkg in C++/CLI projects](../concepts/vcpkg-in-clr-projects.md) article to
learn more.-->

Also learn about other useful features:

- [Manifest mode](../concepts/manifest-mode.md)
- [`vcpkg export`](../commands/export.md#nuget)
- [Versioning](../users/versioning.md)
