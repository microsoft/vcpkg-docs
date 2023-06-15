---
title: Integrating with GitHub dependency graph
description: Record the set of resolved libraries with the GitHub dependency graph
author: michaelbprice
ms.author: miprice
ms.date: 05/22/2023
---

# Integrating with the GitHub dependency graph

[!INCLUDE [experimental](./includes/experimental.md)]

> [!TIP]
> We are actively developing this feature and would like to hear your feedback. Let us know if you have any thoughts about the current functionality or any desired functionality by emailing [vcpkg@microsoft.com](mailto:vcpkg@microsoft.com) or by filing an issue in [vcpkg-docs](https://github.com/microsoft/vcpkg-docs/issues). If you have feedback on any of the GitHub features that this enables, let us know that too and we'll make sure it gets to the right folks at GitHub.

## About the GitHub dependency graph

The GitHub [dependency graph](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/about-the-dependency-graph) stores the set of dependencies for a repository. Beyond just being able to visualize what a repository's dependencies are, GitHub builds several useful features on top of this data, including [dependency review](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/about-dependency-review) and [Dependabot alerts](https://docs.github.com/en/code-security/dependabot/dependabot-alerts/about-dependabot-alerts).

## vcpkg integration with the GitHub dependency graph

vcpkg has experimental support for populating the GitHub dependency graph from within a GitHub Action workflow. In order to enable this feature, you must include the following in your workflow file:

* Set the `dependencygraph` feature flag to the environment variable `VCPKG_FEATURE_FLAGS`.
* Set the `GITHUB_TOKEN` environment variable with the GitHub workflow variable `${{ secrets.GITHUB_TOKEN }}`.
* Enable write access to the contents of your repository by including the following permissions block. This is because it must write dependency graph metadata to your repository. This __will not__ add any commits to your repository or otherwise modify the contents of your repository (despite the permission name).

```yaml
permissions:
  contents: write
```

Additionally, you must have the GitHub dependency graph enabled. If your repository is a public repository, it is enabled by default. If it is a private repository, follow the [GitHub documentation to enable the dependency graph](https://docs.github.com/en/code-security/supply-chain-security/understanding-your-software-supply-chain/configuring-the-dependency-graph#enabling-and-disabling-the-dependency-graph-for-a-private-repository) in order to satisfy this requirement.

## Known limitations

The following are known limitations in the current implementation. __Even though we know of these, if they affect the value for you, we'd still like to hear from you so that we can prioritize what we need to fix and how we should fix it.__

* The version of vcpkg that is bundled with GitHub Actions runners does not have this feature yet. You must use invoke the correct version of vcpkg that supports this feature
* Features that depend on the dependency graph, such as Dependbot alerts and Dependabot pull requests, are not yet available. Please let us know if you are interested in those features!

## Example GitHub Actions workflow

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