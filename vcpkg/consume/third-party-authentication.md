---
title: "Authenticate to third-party services"
description: Explain different authentication methods to third-party services consumed by vcpkg
author: vicroms
ms.author: viromer
ms.date: 10/31/2024
ms.topic: tutorial
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---
# Authenticate to third-party services

vcpkg has features that leverage third-party services like Azure DevOps, Azure
Containers, and other similar services, which require authentication to be used.
This article lists authentication methods for these services that work with
vcpkg.

## Azure DevOps
vcpkg can interact with AzureDevOps in multiple contexts: as a host for a source
code repository, as a backend for binary caching, or running as part of an ADO
pipeline.

To methods described below for [authenticating to Git repositories](#git) and
[NuGet feeds](#nuget) apply for repositories and artifact feeds hosted in Azure
DevOps. See the [tutorial for packaging an ADO
dependency](../produce/package-ado-dependency.md) or the [Test your custom
registry ports using vcpkg with Azure
DevOps](../produce/test-registry-ports-ado.md) for examples.

Azure DevOps provides a non-NuGet based binary caching backend in the form of
[Universal Packages](../reference/binarycaching.md#azuniversal). See the
[Quickstart](../reference/binarycaching.md#azuniversal-quickstart) section for
details on how to use and authenticate to this backend.

The [Test your custom registry ports using vcpkg with Azure
DevOps](../produce/test-registry-ports-ado.md) article contains multiple
examples of authenticating to other services when vcpkg is running as part of an
Azure DevOps pipeline job.

## Azure Blob Storage
vcpkg can use Azure Blob Storage as an asset caching backend. The
authentication method requires providing a SAS token as part of the [asset cache
configuration](../users/assetcaching.md#azurl). 

## Git
To authenticate to a [private Git
registry](../concepts/registries.md#git-registries) follow one of the methods in
the [Authenticate to Git repositories](../users/authentication.md) article.

If your [package's port](../concepts/ports.md) requires source code hosted in a
private Git repository, we recommend to use the
[`vcpkg_from_git`](../maintainers/functions/vcpkg_from_git.md) helper function
and follow one of the methods in the [Authenticate to Git
repositories](../users/authentication.md) article. Alternatively, but not
recommended the
[vcpkg_from_gitHub](../maintainers/functions//vcpkg_from_github.md#AUTHORIZATION_TOKEN)
and
[vcpkg_from_gitlab](../maintainers/functions/vcpkg_from_gitlab.md#AUTHORIZATION_TOKEN)
provide an `AUTHORIZATION_TOKEN` parameter.

Read the [Tutorial: Authenticate vcpkg Git-based registries in GitHub
Actions](../consume/gha-authentication.md) article to learn how to authenticate
to private Git repositories when running vcpkg in a GitHub Actions workflow.

## GitHub Actions
vcpkg can authenticate to GitHub Actions Cache to easily set up a [binary
cache](../reference/binarycaching.md#github-actions-cache) when running in a
GitHub Actions workflow. See the [Tutorial: Set up a vcpkg binary cache using
GitHub Actions Cache](../consume/binary-caching-github-actions-cache.md) article
for instructions and the [Test your custom registry ports using vcpkg with
GitHub Actions](../produce/test-registry-ports-gha.md) for an example.

You can also use [GitHub Packages](../reference/binarycaching.md#github-packages)
as your binary caching backend. See the
[Tutorial: Set up a vcpkg binary cache using GitHub Packages in a GitHub Actions
workflow](../consume/binary-caching-github-packages.md) article for
instructions.

The [Test your custom registry ports using vcpkg with GitHub
Actions](../produce/test-registry-ports-gha.md) article contains multiple
examples of authenticating to other services when vcpkg is running as part of a
GitHub Actions workflow.

## NuGet
To provide credentials for a NuGet feed used as a binary cache use one of the
methods in the [Binary Caching
configuration](../reference/binarycaching.md#nuget-credentials) article. See the
[binary caching using a NuGet
feed](../consume/binary-caching-nuget.md#provide-authentication-credentials)
tutorial for an example.
