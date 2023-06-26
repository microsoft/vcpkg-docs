---
title: GitHub integration
description: Information on the available vcpkg integration points with GitHub
author: michaelbprice
ms.author: miprice
ms.date: 06/15/2023
---

# GitHub integration points

## <a name="caching"></a> Caching vcpkg-built binaries for your GitHub Actions workflows

vcpkg's binary caching feature reduces the amount of time it takes to build projects that use GitHub Actions for continuous integration. There are two binary cache providers that are available to GitHub repositories, the GitHub Actions cache provider and the GitHub Packages NuGet cache provider. For more information on these GitHub features, see [GitHub Actions cache](https://docs.github.com/actions/using-workflows/caching-dependencies-to-speed-up-workflows) and [GitHub Packages](https://docs.github.com/packages/learn-github-packages/introduction-to-github-packages). For more information on vcpkg binary caching, see our [binary caching feature documentation](./users/binarycaching.md).

### <a name="caching-actions"></a> The GitHub Actions cache provider

[!INCLUDE [experimental](../includes/experimental.md)]

The GitHub Actions cache is intended to store a repository's intermediate build files that don't change often between jobs or workflow runs. For GitHub users, the GitHub Actions cache is a natural fit for vcpkg's binary caching, and it is easier to configure than vcpkg's GitHub Package binary caching integration. GitHub provides a few different tools to manage your Actions caches, which include REST APIs and an extension to the `gh` command line tool, so that you can optimize caches for your workflow. vcpkg's integration with GitHub Actions cache is through the `x-gha` binary source provider.

### <a name="caching-nuget"></a> The GitHub Packages NuGet cache provider

GitHub Packages make it possible for a repository to publish binary artifacts for public or private use. Beyond hosting storage for published packages, GitHub Packages supports a variety of package management tools by acting as a package registry. vcpkg can use the NuGet registry interface to GitHub Packages as a cache for vcpkg binary artifacts, by using the `nuget` binary source provider. This integration with GitHub Packages is not as straightforward as the GitHub Actions cache integration and management of the cached binaries is more difficult, making the GitHub Actions cache a better option for most users.

## <a name="dependency-graph"></a> The GitHub dependency graph

[!INCLUDE [experimental](../includes/experimental.md)]

> [!TIP]
> We are actively developing this feature and would like to hear your feedback. Let us know if you have any thoughts about the current functionality or any desired functionality by emailing [vcpkg@microsoft.com](mailto:vcpkg@microsoft.com) or by filing an issue in [vcpkg](https://github.com/microsoft/vcpkg/issues). If you have feedback on any of the GitHub features that this enables, let us know that too and we'll make sure it gets to the right folks at GitHub.

### About the GitHub dependency graph

The GitHub dependency graph stores the set of dependencies for a repository. Beyond being able to visualize a repository's dependencies, GitHub builds several useful features on top of this data, including dependency review and Dependabot alerts. Learn more at GitHub's documentation on [securing your supply chain](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/about-supply-chain-security).

### vcpkg integration with the GitHub dependency graph

vcpkg has experimental support for populating the GitHub dependency graph from within a GitHub Actions workflow. To enable this feature, make the following changes in your workflow file:

* Set the `VCPKG_FEATURE_FLAGS` environment variable to include the value `dependencygraph`. 
* Set the `GITHUB_TOKEN` environment variable to the value `${{ secrets.GITHUB_TOKEN }}`.
* Give GitHub Actions write access to the contents of your repository by including the following permissions block. This is required to write the dependency graph metadata to your repository. This __will not__ add any commits to your repository or make any other modification to the contents of your repository.

```yaml
permissions:
  contents: write
```

You must enable the GitHub dependency graph in your repository's settings (enabled by default on public repositories). For private repositories, follow the GitHub documentation to [enable the dependency graph](https://docs.github.com/code-security/supply-chain-security/understanding-your-software-supply-chain/configuring-the-dependency-graph#enabling-and-disabling-the-dependency-graph-for-a-private-repository) in order to satisfy this requirement.

### Known limitations


* The version of vcpkg bundled with GitHub Actions runners may not be up to date. Ensure you are using tool version `2022-06-22` or later (merged with [commit SHA 8f3e8d6](https://github.com/microsoft/vcpkg/commit/8f3e8d6e95c7f91edea60c47a7d1414ab5ca9492)).
* Features that depend on the dependency graph, such as Dependabot alerts and Dependabot pull requests, are not yet available. Please let us know if you are interested in those features!

### Example GitHub Actions workflow

> [!NOTE]
> This example assumes that there is a valid `vcpkg.json` manifest that lists some dependent ports. For more information on manifests, see our [documentation on manifest mode](./users/manifests.md).

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