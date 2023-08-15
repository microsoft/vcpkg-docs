---
title: vcpkg test-features
description: Command line reference for the vcpkg test-features command. Builds all features of a package.
ms.date: 08/15/2023
---

# vcpkg test-features

## Synopsis

```console
vcpkg test-features [options] <package>...
```

## Description

Tests all features of a package by installing them in manifest mode. This command is useful for testing that all features of a package are working correctly.
The following tests are performed:
- The core feature gets installed with default featured disabled
- Every feature gets installed with default featured disabled
- All features combined gets installed

## Example

```console
$ vcpkg test-features gdal
Compute 34 install plans ...
Compute all abis ...
Detecting compiler hash for triplet arm64-osx...
Check the binary cache

1/34 gdal[core]:arm64-osx
...
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--ci-feature-baseline`

Path to the ci.feature.baseline.txt file. Used to skip ports and detect regressions.

You can find a documentation of the file [here](https://github.com/autoantwort/vcpkg/blob/ce6c48c28ced5a9d0cfe54614049637b6721cf7c/scripts/ci.feature.baseline.txt).

### `--all`

Test the features of all packages.

### `--dont-test-feature-core`

Skip testing the core feature.

### `--dont-test-features-separately`

Skip testing each feature separately.

### `--dont-test-features-combined`

Skip testing all features combined.

### `--failure-logs`

Directory to which failure logs will be copied.

### `--write-failure-abis-to`

Path to a file to which abis from ports that failed to build will be written.
