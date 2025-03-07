---
title: vcpkg list
description: Command line reference for the vcpkg list command. Shows a list of the packages in the installed tree, along with the version and description of each.
ms.date: 01/10/2024
---
# vcpkg list

## Synopsis

```console
vcpkg list [options]
```

## Description

Shows a list of the packages in the installed tree, along with the version and description of each.

## Example

```console
$ vcpkg list

cxxopts:x64-windows              3.0.0              This is a lightweight C++ option parser library...
fmt:x64-windows                  9.0.0              Formatting library for C++. It can be used as a...
range-v3:x64-windows             0.12.0             Range library for C++11/14/17/20.
vcpkg-cmake-config:x64-windows   2022-02-06#1
vcpkg-cmake:x64-windows          2022-08-18
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--x-full-desc`

Print the full, untruncated description of each package.
