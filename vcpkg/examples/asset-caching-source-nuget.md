---
title: How to create a x-script Asset Caching source for NuGet
description: Learn about how to create a x-script Asset Caching source for NuGet
ms.date: 4/18/2023
---

# Creating a x-script Asset Caching source for NuGet

[!INCLUDE [experimental](../../includes/experimental.md)]

In this example, we will set up a NuGet feed as an asset caching source by using a script to restore and push artifacts.

## Prerequisites:

* nuget.exe
* A NuGet packages feed

## Step 1: Create the asset provider script

First you need to create two files: a NuGet package spec template and the asset provider script.

Create `asset-source.nuspec` file with the following content:
```
<?xml version="1.0" encoding="utf-8"?>
<package xmlns="http://schemas.microsoft.com/packaging/2010/07/nuspec.xsd">
    <metadata>
        <id>$sha$</id>
        <version>1.0.0</version>
        <description>example</description>
        <authors>example</authors>
    </metadata>
    <files>
        <file src="$file$" />
    </files>
</package>
```

Next, create `asset-provider.bat`, replace `<feed url>` and `<path to asset-source.nuspec>` with their correct values; vcpkg will invoke this script to do all the necessary work to restore and push artifacts to the asset cache.

```bat
set url=%1
set sha512=%2
set dst=%3
set "_dst=%dst:/=\%"
set "_sha512=%sha512:~0,90%"

cd /d %~dp3
%NUGET% install %sha512:~0,90% -Source <feed url>
if exist %_sha512%.1.0.0 (
    echo "Pull from the NuGet feed"
    cd %_sha512%.1.0.0

    REM Assume both are files not directories
    echo "F" | xcopy /f *.part %_dst%
) else (
    echo "Fetch from the url"
    curl.exe -L %url% --create-dirs --output %dst%
    %NUGET% pack <path to asset-source.nuspec> -BasePath %~dp3 -Properties "sha=%_sha512%;file=%dst%" -OutputDirectory %TEMP%
    %NUGET% push -ApiKey az -SkipDuplicate %TEMP%\%_sha512%.1.0.0.nupkg -Source <feed url>
)
```

NOTE: Packages are named with only the first 90 characters of their SHA512 followed by the version (1.0.0). This is because NuGet feed IDs are limited to 100 characters.

## Step 2: Configure the asset caching sources

Now that you have created the asset provider script, you need to instruct vcpkg to use it as an asset caching source. This can be done by setting the following environment variables:

```powershell
$env:X_VCPKG_ASSET_SOURCES="clear;x-script,C:/path/to/asset-provider.bat {url} {sha512} {dst};x-block-origin"
$env:NUGET="C:/path/to/nuget.exe"
$env:VCPKG_KEEP_ENV_VARS="NUGET;VCPKG_KEEP_ENV_VARS"
```

NOTE: Make sure to replace the placeholder paths to the asset provider script and nuget.exe with the correct paths in your system.

[`X_VCPKG_ASSET_SOURCES`](../users/config-environment.md#x_vcpkg_asset_sources) is the environment variable used to set asset caching sources for vcpkg to use. In this example we set the following values:
 
* `clear` gets rid of the default asset caching location.
* [`x-script`](../users/assetcaching.md#x-script) sets your script as an asset caching source, the first parameter indicates the command line vcpkg should invoke, in this example we call the `asset-provider.bat` script and forward some required parameters.
* `x-block-origin` forces all downloads to come from the configured asset caching sources. If the asset is not available in the asset cache the download fails.


[`VCPKG_KEEP_ENV_VARS`](../users/config-environment.md) is used to forward environment variables to vcpkg's build environment, we need to set this to `NUGET;VCPKG_KEEP_ENV_VARS` so that the values of these environment variables are available when vcpkg needs to call itself recursively.

After all has been properly setup you can run any install command and the assets should appear in your NuGet feed. 