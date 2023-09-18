---
title: Asset caching with vcpkg
description: Use asset caching with vcpkg to mirror your asset's downloads to improve build reliability.
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.topic: conceptual
ms.date: 9/18/2023
---
# Asset caching with vcpkg

[!INCLUDE [experimental](../../includes/experimental.md)]

vcpkg can use download mirrors to upload and restore assets, ensuring continued operation even when
the original download endpoints change or disappear.

Asset caching can help with these common development scenarios:

* Ensuring that download assets are available for continous integration runs.
* Mirroring download assets in a trusted location for air-gapped environments.

This article describes the method to configure asset caching and the available storage backend options.

## Configuration

Asset caching is configured via:
* the `X_VCPKG_ASSET_SOURCES` environment variable, or
* the `--x-asset-sources` command-line option.

In both cases, the expected value is a semicolon-delimited list of sources. Each source has a
specific syntax depending on its storage backend.

Use a backtick (\`) to escape characters inside the source strings.

## Sources

The `<rw>` parameter is optional and common to most of the sources described below. It controls
access permisions for the specific source and accpets the values `read`, `write`, or `readwrite`
(defaults to `read`).

### `clear`

**Syntax**: `clear`

Removes all previous sources in the configuration string. Useful in combination with the
`--x-asset-sources` parameter to disable all asset caching sources coming from the
`X_VCPKG_ASSET_SOURCES` environment variable.

### `x-azurl`

**Syntax**: 
1. `x-azurl,<url>[,<sas>[,<rw>]]`
2. `x-azurl,file://<network-location>[,,<rw>]`

Adds an Azure Blob Storage source.

`<url>`: Required. The Azure Blob Storage connection URL. It must include the container path and a trailing
`/`. 

`<sas>`: Optional. For endpoints using Shared Access Signature validation, use this parameter to provide the generated SAS.

Non-Azure endpoints can provide an access token using this parameter. As long as they can respond to
GET and PUT requests of the form `<url>/<sha512>?<sas>`. 

For example, a source configured as follows: `azurl,https://mydomain.com/vcpkg/,token=abc123,readwrite`, produces a request in the form: `https://mydomain.com/vcpkg/<sha512>?token=abc123`.

Alternatively, you can use a fielsystem location as the endpoint, by using the
`x-azurl,file://<filesystem-location>[,,<rw>]` pattern. 

For example, `x-azurl,file:///Z:/vcpkg/assetcache/,,readwrite` configures a cache in the `Z:/`
network folder.

The workflow of this asset source is:

1. Attempt to read from the mirror
1. (If step 1 failed) Read from the original url
1. (If step 2 succeeded) Write back to the mirror

You can enable/disable steps 1 and 3 via the [`<rw>`](#sources) specifier and you can disable step 2 via
`x-block-origin` below.

See also the [binary caching documentation for Azure Blob Storage](binarycaching.md#azblob) for more information on how to set up an `x-azurl` source.

#### `x-block-origin`

Syntax: `x-block-origin`

Disables falling back to the original download URL when an asset is not found in any of the
configured sources.

#### `x-script`

Syntax: `x-script,<template>`

Dispatches to an external tool to fetch the asset. 

`<template>`: Required. A command template that vcpkg executes to acquire an asset. For example:
`x-script,curl -L {url} --output {dst}` configures vcpkg to execute `curl` and provide the `{url}`
and `{dst}` parameters.

The logic to restore and upload assets and to connect to the cache storage endpoint are responsibility
of the command provided within the template.

vcpkg will substitute this arguments when executing the command:

* `{url}`: the original download URL for the asset.
* `{sha512}`: the expected SHA512 of the downloaded asset.
* `{dst}`: the location where the downloaded asset is expected.

The template command is run within vcpkg's build environment, this means that some environment
variables like `PATH` may be modified by the triplet.

## Next step

> [!div class="nexstepaction"]
> [Use a custom script as an asset caching source for
> NuGet](../examples/asset-caching-source-nuget.md)
