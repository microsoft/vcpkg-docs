---
title: vcpkg_from_github
description: Learn how to use vcpkg_from_github.
ms.date: 04/24/2025
---
# vcpkg_from_github

Download and extract a project from GitHub. Enables support for `install --head`.

This also works with Gitea by specifying the Gitea server with the `GITHUB_HOST` option.

## Usage

```cmake
vcpkg_from_github(
    OUT_SOURCE_PATH <SOURCE_PATH>
    REPO <Microsoft/cpprestsdk>
    [REF <v2.0.0>]
    [SHA512 <45d0d7f8cc350...>]
    [HEAD_REF <master>]
    [PATCHES <patch1.patch> <patch2.patch>...]
    [GITHUB_HOST <https://github.com>]
    [AUTHORIZATION_TOKEN <${SECRET_FROM_FILE}>]
    [FILE_DISAMBIGUATOR <N>]
    [USE_TARBALL_API]
)
```

## Parameters

### OUT_SOURCE_PATH

Specifies the out-variable that will contain the extracted location.

This should be set to `SOURCE_PATH` by convention.

### REPO

The organization or user and repository on GitHub.

### REF

A stable git commit-ish (ideally a tag or commit) that will not change contents. **This should not be a branch.**

For repositories without official releases, this can be set to the full commit id of the current latest master.

If `REF` is specified, `SHA512` must also be specified.

### SHA512

The SHA512 hash that should match the archive (https://github.com/${REPO}/archive/${REF}.tar.gz).

This is most easily determined by first setting it to `0`, then trying to build the port. The error message will contain the full hash, which can be copied back into the portfile.

### HEAD_REF

The unstable git commit-ish (ideally a branch) to pull for `--head` builds.

For most projects, this should be `master`. The chosen branch should be one that is expected to be always buildable on all supported platforms.

### PATCHES

A list of patches to be applied to the extracted sources.

Relative paths are based on the port directory.

### GITHUB_HOST

A replacement host for enterprise GitHub instances.

This field should contain the scheme, host, and port of the desired URL without a trailing slash.

### AUTHORIZATION_TOKEN

A token to be passed via the Authorization HTTP header as "token ${AUTHORIZATION_TOKEN}". If `AUTHORIZATION_TOKEN` is a [fine-grained access token](https://docs.github.com/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens#fine-grained-personal-access-tokens), you must also use [`USE_TARBALL_API`](#use_tarball_api).

### FILE_DISAMBIGUATOR

A token to uniquely identify the resulting filename if the SHA512 changes even though a git ref does not, to avoid stepping on the same file name.

### USE_TARBALL_API

When specified, vcpkg will use the tarball API documented at [GitHub REST API](https://docs.github.com/rest/repos/contents?#download-a-repository-archive-tar) rather than the archive API to download sources.

This option exists rather than being a globally controlled setting because the SHA512 of sources returned by the tarball API differs from the regular archives API.

## Notes

At least one of `REF` and `HEAD_REF` must be specified, however it is preferable for both to be present.

This exports the `VCPKG_HEAD_VERSION` variable during head builds.

## Examples

- [cpprestsdk](https://github.com/Microsoft/vcpkg/blob/master/ports/cpprestsdk/portfile.cmake)
- [ms-gsl](https://github.com/Microsoft/vcpkg/blob/master/ports/ms-gsl/portfile.cmake)
- [boost-beast](https://github.com/Microsoft/vcpkg/blob/master/ports/boost-beast/portfile.cmake)

## Source

[scripts/cmake/vcpkg\_from\_github.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_from_github.cmake)
