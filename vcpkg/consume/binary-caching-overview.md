---
title: "What is binary caching?"
description: Learn what is a vcpkg binary cache and why you should be using one.
author: vicroms
ms.author: viromer
ms.date: 9/25/2023
ms.prod: vcpkg
ms.topic: overview
#CustomerIntent: As a beginner vcpkg user, I want to learn what is binary caching and why should I use it
---
# What is binary caching?

Binary caching is a feature that lets vcpkg save the compiiled files of a package in a shared
location and reuse them when asked to build the same package with the same configuration. We call
the output produced by each package build a "binary package"

## Why use binary caching?

Most ports in the [vcpkg public registry](<https://github.com/Microsoft/vcpkg>) are built from
source. Building from source ensures maximum compatibility by using the same environment, build
tools, and configurations you use for your project in your dependencies. However, this  can also
result in duplicated builds when multiple projects, developers, or machines require the same
dependencies.

Binary caches can be hosted in a variety of environments, the most basic form of binary caching is a
directory containing zipped archives of a package's build output. More advanced scenarios include
NuGet package feeds hosted in [GitHub Packages](<https://docs.github.com/en/packages>), [Azure
DevOps Artifacts](azure/devops/artifacts/start-using-azure-artifacts?view=azure-devops&tabs=nuget),
or other such services.

Binary caching is especially effective in scenarios where dependencies and configuration
don't change often, such as in CI (Continuous Integration) or build servers. Binary packages are
particularly susceptible to configuration changes (compiler version, compilation flags, installed
tools, vcpkg-specific script changes, etc) which makes sharing them between developer machines
difficult. See the [`vcpkg export`](../commands/export.md) for a different option to share built
packages.

See the [binary caching documentation](../users/binarycaching.md) to learn about the different [storage backends](../users/binarycaching.md#providers).

## Next steps

> [!div class="nextstepaction"]
> [Set up a local binary cache](binary-caching-local.md)

Here are some additional tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)
