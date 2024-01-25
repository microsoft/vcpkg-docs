---
title: "Tutorial: Set up a vcpkg binary cache using GitHub Actions Cache"
description: This tutorial shows how to set up a vcpkg binary cache using a NuGet feed hosted using GitHub Actions Cache
author: vicroms
ms.author: viromer
ms.topic: tutorial
ms.date: 09/27/2023
#CustomerIntent: As a vcpkg user, I want to setup binary caching in my GitHub Actions workflow using GitHub Packages as the binary cache storage
---
# Tutorial: Set up a vcpkg binary cache using GitHub Actions Cache

[!INCLUDE [experimental](../../includes/experimental.md)]

vcpkg supports using the GitHub Actions cache as binary caching storage when running in the context of
a GitHub Actions workflow. If you don't care about uploading binary packages to an external NuGet
feed, this is the recommended method to use binary caching in a GitHub Actions workflow. Otherwise,
read the [tutorial to use GitHub Packages in a GitHub Actions
workflow](binary-caching-github-packages.md).

In this tutorial, you'll learn how to:

> [!div class="checklist"]
>
> * [Export required GitHub Actions environment variables](#1---export-required-github-actions-envrionment-variables)
> * [Configure vcpkg to use the GitHub Actions
>   cache](#2---configure-vcpkg-to-use-the-github-actions-cache)

## Prerequisites

* A code editor
* A GitHub repository using GitHub Actions
* A project using vcpkg

## 1 - Export required GitHub Actions envrionment variables

vcpkg needs the Actions Cache URL and Runtime Token to be exported as environment variables
available in your workflow. Copy the following step in your workflow file:

```YAML
- name: Export GitHub Actions cache environment variables
  uses: actions/github-script@v6
  with:
    script: |
      core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
      core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');
```

## 2 - Configure vcpkg to use the GitHub Actions cache

Next set the value of `VCPKG_BINARY_SOURCES` as follows:

```YAML
env: 
    VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"
```

And that's it! vcpkg will now upload or restore packages from your GitHub Actions cache.

## Next steps

> [!div class="nextstepaction"]
> [Authenticate to private NuGet feeds](../users/binarycaching.md#nuget-credentials)

Here are other tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a local binary cache](binary-caching-local.md)
* [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Packages](binary-caching-github-packages.md)
