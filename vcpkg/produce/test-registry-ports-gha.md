---
title: Test your custom registry ports using vcpkg with GitHub Actions
description: This tutorial shows users how to set up Continuous Integration environment for their registry's vcpkg ports using GitHub Actions.
author: vicroms
ms.author: viromer
ms.date: 1/10/2024
ms.topic: tutorial
---

# Test your custom registry ports using vcpkg with GitHub Actions

Once you have set up a custom registry of vcpkg ports, you may want to also add
Continous Integration to validate that all your dependencies can be built
successfully.

The main vcpkg registry at
[Microsoft/vcpkg](<https://github.com/Microsoft/vcpkg>) is tested by the vcpkg
team using Continuous Integration (CI) with Azure DevOps. This ensures that
adding ew packages or updating existing ones does not break consumers.

In this article we'll show you how to set up a CI environment to test the vcpkg
ports in your own registry.

In this article, you'll lear to:

> [!divclass]
>
> * Set up a binary cache and asset cache for your GitHub Actions workflows
> * Set up a workflow to test your registry's ports

## Prerequisites

* A GitHub account
* Your own [vcpkg Git registry](../produce/publish-to-a-git-registry.md)
* Completion of the [binary
  caching](../consume/binary-caching-github-actions-cache.md)
  and [asset caching](../consume/asset-caching.md) tutorials.
* Knowledge about GitHub Actions workflows

## Set up a binary cache and asset cache for your GitHub Actions workflows

Building a large collection of ports is an expensive task both in terms of
computing power and time. We strongly recommend that before engagin in CI for
your ports, you invest in setting up a binary cache and an asset cache for your
GitHub Action workflows.

A binary cache provides the most benefit for CI scenarios by ensuring that
unmodified packages aren't rebuilt on every CI run. An asset cache mirrors
artifacts downloaded for your packages during a run and uses the cached artifact
in subsequent runs; an asset cache can help mitigate issues where upstream is
unreliable for example a broken download URL.

For detailed instructions on how to set up these caches read our [binary
caching](../consume/binary-caching-github-actions-cache.md) and [asset
caching](../consume/asset-caching.md) articles.

## Example: Enable asset and binary caching in a GitHub Actions workflow

```yml
env: 
    VCPKG_ASSET_SOURCES: "clear;x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,{{ secrets.ADO_SAS }},readwrite"
    VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"

steps:
    - name: Enable GitHub Actions Cache backend
        uses: actions/github-script@v6
        with:
        script: |
            core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
            core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');
```

This example shows how to set up a binary cache and asset cache in a GitHub
Actions workflow, you should adapt this snippet to use on your own YAML file.

Breaking down this snippet:

`VCPKG_ASSET_SOURCES` is the environment variable used to configure asset caches
in vcpkg. In this example, it is set to
`x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,{{secrets.ADO_SAS}},readwrite`.
 The `x-azurl` backend instructs vcpkg to use an Azure Artifacts NuGet feed as the
 storage provider. The `x-azurl` is followed by three parameters separated by
 commas (`,`).

* `https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json` is the Nuget feed
  URL.
* `{{secrets.ADO_SAS}}` is a GitHub Actions secret that contains a SAS token to
  authenticate to your NuGet feed.
* `readwrite` sets read and write permissions for the asset cache, this means
  that this asset cache is used to store artifacts as well as to restore
  artifacts from it.

`VCPKG_BINARY_SOURCES` is the environment variable used to configure binary
caches in vcpkg. In this example, it is set to `clear;x-gha,readwrite`; this
enables the GitHub Actions Cache backend for the binary cache. An extra step is
required in your workflow to enable this backend successfully.

The following step should be included as-is in your GitHub Action workflow
steps. This step exports two environment variables required by the `x-gha`
backend to work.

```yml
- name: Enable GitHub Actions Cache backend
  uses: actions/github-script@v6
  with:
  script: |
    core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
    core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');
```

Learn more about how all of these work by reading the documentation on the
[asset cache](../users/assetcaching.md) and [binary
cache](../users/binarycaching.md) features.

## Set up a workflow to test your registry's ports

After you have set up a binary cache and asset cache for your CI environment,
the next step is to set up a workflow to test all your registry's ports. You can
decide whether this workflow runs on a schedule or if it is triggered by new
commits or Pull Requests.

The main vcpkg registry uses the `vcpkg ci` command, which has been tailored to
the vcpkg project's needs and is not intended to remain stable or be used by
consumersof vcpkg. As such, it is not suited to use for testing your own vcpkg
registries. We recommend to follow the steps outlined in this article instead.

### Use a manifest file to include all your ports

Instead of using the `vcpkg ci` command, we suggest using a manifest file to
create a build that depends on all the packages in your registry.

The following example, creates a manifest file to test all the ports in a
ficticious vcpkg registry. Replace the list of dependencies to include all the
ports in your registry and place it in the root of you registry.

`vcpkg.json`

```json
{
  "dependencies": [
    "beicode",
    "beison"
  ]
}
```

Your own ports may have dependencies on the main vcpkg registry or other
third-party registries, in which case you will want to add those registries in a
`vcpkg-configuration.json` file. While vcpkg can resolve packages from the main
registry without having to explicitly add it to the configuration, we strongly
recommend explicitly adding it to the registries list for version control
purposes. This ensures that you have control of the set of underlying port
versions. Check out the [`vcpkg x-update-baseline`
command](../commands/update-baseline.md) to help managing the baseline of your
registries.

`vcpkg-configuration.json`

```json
{
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "42bb0d9e8d4cf33485afb9ee2229150f79f61a1f",
      "packages": "*"
    }
  ]
}
```

Read the [`vcpkg.json`](../reference/vcpkg-json.md) and
[`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md) reference
articles to learn more. And the [manifest mode
documentation](../consume/manifest-mode.md) to learn how these work together.

### Acquire vcpkg in your GitHub Actions workflow

Next, you need to acquire vcpkg to use it in your workflow. Add the following
steps to install vcpkg.

```yml
steps:
- uses: actions/checkout@v4
  with:
    repository: "https://github.com/Microsoft/vcpkg"
    fetch-depth: 0

- name: Bootstrap vcpkg
  run: ${{ github.worksapce }}/vcpkg/bootstrap.sh
  shell: bash
```

Make sure that you set `fetch-depth` to 0 when checking out the vcpkg repository
since you need the full repository's commit history for port versioning to work.

Once this steps are completed you should have a vcpkg executable to work with.

### Run vcpkg install to build your ports

The last step is to tell vcpkg to build all your ports. You may have noticed
that your own registry is absent from the `vcpkg-configuration.json` created a
couple of steps above. The reason is that you want to test the version of the
ports currently in the working directory as opossed to the versions published in
your repository.

To that goal, you need to add your registry's ports as [overlay
ports](../concepts/overlay-ports.md). This is accomplished by setting the
`VCPKG_OVERLAY_PORTS` environment variable to your registry's `ports` directory.

The snippet below shows how to set up your registry's ports as overlay ports and
runs `vcpkg install` in manifest mode to install all of your custom ports.

```yml
- env:
  VCPKG_OVERLAY_PORTS: ${{ github.workspace }}/ports

- name: Build ports
  run: ${{ github.workspace }}/vcpkg/vcpkg install
  shell: bash
```

In this example we assume that the `vcpkg.json` file is created in the root of
your registry's repository.

Putting it all together your workflow's YAML file should look similar to this:

`.github/workflows/test-ports.yml`

```yml
name: Test vcpkg ports

on:
  push:
    branches: ["main"]
  pull_request:
    branches: ["main"]

env:
  VCPKG_ASSET_SOURCES: "clear;x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,{{ secrets.ADO_SAS }},readwrite"
  VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"
  VCPKG_OVERLAY_PORTS: ${{ github.workspace }}/ports

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4
    
    - name: Acquire vcpkg
      uses: actions/checkout@v4
      with:
        repository: "https://github.com/Microsoft/vcpkg"
        fetch-depth: 0

    - name: Bootstrap vcpkg
      run: ${{ github.worksapce }}/vcpkg/bootstrap.sh
      shell: bash

    - name: Enable GitHub Actions Cache backend
      uses: actions/github-script@v6
      with:
      script: |
        core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
        core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');

    - name: Build ports
      run: ${{ github.workspace }}/vcpkg/vcpkg install
      shell: bash
```

This is the basic structure for a CI workflow for your registry's ports. You may
require some extra work to [authenticate to private
repositories](../consume/gha-authentication.md) or to your [NuGet
feeds](../users/binarycaching.md#nuget-credentials).

You may also want to add steps to automate the generation of the `vcpkg.json`
file or a step that verifies that ports newly added to your registry are not
left out of the tests.

## Next steps

The following articles may be useful to you when setting up a CI environment.

* [Authenticate to private Git registries](../consume/gha-authentication.md)
* [Set up a binary cache using GHA
  Cache](../consume/binary-caching-github-actions-cache.md)
* [Set up an asset cache](../consume/asset-caching.md)
* [Binary cache reference](../users/binarycaching.md)
* [Asset cache reference](../users/assetcaching.md)
* [Overlay ports](../concepts/overlay-ports.md)
* [Manifest mode](../concepts/manifest-mode.md)
* [`vcpkg.json`](../reference/vcpkg-json.md)
* [`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md)
* [vcpkg configuration environment variables](../users/config-environment.md)
