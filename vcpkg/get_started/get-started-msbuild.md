---
title: Install and use packages with MSBuild in Visual Studio
description: Tutorial guides the user through the process of installing and using packages with MSBuild and Visual Studio.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
author: data-queue
ms.author: danshaw2
ms.date: 10/16/2023
ms.prod: vcpkg
ms.topic: tutorial
---

# Tutorial: Install and use packages with MSBuild in Visual Studio

This tutorial shows you how to create a C++ "Hello World" program that uses the `fmt` library with MSBuild, vcpkg and Visual Studio. You'll install dependencies, configure, build, and run a simple application.

## Prerequisites:
- [Visual Studio](<https://visualstudio.microsoft.com/downloads/>) with C++ development workload
- [Git](<https://git-scm.com/downloads>)
- Windows 7 or newer

## 1 - Set up vcpkg

[!INCLUDE [setup-vcpkg](includes/setup-vcpkg.md)]

3. Integrate with Visual Studio MSBuild

The next step is to set the user-wide instance of vcpkg so that MSBuild will be able to find it:
```
.\vcpkg.exe integrate install
```

This outputs:
```
All MSBuild C++ projects can now #include any installed libraries. Linking will be handled automatically. Installing new libraries will make them instantly available.
```

## 2 - Set up the Visual Studio project

1. Create the Visual Studio project
    
    * Create a new project in Visual Studio using the "Console Application" template
    :::image type="complex" source="../resources/get_started/visual-studio-create-project-msbuild.png" alt-text="create a new C++ Windows console application":::
        Screenshot of the Visual Studio UI for showing how to create a new C++ Windows console application in Visual Studio
    :::image-end:::
    * Name your project "helloworld"
    * Check the box for "Place solution and project in the same directory."
    * Click the "Create" button
    :::image type="complex" source="../resources/get_started/visual-studio-name-project-msbuild.png" alt-text="naming your MSBuild C++ project":::
        Screenshot of Visual Studio UI for naming your MSBuild C++ project and clicking the "create" button.
    :::image-end:::

2. Configure the `VCPKG_ROOT` environment variable.
   
    ::: zone pivot="shell-powershell"
    Open the built-in Developer PowerShell window in Visual Studio.
    
    :::image type="complex" source="../resources/get_started/visual-studio-developer-powershell.png" alt-text="opening built-in developer powershell":::
        Screenshot of Visual Studio UI for the built-in PowerShell developer window
    :::image-end:::

    Run the following commands:

    ```PowerShell
    $env:VCPKG_ROOT = "C:\path\to\vcpkg"
    $env:PATH = "$env:VCPKG_ROOT;$env:PATH"
    ```

    :::image type="complex" source="../resources/get_started/visual-studio-environment-variable-setup-powershell.png" alt-text="setting up your environment variables":::
        Screenshot of Visual Studio UI for the built-in PowerShell developer window showing how to set up VCPKG_ROOT and and add it to PATH.
    :::image-end:::
    ::: zone-end
    ::: zone pivot="shell-cmd"
    Open the Developer command prompt in Visual Studio.
    
    :::image type="complex" source="../resources/get_started/visual-studio-developer-cmd.png" alt-text="opening Visual Studio developer command prompt.":::
        Screenshot of Visual Studio UI for developer command prompt.
    :::image-end:::

    Run the following commands:

    ```console
    set VCPKG_ROOT="C:\path\to\vcpkg"
    set PATH=%VCPKG_ROOT%;%PATH%
    ```

    :::image type="complex" source="../resources/get_started/visual-studio-environment-variable-setup-cmd.png" alt-text="setting up your environment variables":::
        Screenshot of Visual Studio developer command prompt showing how to set up VCPKG_ROOT and and add it to PATH.
    :::image-end:::
    ::: zone-end

    Setting `VCPKG_ROOT` helps Visual Studio locate your vcpkg instance.
    Adding it to `PATH` ensures you can run vcpkg commands directly from the shell.

1. Generate a manifest file and add dependencies.

    Run the following command to create a vcpkg manifest file (`vcpkg.json`):

    ```console
    vcpkg new --application
    ```

    The [`vcpkg new`](../commands/new.md) command adds a `vcpkg.json` file and a `vcpkg-configuration.json` file in the project's directory.

    Add the `fmt` package as a dependency:

    ```console
    vcpkg add port fmt
    ```

    Your `vcpkg.json` should now contain:

    ```json
    {
        "dependencies": [
            "fmt"
        ]
    }
    ```

    This is your manifest file. vcpkg reads the manifest file to learn what dependencies to install and integrates with MSBuild to provide the dependencies required by your project.

    The generated `vcpkg-configuration.json` file introduces a [baseline](../reference/vcpkg-configuration-json.md#registry-baseline) that places [minimum version constraints](../users/versioning.md) on the project's dependencies. Modifying this file is beyond the scope of this tutorial. While not applicable in this tutorial, it's a good practice to keep the `vcpkg-configuration.json` file under source control to ensure version consistency across different development environments.

## 3 - Set up the project files

Modify the `helloworld.cpp` file.

Replace the content of `helloworld.cpp` with the following code:

:::code language="cpp" source="../examples/snippets/get-started/main.cpp":::

This source file includes the `<fmt/core.h>` header which is part of the `fmt` library. The `main()` function calls `fmt::print()` to output the "Hello World!" message to the console.

> [!NOTE]
> There will be error squiggles in your project when MSBuilds it for the first time. Build the project to acquire vcpkg dependencies to remove them.


## 4 - Enable manifest mode

Navigate to the Project Properties pages of your project. Under **Configuration Properties > vcpkg**, set `Use vcpkg manifest` to `Yes`. MSBuild checks whether this property is set before installing any dependencies from the vcpkg manifest.

:::image type="complex" source="../resources/get_started/visual-studio-manifest-msbuild.png" alt-text="Enable manifest mode in project properties":::
    Screenshot of enabling vcpkg manifest mode in Visual Studio Project Properties
:::image-end:::

Other settings, such as [triplets](..\users\triplets.md), are filled in with default values vcpkg detects from your project and will be useful when configuring your project. 

## 5 - Build and run the project

1. Build the project.
   
   Press `Ctrl+Shift+B` to build the project in Visual Studio and acquire the vcpkg dependencies.

If MSBuild detects a `vcpkg.json` file and manifests are enabled in your project, MSBuild installs the manifest's dependencies as a pre-build step. Dependencies are installed in a `vcpkg_installed` directory in the project's build output directory. Any headers installed by the library can be directly used, and any libraries installed will be automatically linked.

> [!NOTE]
> `vcpkg install` builds both Debug and Release configurations for a library. To only build release libraries, add `VCPKG_RELEASE_ONLY` to your triplet.

2. Run the application.

    Finally, run the executable:
    :::image type="complex" source="../resources/get_started/visual-studio-msbuild-project.png" alt-text="Running the executable":::
        Screenshot of Visual Studio UI for running the executable.
    :::image-end:::

    You should see the output:
    :::image type="complex" source="../resources/get_started/visual-studio-msbuild-output.png" alt-text="Program output":::
        Screenshot of the program outputs - "Hello World!"
    :::image-end:::

## Next steps

To learn more about `vcpkg.json` and vcpkg MSBuild integration, see our reference documentation:

- [vcpkg.json](..\reference\vcpkg-json.md)
- [manifest](..\users\manifests.md)
- [vcpkg in MSBuild projects](..\users\buildsystems\msbuild-integration.md)
