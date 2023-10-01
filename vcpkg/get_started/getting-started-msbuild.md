# Getting started with vcpkg in Visual Studio with MSBuild

This guide sets up a simple C++ project using the library `zlib` with the MSBuild project build system.

## Prerequisites:
- Visual Studio 2015 Update 3 or later with the English Language Pack
- Windows 7 or newer
- Git

## Install `vcpkg`

In a command prompt, run:
```
git clone https://github.com/microsoft/vcpkg
.\bootstrap.bat
.\vcpkg.exe integrate install
```
Outputs:
```
All MSBuild C++ projects can now #include any installed libraries. Linking will be handled automatically. Installing new libraries will make them instantly available.
```

The `bootstrap.bat` script fetches a copy of `vcpkg.exe` that we will use to install dependencies. The `integrate install` command sets the user wide vcpkg instance in `%APPDATA%\Local\vcpkg` so MSBuild will be able to find it.

> [!NOTE]
> The C++ Desktop Development workload for Visual Studio 17.6 and later comes with a copy of vcpkg, which is intended for experimental use. While you will be able to use this directly in the Visual Studio Developer Command Prompt, we recommend users to obtain a seperate copy of vcpkg instead.

## Create a new project in Visual Studio

Create a new console application project, **File > New Project > Console Application (C++ for Windows)**. This prints "Hello World" by default.

## Add a vcpkg manifest to your project

The manifest will contain the set of C++ dependencies you will use from vcpkg. In the Solution Explorer, under your project, navigate to **Source Files**, and right click to **Add > New Item > Type in "vcpkg.json"**

```json
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
  "name": "my-application",
  "builtin-baseline": "78ba9711d30c64a6b40462c72f356c681e2255f3",
  "version": "0.15.2",
  "dependencies": [
    "zlib"
   ]
}
```

The builtin-baseline sets the global minimum version information for your manifest. While we can set it to any git commit hash in the vcpkg project, we recommend setting it to the latest commit. In the terminal where you acquired vcpkg, you can run `git rev-parse HEAD` or simply copy the latest commit from `git log`. For more information, see the reference on `vcpkg.json`. 

> [!NOTE]
> Classic mode users will need to install dependencies directly in their terminal, i.e. `vcpkg install zlib`. In a post-build step, any dynamic libraries will be copied to the application folder, so the program can work correctly.

## Enable manifest mode in Visual Studio

Navigate to **Console Application > Properties**. Under **Configuration Properties > vcpkg**, set `Use vcpkg manifest` to `Yes`. MSBuild will check if this property is set before installing any dependencies from the vcpkg manifest.

A triplet in vcpkg is the set of configuration properties for your libraries. The default x64-windows triplet builds dynamic libraries. The default triplet is your host triplet?, but it can be set to something else.

For a reference on other properties, see reference.

> [!NOTE]
> If you are using the command line toolset, Visual Studio Build Tools, make sure to pass `/p:UseVcpkgManifest=True???` in your msbuild invocation. Or add a property group to your MSBuild project file.

## Use the zlib library

Use the following for you `main.cpp`
```cpp
#include <zlib.h>
#include <iostream>

int main()
{
    std::cout << zlibVersion();
}
```

## Build and Run

If MSBuild detects a vcpkg manifest and it is enabled, it will install the manifest's C++ dependencies as a pre-build step. Any headers can be directly used, and any libraries installed will be automatically linked.

You can build and run for the default configuration and project platform (Debug, x64):

