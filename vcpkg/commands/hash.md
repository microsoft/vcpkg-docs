---
title: vcpkg hash
description: Reference for the vcpkg hash command. Computes the hash of a specified file.
author: JavierMatosD
ms.author: javiermat
ms.date: 08/17/2023
---


# vcpkg hash

## Synopsis

```console
vcpkg hash <file-to-hash> [algorithm]
```
## Description

Computes the hash of a specified file. By default, it computes the SHA-512 hash, but another algorithm can be optionally specified. The output will be the computed hash for the given file displayed directly on the console.

## Examples

To compute the SHA-512 for the `boost_1_83_0.tar.bz2`:
```console
vcpkg hash boost_1_83_0.tar.bz2

-- d133b521bd754dc35a9bd30d8032bd2fd866026d90af2179e43bfd7bd816841f7f3b84303f52c0e54aebc373f4e4edd601a8f5a5e0c47500e0e852e04198a711
```

To compute the hash for the file `boost_1_83_0.tar.bz2` using a specific algorithm (e.g., SHA-256):
```console
vcpkg hash boost_1_83_0.tar.bz2 sha256

-- 6478edfe2f3305127cffe8caf73ea0176c53769f4bf1585be237eb30798c3b8e
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### Algorithms
- SHA-512
- SHA-256
