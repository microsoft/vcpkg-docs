---
title: Asset Caching with an alternate storage source
description: Learn about adding a new storage source
ms.date: 4/18/2023
---

# Asset Caching with an alternate storage source

[!INCLUDE [experimental](../../includes/experimental.md)]

Your scenario might need a different storage source such as NuGet feed, Universal Packages, or something else.

In this example, we will set up a NuGet feed as a source, meaning we will use `nuget.exe` to pull and push assets.

First, we will set up the environment before any call to `vcpkg install`:
```powershell
$env:X_VCPKG_ASSET_SOURCES="clear;x-script,C:/path/to/sources.bat {url} {sha512} {dst};x-block-origin"
$env:NUGET=C:/path/to/nuget.exe
$env:VCPKG_KEEP_ENV_VARS=NUGET
```
`X_VCPKG_ASSET_SOURCES` passes in a script `sources.bat` which will handle `nuget` push and pull commands. The `vcpkg` environment needs to use a copy of `nuget.exe` which can be supplied by the environment with [VCPKG_KEEP_ENV_VARS](config-environment.md#vcpkg_keep_env_vars).

The script `sources.bat` checks if the asset is stored in the feed, and if so, installs it to the destination path. If the asset is not stored on the feed, the script fetches the url via curl, packs the asset in a `nuget` package and pushes it to the feed.


`sources.bat`
```bat
set url=%1
set sha512=%2
set dst=%3
set "_dst=%dst:/=\%"
set "_sha512=%sha512:~0,90%"

cd %dst%/../
%NUGET% install %sha512:~0,90% -Source <feed url>
if exist %_sha512%.1.0.0 (
    echo "Pull from the NuGet feed"
    cd %_sha512%.1.0.0

    REM Assume both are files not directories
    echo "F" | xcopy /f *.part %_dst%
) else (
    echo "Fetch from the url"
    curl.exe -L %url% --create-dirs --output %dst%
    cd %dst%/../
    mkdir tempdir
    cd tempdir
    xcopy /f C:\path\to\test.nuspec .
    echo %_dst%
    xcopy /f %_dst% .

    %NUGET% pack test.nuspec -Properties sha=%_sha512%
    %NUGET% push %_sha512%.1.0.0.nupkg -Source <feed url>
)
```

`test.nuspec`
```
<?xml version="1.0" encoding="utf-8"?>
<package xmlns="http://schemas.microsoft.com/packaging/2010/07/nuspec.xsd">
    <metadata>
        <id>$sha$</id>
        <version>1.0.0</version>
        <description>example</description>
        <authors>example</authors>
    </metadata>
</package>
```

You will notice that the NuGet package pushed to the feed is named the only the first 90 characters of the `sha`. This is because `nuget` feed ids are limited to 100 characters.