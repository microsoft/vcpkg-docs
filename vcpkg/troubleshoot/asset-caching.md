---
title: Troubleshoot asset caching issues
description: Troubleshooting guide for common asset caching issues
author: vicroms
ms.author: viromer
ms.date: 02/12/2024
ms.topic: troubleshooting-general
---

# Troubleshoot asset caching issues

Asset caching accelerates package retrieval by storing downloaded packages (assets)
locally or on a network, reducing dependency on external sources. This guide assists
users facing difficulties with asset cache setup or operation.

For initial setup instructions, refer to the [asset caching documentation](../concepts/asset-caching.md).

## Diagnosing asset caching errors

vcpkg checks available asset caches before downloading artifacts from the Internet.
By default, it silently falls back to external sources if an asset is not found in
the cache.

The [`x-block-origin``](../users/assetcaching.md#x-block-origin) option transforms
asset restoration failures into explicit build errors, preventing unintended external
downloads. Use this option to enhance security and make missing cache hits more visible.

### Detect asset restore issues

Asset restoration issues are silent by default. They can be detected by validating
the download URL of an artifact.

```Console
Downloading 7zip...
https://www.7-zip.org/a/7z2301-extra.7z -> C:\vcpkg\downloads\7z2301-extra.7z
```

Or an explicit error when the `x-block-origin` option is enabled:

```Console
error: Failed to download from mirror set
```

### Detect asset upload issues

If an asset source is correctly configured, vcpkg emits a warning when an asset can't
be uploaded to the cache.

```Console
warning: failed to store back to mirror
```

## <a name="cache-failure"></a> Artifacts aren't being uploaded or restored from my asset cache

The `x-azurl` asset cache backend provided by vcpkg, is designed to work with
Azure Storage Containers, it may work with other storage services that accept
PUT requests with simple token authentication.

The configuration has the following format:

`x-azurl,<url>,<sas>[,<rw>]`

* `<url>`: The container's base URL
* `<sas>`: A Shared Access Signature (SAS) token if working with Azure Storage
  Containers; or an authentication request parameter if working with other providers.
* `<rw>`: (Optional) read/write permission configuration

> [!NOTE]
> While designed to work with Azure Storage Containers. The `x-azurl` backend
> can be used for storage services that accept requests in the form of
> `<url>?<sas>`.

For example, `x-azurl,https://contoso.com,token=TOKEN_VALUE,readwrite` results in
a request in the form of `https://contoso.com?token=TOKEN_VALUE`.

If your asset cache provider requires no authorization, the `<sas>` parameter can
be left empty. For example, `x-azurl,https://contoso.com,,readwrite`.

### Cause 1: The URL is not correctly formatted

When setting up your asset cache, make sure that you include `https://` as part of
the URL.

Steps to resolve:

1 - Verify that the configured URL is correct.

### Cause 2: The authorization token is not correctly formatted

vcpkg uses the `<sas>` parameter to authenticate to your asset cache provider.
There are a variety of reasons why the authorization may not succeed.

If you're using an Azure Storage Container, the `<sas>` parameter should contain
only the SAS token without any other request parameter formatting. For example:

`x-azurl,https://mystorageaccount.blob.core.windows.net/mystoragecontainer,sasvaluehere,readwrite`

Read the documentation on [generating SAS
tokens](/azure/storage/blobs/blob-containers-portal#generate-a-shared-access-signature)
for more information. Make sure that the generated token's read and write
permissions match your required use case.

If you're using a different provider, you may need to format the request
parameters correctly. For example, prefixing a parameter name before the token's
value.

`x-azurl,https://contoso.com,authorization=tokenvaluehere,readwrite`.

Steps to resolve:

1 - Verify that the authorization token is not expired

2 - Verify that the authorization token has the correct permissions to your container

3 - Verify that the authorization token's value is correct

4 - Verify that the authorization token's format matches the expected format by your provider.

## I get a "Failed to download from mirror set" error message

This issue occurs when the asset caching configuration contains
[`x-block-origin`](../users/assetcaching.md#x-block-origin).

Using `x-block-origin` turns failures to restore an asset from any configured cache
into package build failures. Preventing possibly unwanted access to external sources.

Steps to resolve:

1 - Verify that your network has access to the asset source

2 - Verify that the requested asset exists in at least one of the configured asset
caches. If the package does not exist there are two methods to resolve the issue:

  * Disabling `x-block-origin` temporarily, to allow vcpkg to download the asset
    and upload it to a configured asset cache.
  * Uploading the asset manually to one of the configured asset caches.

3 - Verify that the asset caches are properly configured, follow the steps in
[Artifacts aren't being uploaded or restored from my asset cache](#cache-failure).

## Issue isn't listed here

If your issue isn't listed here, visit [our
repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