```
Build started...
1>------ Build started: Project: ConsoleApplication1, Configuration: Debug x64 ------
1>Installing vcpkg dependencies to C:\Users\danshaw2\source\repos\ConsoleApplication1\ConsoleApplication1\vcpkg_installed\x64-windows\
1>"C:\Users\danshaw2\workspace\vcpkg\vcpkg.exe" install  --x-wait-for-lock --triplet "x64-windows" --vcpkg-root "C:\Users\danshaw2\workspace\vcpkg\\" "--x-manifest-root=C:\Users\danshaw2\source\repos\ConsoleApplication1\ConsoleApplication1\\" "--x-install-root=C:\Users\danshaw2\source\repos\ConsoleApplication1\ConsoleApplication1\vcpkg_installed\x64-windows\\"
1>Detecting compiler hash for triplet x64-windows...
1>The following packages will be built and installed:
1>    rapidjson:x64-windows -> 2023-04-27
1>  * vcpkg-cmake:x64-windows -> 2023-05-04
1>  * vcpkg-cmake-config:x64-windows -> 2022-02-06#1
1>    zlib:x64-windows -> 1.3
1>Additional packages (*) will be modified to complete this operation.
1>Restored 3 package(s) from C:\Users\danshaw2\AppData\Local\vcpkg\archives in 160 ms. Use --debug to see more details.
1>Installing 1/4 vcpkg-cmake-config:x64-windows...
1>Elapsed time to handle vcpkg-cmake-config:x64-windows: 12.5 ms
1>Installing 2/4 vcpkg-cmake:x64-windows...
1>Elapsed time to handle vcpkg-cmake:x64-windows: 17.8 ms
1>Installing 3/4 rapidjson:x64-windows...
1>Building rapidjson:x64-windows...
1>-- Downloading https://github.com/Tencent/rapidjson/archive/949c771b03de448bdedea80c44a4a5f65284bfeb.tar.gz -> Tencent-rapidjson-949c771b03de448bdedea80c44a4a5f65284bfeb-2.tar.gz...
1>-- Extracting source C:/Users/danshaw2/workspace/vcpkg/downloads/Tencent-rapidjson-949c771b03de448bdedea80c44a4a5f65284bfeb-2.tar.gz
1>-- Using source at C:/Users/danshaw2/workspace/vcpkg/buildtrees/rapidjson/src/f65284bfeb-9c9325ad06.clean
1>-- Found external ninja('1.11.0').
1>-- Configuring x64-windows
1>-- Building x64-windows-dbg
1>-- Building x64-windows-rel
1>-- Using cached mingw-w64-i686-pkgconf-1~1.8.0-2-any.pkg.tar.zst.
1>-- Using cached msys2-msys2-runtime-3.4.6-1-x86_64.pkg.tar.zst.
1>-- Using msys root at C:/Users/danshaw2/workspace/vcpkg/downloads/tools/msys2/6f3fa1a12ef85a6f
1>-- Installing: C:/Users/danshaw2/workspace/vcpkg/packages/rapidjson_x64-windows/share/rapidjson/copyright
1>-- Performing post-build validation
1>Stored binaries in 1 destinations in 86 ms.
1>Elapsed time to handle rapidjson:x64-windows: 5.4 s
1>Installing 4/4 zlib:x64-windows...
1>Elapsed time to handle zlib:x64-windows: 21.8 ms
1>Total install time: 5.4 s
1>rapidjson provides CMake targets:
1>
1>  # this is heuristically generated, and may not be correct
1>  find_package(RapidJSON CONFIG REQUIRED)
1>  target_link_libraries(main PRIVATE rapidjson)
1>
1>The package zlib is compatible with built-in CMake targets:
1>
1>    find_package(ZLIB REQUIRED)
1>    target_link_libraries(main PRIVATE ZLIB::ZLIB)
1>
1>ConsoleApplication1.cpp
1>ConsoleApplication1.vcxproj -> C:\Users\danshaw2\source\repos\ConsoleApplication1\x64\Debug\ConsoleApplication1.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build started at 3:04 PM and took 16.921 seconds ==========
```

> [!NOTE]
> You might notice that `vcpkg install` builds both Debug and Release configurations for a library. This will make it easier to switch between configurations. To build release only, set `VCPKG_RELEASE_ONLY` in your triplet.

Running the program yields `1.3` which is the expected version for `zlib`.

## Folder View of the project

Navigate to the Folder View in Solution Explorer. vcpkg creates a new directory `vcpkg_installed` which contains all relevant build artifacts libraries, headers, debugging information. Required libraries for running and deployment (such as `zlibd1.dll`) are copied next to the application binary.
