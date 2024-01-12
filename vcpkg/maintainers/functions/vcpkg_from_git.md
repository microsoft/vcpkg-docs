---
title: vcpkg_from_git
description: Learn how to use vcpkg_from_git.
author: vicroms
ms.author: viromer
ms.topic: reference
ms.date: 11/10/2023
---
# vcpkg_from_git

Download and extract a project from git.

This helper reuses `git` CLI credentials. See [Authentication](../../users/authentication.md) to
configure credentials for your private git hosts.

If the git repo is hosted on BitBucket, on GitHub, or in GitLab, their specific functions should
be used rather than this function:

* [`vcpkg_from_bitbucket`](vcpkg_from_bitbucket.md)
* [`vcpkg_from_github`](vcpkg_from_github.md)
* [`vcpkg_from_gitlab`](vcpkg_from_gitlab.md)

## Usage

```cmake
vcpkg_from_git(
    OUT_SOURCE_PATH <SOURCE_PATH>
    URL <https://android.googlesource.com/platform/external/fdlibm>
    REF <59f7335e4d...>
    [HEAD_REF <ref>]
    [PATCHES <patch1.patch> <patch2.patch>...]
    [LFS [<url>]]
)
```

## Parameters

### OUT_SOURCE_PATH
Specifies the out-variable that will contain the extracted location.

This should be set to `SOURCE_PATH` by convention.

### URL

The url of the git repository.

### REF

The git sha of the commit to download.

### FETCH_REF

The git branch to fetch in non-HEAD mode. After this is fetched,
then `REF` is checked out. This is useful in cases where the git server
does not allow checking out non-advertised objects.

### HEAD_REF

The git branch to use when the package is requested to be built from the latest sources.

Example: `main`, `develop`, `HEAD`

### PATCHES

A list of patches to be applied to the extracted sources.

Relative paths are based on the port directory.

### LFS
_Added in vcpkg version 2022.11.14_

Enable fetching files stored using Git LFS.
Only files pointed to by `REF` are fetched.

This makes Git LFS mandatory for the port.
It's a fatal error if the extension is not installed.

_Added in vcpkg version 2023.01.23_

If specified, `<url>` will be used instead of the original git URL for LFS.

## Notes

`OUT_SOURCE_PATH`, `REF`, and `URL` must be specified.

## Examples

- [fdlibm](https://github.com/Microsoft/vcpkg/blob/master/ports/fdlibm/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_from\_git.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_from_git.cmake)
