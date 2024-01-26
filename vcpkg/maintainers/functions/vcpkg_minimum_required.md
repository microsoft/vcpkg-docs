---
title: vcpkg_minimum_required
description: Learn how to use vcpkg_minimum_required.
ms.date: 01/10/2024
---
# vcpkg_minimum_required

Asserts that the version of the vcpkg program being used to build a port is later than the supplied date, inclusive.

## Usage

```cmake
vcpkg_minimum_required(VERSION 2021-01-13)
```

## Parameters

### VERSION

The date-version to check against.

## Source

[scripts/cmake/vcpkg\_minimum\_required.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_minimum_required.cmake)
