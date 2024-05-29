---
title:  vcpkg in continuous integration environments
description: This article describes how to effectively use vcpkg in continuous integration environments.
author: vicroms
ms.author: viromer
ms.date: 5/10/2024
ms.topic: concept-article
---
# vcpkg in Continuous Integration environments

This article describes how to effectively use vcpkg in a continuous integration
(CI) environment. Either because you are using vcpkg to manage your project's
dependencies or you have a custom vcpkg registry and want to add CI tests for
your ports.

## Acquire vcpkg

The steps to acquire vcpkg in a CI environment are the same as those to acquire
vcpkg in your development environment. You need to acquire a clone of the vcpkg
repository at <https://github.com/Microsoft.com> and you need to run the
bootstrap script to get a vcpkg executable.

Regardless of the acquisition method you follow, it is highly recommended that
your vcpkg instance has a complete Git history. The versioning feature of vcpkg
relies on having a complete commit history to retrieve specific port versions. 

### Use vcpkg as a submodule

An easy way to acquire vcpkg in your CI environment is to declare the vcpkg
repository as a submodule of your repository.

```bash
git submodule init
./vcpkg/vcpkg-bootstrap.sh
```

### Use Git CLI

A secondary method is to run a Git command as part of your CI scripts to clone
an instance of the vcpkg repository.

```bash
git clone http://github.com/Microsoft/vcpkg
./vcpkg/vcpkg-bootstrap.sh
```

### Use a third-party action/script

Depending on the vendor you use to run your CI environment, there may be readily
available third-party scripts that can acquire vcpkg in a single step. The vcpkg
team does not maintain any of these third-party scripts and you should exercise
proper caution if you intend to use any of them.

### Other acquisition methods

The following methods are not recommended for CI environments.

* The one-line installation script: `iex (iwr -useb
  https://aka.ms/vcpkg-init.ps1)` or `. <(curl https://aka.ms/vcpkg-init.sh
  -L)`.
  This is a vcpkg distribution method that enables the "artifacts" experimental
  feature and is not recommended for most users.

## Set up binary cache

It is highly recommended that you enable a binary cache to avoid rebuilding your
project's dependencies on each CI run.

To learn more about binary caching read these articles:

* [What is binary Caching](../consume/binary-caching-overview.md)
* [Binary caching in GitHub
  Actions](../consume/binary-caching-github-actions-cache.md)
* [Authenticate NuGet
  feeds](../consume/binary-caching-nuget.md#provide-authentication-credentials)
* [Binary caching configuration reference](../reference/binarycaching.md)


## Set up an asset cache

Setting up an asset cache will make your CI environment more resilient to errors
caused by external services. 

To learn more about asset caching read these articles:

* [What is asset caching](../concepts/asset-caching.md)
* [Set up an asset cache](../consume/asset-caching.md)

## Examples

These articles show you examples of how everything works together:

* [Test your custom registry ports using vcpkg with Azure
  DevOps](../produce/test-registry-ports-ado.md)
* [Test your custom registry ports using vcpkg with GitHub
  Actions](../produce/test-registry-ports-gha.md)
