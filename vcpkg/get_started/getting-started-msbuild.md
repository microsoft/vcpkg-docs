# Getting started with vcpkg in Visual Studio with MSBuild

This guide sets up a simple C++ project using the library `zlib` with the MSBuild project build system.

## Prerequisites:
- Visual Studio 2015 Update 3 or later with the C++ Desktop Development workload
- Windows 7 or newer
- Git

## Install `vcpkg`

In a command prompt, run:
```
git clone https://github.com/microsoft/vcpkg
cd vcpkg && .\bootstrap.bat
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

The [manifest](./manifest.md) will contain the set of C++ dependencies from vcpkg. In `Solution Explorer - Folder View`, right click to  **Add > New File > Type in "vcpkg.json"** to create a new manifest in the root of your project

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

The builtin-baseline sets the global minimum version information for your manifest. While we can set it to any git commit hash in the vcpkg project, we recommend setting it to the latest commit. In the terminal where you acquired vcpkg, you can run `git rev-parse HEAD` or simply copy the latest commit from `git log`. For more information, see the [reference](../reference/vcpkg-json.md) on `vcpkg.json`. 

> [!NOTE]
> [Classic mode](./classic-mode.md) users need to install dependencies directly in their terminal, i.e. `vcpkg install zlib`. In a post-build step, any dynamic libraries will be copied to the application folder, so the program can work correctly.

## Enable manifest mode in Visual Studio

Navigate to the Project Properties pages of your project. Under **Configuration Properties > vcpkg**, set `Use vcpkg manifest` to `Yes`. MSBuild will check if this property is set before installing any dependencies from the vcpkg manifest.

Other settings, such as [triplets](./triplet.md), are filled in with default values vcpkg detects from your project and will be useful when configuring your project. 

## Use the zlib library

In your `main.cpp`, add:
```cpp
#include <zlib.h>
#include <iostream>

int main()
{
    std::cout << zlibVersion();
}
```
> [!NOTE]
> There will be error squiggles in your project when MSBuilds it for the first time. Build the project to acquire vcpkg dependencies to remove them.

## Build and Run

If MSBuild detects a vcpkg manifest and it is enabled, MSBuild will install the manifest's C++ dependencies as a pre-build step. Relevant artifacts are installed in a folder `vcpkg_installed` in the build directory of your project. Any headers installed by the library can be directly used, and any libraries installed will be automatically linked.

Building and running for the default configuration and project platform (Debug, x64):

```
1>------ Build started: Project: ConsoleApplication1, Configuration: Debug x64 ------
1>Installing vcpkg dependencies to C:\Users\<user>\source\repos\ConsoleApplication1\vcpkg_installed\x64-windows\
1>"C:\Users\<user>\workspace\vcpkg\vcpkg.exe" install  --x-wait-for-lock --triplet "x64-windows" --vcpkg-root "C:\Users\<user>\workspace\vcpkg\\" "--x-manifest-root=C:\Users\<user>\source\repos\ConsoleApplication1\\" "--x-install-root=C:\Users\<user>\source\repos\ConsoleApplication1\vcpkg_installed\x64-windows\\"
1>Detecting compiler hash for triplet x64-windows...
1>The following packages will be built and installed:
1>  * vcpkg-cmake:x64-windows -> 2023-05-04 -- C:\Users\<user>\workspace\vcpkg\buildtrees\versioning_\versions\vcpkg-cmake\88a7058fc7fa73a9c4c99cfcae9d79e2abf87a5a
1>    zlib:x64-windows -> 1.3 -- C:\Users\<user>\workspace\vcpkg\buildtrees\versioning_\versions\zlib\5ac18c6e6e3e2bf5a9e3d0bc8a845f198e4c4e05
1>Additional packages (*) will be modified to complete this operation.
1>Restored 2 package(s) from C:\Users\<user>\AppData\Local\vcpkg\archives in 157 ms. Use --debug to see more details.
1>Installing 1/2 vcpkg-cmake:x64-windows...
1>Elapsed time to handle vcpkg-cmake:x64-windows: 14.6 ms
1>Installing 2/2 zlib:x64-windows...
1>Elapsed time to handle zlib:x64-windows: 42.3 ms
1>Total install time: 57 ms
1>The package zlib is compatible with built-in CMake targets:
1>
1>    find_package(ZLIB REQUIRED)
1>    target_link_libraries(main PRIVATE ZLIB::ZLIB)
1>
1>ConsoleApplication1.cpp
1>ConsoleApplication1.vcxproj -> C:\Users\<user>\source\repos\ConsoleApplication1\x64\Debug\ConsoleApplication1.exe
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
========== Build started at 11:31 AM and took 14.614 seconds ==========
```

> [!NOTE]
> You might notice that `vcpkg install` builds both Debug and Release configurations for a library. This will make it easier to switch between configurations when developing with Visual Studio. To only build release libraries, add `VCPKG_RELEASE_ONLY` to your triplet.

Running the program yields `1.3` which is the expected version for `zlib`.

## Folder View of the project

Navigate to the Folder View in Solution Explorer. vcpkg creates a new directory `vcpkg_installed` which contains all relevant build artifacts libraries, headers, debugging information. Additionally, required libraries for running and deployment (such as `zlibd1.dll`) are copied next to the application binary.
