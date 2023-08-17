---
title: vcpkg add
description: Reference for the vcpkg add command. Adds packages to the manifest or introduces new artifacts to the environment.
author: JavierMatosD
ms.author: javiermat
ms.date: 08/15/2023
ms.prod: vcpkg
---
# vcpkg add

## Synopsis

```console
vcpkg add port [options] <port-name>...
```

[!INCLUDE [experimental](../../includes/experimental.md)]
```console
vcpkg add artifact [options] <artifact-name>
```

## Description
The `add` command provides a way to either extend the project's manifest with new packages or to introduce new artifacts to the environment.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### port
`vcpkg add port <port-name>...` adds the specified ports to the manifest if it's not already present.

### artifact
[!INCLUDE [experimental](../../includes/experimental.md)]

`vcpkg add artifact <artifact-name>...` introduces the specified artifact to the environment.
