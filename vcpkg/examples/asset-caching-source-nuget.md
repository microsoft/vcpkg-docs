---
title: Use a custom script as an asset caching source
description: Learn about how to create a x-script Asset Caching source for NuGet
ms.date: 01/10/2024
ms.topic: tutorial
---

# Use a custom script as an asset caching source

[!INCLUDE [experimental](../../includes/experimental.md)]

In this example, we'll set up a NuGet feed as an asset caching source by using a script to restore and push artifacts.

## Prerequisites

* nuget.exe
* A NuGet packages feed

## Step 1: Create `asset-source.nuspec`

Create a NuGet package spec template with the following contents:

```xml
<?xml version="1.0" encoding="utf-8"?>
<package xmlns="http://schemas.microsoft.com/packaging/2010/07/nuspec.xsd">
    <metadata>
        <id>$sha$</id>
        <version>1.0.0</version>
        <description>vcpkg download asset</description>
        <authors>vcpkg</authors>
    </metadata>
    <files>
        <file src="$file$" />
    </files>
</package>
```

## Step 2: Create an asset provider script

Now you need to create a script that downloads packages from the NuGet feed if available and uploads
missing packages to your feed if they are not.

Create `asset-provider.bat` with the contents provided below, make sure to replace the NuGet feed URL and path to `asset-source.nuspec` with their correct values on your system.

```bat
@echo off
set url=%1
set sha512=%2
set dst=%3
set "_dst=%dst:/=\%"
set "_sha512=%sha512:~0,90%"

cd /d %~dp3
%NUGET% install %sha512:~0,90% -Source https://your-nuget-feed-url
echo.
if exist %_sha512%.1.0.0 (
    echo "Pull from the NuGet feed"
    cd %_sha512%.1.0.0

    REM Assume both are files not directories
    echo "F" | xcopy /f *.part %_dst%
) else (
    echo "Fetch from the url"
    curl.exe -L %url% --create-dirs --output %dst%
    REM Replace with the correct path
    %NUGET% pack C:\path\to\asset-source.nuspec -BasePath %~dp3 -Properties "sha=%_sha512%;file=%dst%" -OutputDirectory %TEMP%
    %NUGET% push -ApiKey az -SkipDuplicate %TEMP%\%_sha512%.1.0.0.nupkg -Source https://your-nuget-feed-url
)
```

## Step 3: Configure the asset caching sources

Now that you have created the asset provider script, you need to instruct vcpkg to use it as an
asset caching source. To do that, set the following environment variables:

```powershell
$env:X_VCPKG_ASSET_SOURCES="clear;x-script,C:/path/to/asset-provider.bat {url} {sha512} {dst};x-block-origin"
$env:NUGET="C:/path/to/nuget.exe"
$env:VCPKG_KEEP_ENV_VARS="NUGET"
```

NOTE: Make sure to replace the placeholder paths to the asset provider script and nuget.exe with the correct paths in your system.

[`X_VCPKG_ASSET_SOURCES`](../users/config-environment.md#x_vcpkg_asset_sources) is the environment variable used to set asset caching sources for vcpkg to use. In this example we set the following values:

* `clear` gets rid of the default asset caching location.
* [`x-script`](../users/assetcaching.md#x-script) adds your script as an asset caching source, the first parameter indicates the command line vcpkg should invoke, in this example we call the `asset-provider.bat` script and forward some required parameters.
* `x-block-origin` forces all downloads to come from the configured asset caching sources.

[`VCPKG_KEEP_ENV_VARS`](../users/config-environment.md) is used to forward environment variables to
vcpkg's build environmet. During builds vcpkg creates a clean environment, by adding `NUGET` to
`VCPKG_KEEP_ENV_VARS` we ensure that the NuGet executable location is forwarded during builds.

Once all has been properly set up, any time that vcpkg downloads an asset it will upload it to your
NuGet feed to use in future downloads. You'll notice that cached assets are named after their file
SHA512 and the version specified in `asset-source.nuspec`. If you'd like to have more meaninful
names for your packages, you can modify the package template and asset provider script with your own logic.
