---
title: vcpkg hash
description: Reference for the vcpkg hash command. Computes the hash of a specified file.
author: JavierMatosD
ms.author: javiermat
ms.date: 08/17/2023
ms.prod: vcpkg
---


# vcpkg hash

## Synopsis

```console
vcpkg hash <file-to-hash> [algorithm]
```
## Description

Computes the hash of a specified file. By default, it computes the SHA-512 hash, but another algorithm can be optionally specified. The output will be the computed hash for the given file displayed directly on the console.

>[!NOTE]
> - The command requires two arguments:
>1. A mandatory file path.
>2. An optional hash algorithm, either `SHA256` or `SHA512``. If omitted, it will default to using SHA512 to compute the file's hash.

## Examples

To compute the SHA-512 for the `boost_1_62_0.tar.bz2`:
```console
vcpkg hash boost_1_62_0.tar.bz2
```

To compute the hash for the file `boost_1_62_0.tar.bz2` using a specific algorithm (e.g., SHA-256):
```console
vcpkg hash boost_1_62_0.tar.bz2 sha256
```

## Options

All vcpkg commands support a set of [common options](common-options.md).
