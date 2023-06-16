---
title: GitHub Integration
description: Information on the available vcpkg integration points with GitHub
author: michaelbprice
ms.author: miprice
ms.date: 06/15/2023
---

# Available GitHub Integrations

## Caching vcpkg-built binaries for your GitHub Actions workflows

Repositories that use GitHub Actions for continuous integration can take advantage of [vcpkg's binary caching feature](./users/binarycaching.md) to reduce the amount of time it takes to build their projects. There are two binary cache providers that are available to GitHub repositories.

### The GitHub Actions cache provider

[!INCLUDE [experimental](../../includes/experimental.md)]

The [GitHub Actions cache](https://docs.github.com/en/actions/using-workflows/caching-dependencies-to-speed-up-workflows) is intended to store a repository's intermediate build files that don't change often between jobs or workflow runs. GitHub provides a few different [tools to manage your Actions caches](https://docs.github.com/en/actions/using-workflows/caching-dependencies-to-speed-up-workflows#managing-caches), which include REST APIs and an extension to the gh command line tool, so that you can optimize caches for your workflow. vcpkg's integration with GitHub Actions cache is through the [`x-gha` binary source provider](./users/binarycaching.md#gha).

### The GitHub Packages NuGet cache provider

[GitHub Packages](https://docs.github.com/en/packages/learn-github-packages/introduction-to-github-packages) allows a repository to publish binary artifacts for public or private use. In addition to hosting storage for published packages, GitHub Packages supports a [variety of package management tools](https://docs.github.com/en/packages/learn-github-packages/introduction-to-github-packages#supported-clients-and-formats) by acting as a package registry. vcpkg can use the NuGet registry interface to GitHub Packages as a cache for vcpkg binary artifacts, by using the [`nuget` binary source provider](./users/binarycaching.md#nuget).

## The GitHub dependency graph

[!INCLUDE [experimental](./includes/experimental.md)]

> [!TIP]
> We are actively developing this feature and would like to hear your feedback. Let us know if you have any thoughts about the current functionality or any desired functionality by emailing [vcpkg@microsoft.com](mailto:vcpkg@microsoft.com) or by filing an issue in [vcpkg-docs](https://github.com/microsoft/vcpkg-docs/issues). If you have feedback on any of the GitHub features that this enables, let us know that too and we'll make sure it gets to the right folks at GitHub.

### About the GitHub dependency graph

The GitHub [dependency graph](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/about-the-dependency-graph) stores the set of dependencies for a repository. Beyond being able to visualize a repository's dependencies, GitHub builds several useful features on top of this data, including [dependency review](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/about-dependency-review) and [Dependabot alerts](https://docs.github.com/en/code-security/dependabot/dependabot-alerts/about-dependabot-alerts).

### vcpkg integration with the GitHub dependency graph

vcpkg has experimental support for populating the GitHub dependency graph from within a GitHub Actions workflow. To enable this feature, make the following changes in your workflow file:

* Set the `dependencygraph` feature flag to the environment variable `VCPKG_FEATURE_FLAGS`.
* Set the `GITHUB_TOKEN` environment variable with the GitHub workflow variable `${{ secrets.GITHUB_TOKEN }}`.
* Give GitHub Actions write access to the contents of your repository by including the following permissions block. This is required to write the dependency graph metadata to your repository. This __will not__ add any commits to your repository or make any other modification to the contents of your repository.

```yaml
permissions:
  contents: write
```

Additionally, you must enable the GitHub dependency graph in your repository's settings (enabled by default on public repositories). For private repositories, follow the [GitHub documentation to enable the dependency graph](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/configuring-the-dependency-graph#enabling-and-disabling-the-dependency-graph-for-a-private-repository) in order to satisfy this requirement.

### Known limitations

The following are known limitations in the current implementation. __Even though we know of these, if they affect the value for you, we'd still like to hear from you so that we can prioritize what we need to fix and how we should fix it.__

* The version of vcpkg that is bundled with GitHub Actions runners does not have this feature yet. Ensure you are using a version of vcpkg that supports this feature.
* Features that depend on the dependency graph, such as Dependabot alerts and Dependabot pull requests, are not yet available. Please let us know if you are interested in those features!

### Example GitHub Actions workflow

```yaml
name: Populate dependencies

on:
  push:
    branches: [ main ]
  workflow_dispatch:

permissions:
  contents: write

env:
  GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
  VCPKG_FEATURE_FLAGS: dependencygraph

jobs:
  build:

    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3
      with:
        submodules: 'recursive'

    # This will execute a dry-run, meaning that libraries will not be built and
    # installed, but they will still be reported to the GitHub dependency graph.
    # This step assumes `vcpkg` has been bootstrapped (run `./vcpkg/bootstrap-vcpkg`)
    - name: Run vcpkg
      run: ${{ github.workspace }}/vcpkg/vcpkg install --dry-run

```