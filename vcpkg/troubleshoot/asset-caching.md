---
title: Troubleshoot asset caching issues
description: Troubleshooting guide for common asset caching issues
author: vicroms
ms.author: viromer
ms.date: 02/12/2024
ms.topic: troubleshooting-general
---

# Troubleshoot asset caching issues

This articles is intended for users experiencing issues while [setting
up](../commands/install.md) an asset cache. For instructions to set up an asset
cache read the [asset caching documentation](../concepts/asset-caching.md).

## Artifacts aren't being uploaded or restored from my asset cache

When vcpkg downloads assets it looks at available asset caches before
attempting to download from the internet.

By default, vcpkg emits no warnings or errors when a package is not found on an
asset cache. Instead, it silently falls back to downloading the asset from the Internet.

vcpkg prints the URL for each of the asset it downloads. You can notice asset
caching errors if the download URL does not match your asset cache's URL.

The `x-azurl` asset cache backend provided is designed to work with Azure Storage
Containes and other storage servies that accept requests that require an
authentication token.

The configuration is as follows:

`x-azurl,<url>,<sas>[,<rw>]`

* `<url>`: The container's base URL
* `<sas>`: A Shared Access Signature (SAS) token if working with Azure Storage
  Contianers; or an authentication request parameter if working with other providers.
* `<rw>`: (Optional) read/write permission configuration

> !NOTE
> While designed to work with Azure Storage Containers. The `x-azurl` backend
> can be used for storage services that accept requests in the form of
> `<url>?<sas>`.

For example, `x-azurl,https://contoso.com,token=TOKEN_VALUE,readwrite` will
result in a request in the form of `https://contoso.com?token=TOKEN_VALUE`.

If your asset cache provider requires no authorization the `<sas>` parameter can
be left empty. For example, `x-azurl,https://contoso.com,,readwrite`.

### Cause 1: The URL is not correctly formatted

When setting up your asset cache may sure that you include `https://` as part of
the URL.

Steps to resolve:

1 - Verify that the configured URL is correct.

### Cause 2: The authorization token is not correctly formatted

vcpkg uses the `<sas>` parameter to authenticate to your asset cache provider.
There are a variety of reasons why the authorization may not succeed.

If you're using an Azure Storage Container, the `<sas>` parameter should contain
only the SAS token without any other request parameter formatting. For example:

`x-azurl,https://mystorageaccount.blob.core.windows.net/mystoragecontainer,sasvaluehere,readwrite`

Read the documentation on generating [SAS
tokens](/azure/storage/blobs/blob-containers-portal#generate-a-shared-access-signature)
for more information. Make sure that the generated token's read and write
permissions match your required use case.

If you're using a different provider, you may require to format the request
parameters correctly. For example, prefixing a parameter name before the token's
value.

`x-azurl,https://contoso.com,authorization=tokenvaluehere,readwrite`.

Steps to resolve:

1 - Verify that the authorization token is not expired
2 - Verify that the authorization token has the correct permissions to your container
3 - Verify that the authorization token's value is correct
4 - Verify that the `<sas>` token format matches the expected format by your provider

## Issue isn't listed here

If your issue isn't listed here, visit [our
repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
