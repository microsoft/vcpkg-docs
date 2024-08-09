---
title: Asset caching reference
description: Reference documentation for the asset caching feature configuration and capabilities.
author: vicroms
ms.author: viromer
ms.topic: reference
ms.date: 01/10/2024
---
# Asset caching reference

## Sources

The `<rw>` parameter is optional and common to most sources described below. It controls
access permissions for the specific source and accepts the values `read`, `write`, or `readwrite`
(defaults to `read`).

Use a backtick (\`) to escape characters inside the source strings.

### <a name="clear"></a> `clear`

**Syntax**: `clear`

Removes all previous sources in the configuration string. Useful in combination with the
`--x-asset-sources` parameter to disable all asset caching sources coming from the
`X_VCPKG_ASSET_SOURCES` environment variable.

### <a name="x-azurl"></a>Azure Blob Storage (`x-azurl`)

**Syntax**:
1. `x-azurl,<url>[,<sas>[,<rw>]]`
1. `x-azurl,file://<network-location>[,,<rw>]`

`<url>`: Required. The Azure Blob Storage connection URL. It must include the container path and a trailing
`/`.

`<sas>`: Optional. For endpoints using Shared Access Signature validation, use this parameter to provide the generated SAS.

Non-Azure endpoints can provide an access token using this parameter. As long as they can respond to
GET and PUT requests of the form `<url>/<sha512>?<sas>`.

For example, a source configured as follows: `azurl,https://mydomain.com/vcpkg/,token=abc123,readwrite`, produces a request in the form: `https://mydomain.com/vcpkg/<sha512>?token=abc123`.

Alternatively, you can use a filesystem location as the endpoint by using the
`x-azurl,file://<filesystem-location>[,,<rw>]` pattern.

For example, `x-azurl,file:///Z:/vcpkg/assetcache/,,readwrite` configures a cache in the `Z:/`
network folder.

The workflow of this asset source is:

1. Attempt to read from the mirror
1. (If step 1 failed) Read from the original url
1. (If step 2 succeeded) Write back to the mirror

You can enable/disable steps 1 and 3 via the [`<rw>`](#sources) specifier, and you can disable step 2 via
`x-block-origin` below.

See also the [binary caching documentation for Azure Blob
Storage](../reference/binarycaching.md#azblob) for more information on how to
set up an `x-azurl` source.

#### <a name="x-block-origin"></a> `x-block-origin`

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

vcpkg will substitute these arguments when executing the command:

* `{url}`: the original download URL for the asset.
* `{sha512}`: the expected SHA512 of the downloaded asset.
* `{dst}`: the location where the downloaded asset is expected.

The template command is run within vcpkg's build environment; this means that some environment
variables like `PATH` may be modified by the triplet.

## Next step

> [!div class="nexstepaction"]
> [Use a custom script as an asset caching source for
> NuGet](../examples/asset-caching-source-nuget.md)
