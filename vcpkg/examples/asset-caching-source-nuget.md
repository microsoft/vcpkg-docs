---
title: How to create a x-script Asset Caching source for NuGet
description: Learn about how to create a x-script Asset Caching source for NuGet
ms.date: 4/18/2023
---

# Creating a x-script Asset Caching source for NuGet

[!INCLUDE [experimental](../../includes/experimental.md)]

In this example, we will set up a NuGet feed as a source, meaning we will use `nuget.exe` to pull and push assets.

First, we will set up the environment before any call to `vcpkg install`:
```powershell
$env:X_VCPKG_ASSET_SOURCES="clear;x-script,C:/path/to/asset-provider.bat {url} {sha512} {dst};x-block-origin"
$env:NUGET=C:/path/to/nuget.exe
$env:VCPKG_KEEP_ENV_VARS=NUGET
```
[`X_VCPKG_ASSET_SOURCES`](../users/config-environment.md#x_vcpkg_asset_sources) defines a single [`x-script`](../users/assetcaching.md#x-script) asset source while blocking fallback to the original url ([`x-block-origin`](../users/assetcaching.md#x-block-origin)). The `x-script` source is defined to call a script `sources.bat` which will handle `nuget` push and pull commands. This script must be able to run both inside and outside the inner build environment, so we pass it the path to `nuget.exe` in the environment with `NUGET` and [`VCPKG_KEEP_ENV_VARS`](../users/config-environment.md#vcpkg_keep_env_vars).

The script `asset-provider.bat` checks if the asset is stored in the feed, and if so, installs it to the destination path. If the asset is not stored on the feed, the script fetches the url via curl, packs the asset in a `nuget` package and pushes it to the feed.


Create a `asset-provider.bat` file in your project with the following content:
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
    %NUGET% pack C:\path\to\asset-source.nuspec -BasePath %~dp3 -Properties "sha=%_sha512%;file=%dst%" -OutputDirectory %TEMP%
    %NUGET% push %TEMP%\%_sha512%.1.0.0.nupkg -Source <feed url>
)
```

Create a `asset-source.nuspec` file with the following content:
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

You will notice that the NuGet package pushed to the feed is named the only the first 90 characters of the `sha`. This is because `nuget` feed ids are limited to 100 characters.