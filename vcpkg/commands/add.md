---
title: vcpkg add
description: Reference for the vcpkg add command. Adds packages to the manifest.
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

## Description
The `add` command adds packages to the manifest (`vcpkg.json`).

## Options

All vcpkg commands support a set of [common options](common-options.md).

### port
`vcpkg add port <port-name>...` adds the specified ports to the manifest if it's not already present.
