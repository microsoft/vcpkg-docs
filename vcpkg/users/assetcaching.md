---
title: Asset Caching
description: Use vcpkg to mirror sources and tools internally to guarantee build reliability.
ms.date: 11/30/2022
---
# Asset caching

[!INCLUDE [experimental](../../includes/experimental.md)]

vcpkg can utilize mirrors to cache downloaded assets, ensuring continued operation even if the original source changes or disappears.

In-tool help is available via `vcpkg help assetcaching`.

## Configuration

Asset caching can be configured by setting the environment variable `X_VCPKG_ASSET_SOURCES` to a semicolon-delimited
list of source strings. Characters can be escaped using backtick (\`).

### Valid source strings

The `<rw>` optional parameter for certain strings controls how they will be accessed. It can be specified as `read`,
`write`, or `readwrite` and defaults to `read`.

#### `clear`

Syntax: `clear`

Removes all previous sources

#### `x-azurl`

Syntax: `x-azurl,<url>[,<sas>[,<rw>]]`

Adds an Azure Blob Storage source, optionally using Shared Access Signature validation. URL should include the container
path and be terminated with a trailing `/`. SAS, if defined, should be prefixed with a `?`. Non-Azure servers will also
work if they respond to GET and PUT requests of the form: `<url><sha512><sas>`. As an example, if you set
`X_VCPKG_ASSET_SOURCES` to `x-azurl,https://mydomain.com/vcpkg/,token=abc123,readwrite` your server should respond to
`GET` and `PUT` requests of the form `https://mydomain.com/vcpkg/<sha512>?token=abc123`.

You can also use the filesystem (e.g. a network drive) via `file://` as asset cache. For example you then set
`X_VCPKG_ASSET_SOURCES` to `x-azurl,file:///Z:/vcpkg/assetcache/,,readwrite` when you have a network folder mounted at
`Z:/`.

The workflow of this asset source is:

1. Attempt to read from the mirror
1. (If step 1 failed) Read from the original url
1. (If step 2 succeeded) Write back to the mirror

You can enable/disable steps 1 and 3 via the [`<rw>`](#valid-source-strings) specifier and you can disable step 2 via
`x-block-origin` below.

See also the [binary caching documentation for Azure Blob Storage](binarycaching.md#azblob) for more information on how to set up an `x-azurl` source.

#### `x-block-origin`

Syntax: `x-block-origin`

Disables use of the original URLs in case the mirror does not have the file available.

#### `x-script`

Dispatches to an external tool to fetch the asset. Within the template, `{url}` will be replaced by the original url, `{sha512}` will be replaced by the SHA512 value, and `{dst}` will be replaced by the output path to save to. These substitutions will all be properly shell escaped, so an example template would be: `curl -L {url} --output {dst}`. `{{` will be replaced by `}` and `}}` will be replaced by `}` to avoid expansion. Note that this will be executed inside the build environment, so the PATH and other environment variables will be modified by the triplet.

## Example

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