---
title: "What is binary caching?"
description: Learn what is a vcpkg binary cache and why you should be using one.
author: vicroms
ms.author: viromer
ms.date: 07/30/2024
ms.topic: concept-article
#CustomerIntent: As a beginner vcpkg user, I want to learn what is binary caching and why should I use it
---
# What is binary caching?

Binary caching is a feature that lets vcpkg save the compiled files of a package in a shared
location and reuse them when asked to build the same package with the same configuration. We call
the output produced by each package build a "binary package".

## Why use binary caching?

Most ports in the [vcpkg curated registry](<https://github.com/Microsoft/vcpkg>) are built from
source. Building from source ensures maximum compatibility by using the same environment, build tools,
and configurations you use for your project in your dependencies. However, this  can also
result in duplicated builds when multiple projects, developers, or machines require the same
dependencies.

Binary caches can be hosted in a variety of environments. The most basic form of binary caching is a
directory that holds zipped archives of a package's build output. More advanced scenarios include
NuGet package feeds hosted in [GitHub Packages](<https://docs.github.com/en/packages>), [Azure
DevOps
Artifacts](/azure/devops/artifacts/start-using-azure-artifacts?view=azure-devops&tabs=nuget&preserve-view=true),
or other such services.

Binary caching is especially effective in scenarios where dependencies and configuration
don't change often, such as in CI (Continuous Integration) or build servers. Binary packages are
particularly susceptible to configuration changes (compiler version, compilation flags, installed
tools, or vcpkg-specific script changes) which makes sharing them between developer machines
difficult. See [`vcpkg export`](../commands/export.md) for a different option to share built
packages.

See the [binary caching reference documentation](../reference/binarycaching.md) to learn about
the different [storage backends](../reference/binarycaching.md#providers).

## Next steps

> [!div class="nextstepaction"]
> [Set up a local binary cache](binary-caching-local.md)

Here are other tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a local binary cache](binary-caching-local.md)
* [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Packages](binary-caching-github-packages.md)
* [Authenticate to private NuGet feeds](../reference/binarycaching.md#nuget-credentials)
