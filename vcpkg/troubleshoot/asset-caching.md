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

The `x-azurl` asset cache backend provided by vcpkg works with Azure Storage
Container URL. When working with Azure Containers the following parameters are
required:

`x-azurl,<url>,<sas>[,<rw>]`

* `<url>`: The container's URL
* `<sas>`: A SAS key
* `<rw>`: (Optional) read/write permissions

However, the `x-azurl` backend can be used for storage services that accept
requests in the form of `<url>?<sas>`.

For example, `x-azurl,https://contoso.com,token=TOKEN_VALUE,readwrite` will
result in requests in the form of `https://contoso.com?token=TOKEN_VALUE`.

If your asset cache provider requires no authorization the `<sas>` parameter can
be left empty. For example, `x-azurl,https://contoso.com,,readwrite`.

### Cause 1: The URL is not correct

When setting up your asset cache may sure that you include `https://` as part of
the URL.

### Cause 2: The SAS token is not correct

vcpkg uses the SAS token you provide to authenticate to your asset cache
provider. There are a variety of reasons why the SAS token may not be accepted.

If you're using an Azure Storage Container, the `<sas>` parameter should contain
the SAS token only without any other request parameter formatting.

If you're using a different provider, you may require to format the request
parameters correctly. For example, prefixing `token=` before the actual SAS
token value.

Steps to resolve:

1 - Verify that provided token isn't expired
2 - Verify that the value of the token is correct
3 - Verify that your `<sas>` parameter configuration is properly formatted

## Issue isn't listed here

If your issue isn't listed here, visit [our
repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
