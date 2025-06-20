---
title: Test your custom registry ports using vcpkg with GitHub Actions
description: This tutorial shows users how to set up continuous integration environment for their registry's vcpkg ports using GitHub Actions.
author: vicroms
ms.author: viromer
ms.date: 5/1/2025
ms.topic: how-to

#customer intent: As an advanced vcpkg user, I want to add continuous integration to test the vcpkg ports in my custom Git registry
---

# Test your custom registry ports using vcpkg with GitHub Actions

Once you have set up a custom registry of vcpkg ports, you may want to add
Continuos Integration (CI) to validate that all your dependencies can be built
successfully.

The main vcpkg registry at [Microsoft/vcpkg](<https://github.com/Microsoft/vcpkg>) is tested by the vcpkg team using Azure
DevOps Pipelines. This ensures that adding new packages or updating existing ones does not break consumers.

In this article, we show you how to set up a CI environment to test the vcpkg
ports in your own registry.

In this article, you'll learn to set up a CI workflow for your registry running in GitHub Actions:

> [!divclass]
>
> * Set up a binary cache and asset cache for your GitHub Actions workflows
> * Set up a workflow to test your registry's ports

## Prerequisites

* A GitHub account
* A [vcpkg Git registry](../produce/publish-to-a-git-registry.md)
* Read the [binary caching](../consume/binary-caching-github-packages.md) tutorial
* Read the [asset caching](../consume/asset-caching.md) tutorial

## Set up a binary cache and asset cache for your GitHub Actions workflows

Testing a large collection of ports is an expensive task both in terms of
time and computing power. We strongly recommend that before engaging CI for
your ports, you invest in setting up a binary cache and an asset cache for your
GitHub Action workflows.

A binary cache provides the most benefit for CI scenarios by ensuring that
unmodified packages aren't rebuilt on every CI run. An asset cache mirrors
artifacts downloaded for your packages during a run and uses the cached
artifacts in subsequent runs. It can also help mitigate issues where the
upstream repository is unreliable: for example, a broken download URL.

For detailed instructions on how to set up these caches read our [binary
caching](../consume/binary-caching-github-packages.md) and [asset
caching](../consume/asset-caching.md) articles.

## Example: Enable asset and binary caching in a GitHub Actions workflow

```yml
env:
  FEED_URL: https://nuget.pkg.github.com/<OWNER>/index.json

steps:
  - name: Install vcpkg ports
    run: "${{ github.workspace }}/vcpkg/vcpkg install"
    env: 
      X_VCPKG_ASSET_SOURCES: "clear;x-azurl,https://my.domain.com/container,{{ secrets.SAS }},readwrite"
      VCPKG_BINARY_SOURCES: "clear;nuget,https://nuget.pkg.github.com/<OWNER>/index.json,readwrite"
```

This example shows how to set up a binary cache and asset cache in a GitHub Actions workflow. You should adapt this snippet 
to use on your own workflow's YAML file.

Breaking down this snippet:

`X_VCPKG_ASSET_SOURCES` is the environment variable used to configure asset caches
in vcpkg. In this example, it is set to
`x-azurl,https://my.domain.com/container,{{secrets.SAS}},readwrite`.
 The `x-azurl` backend instructs vcpkg to use an Azure Storage container as the
 storage provider. The `x-azurl` is followed by three parameters separated by
 commas (`,`).

* `https://my.domain.com/container` is a storage container URL.
* `{{secrets.SAS}}` is a GitHub Actions secret variable that contains a SAS token to
  authenticate to the storage container.
* `readwrite` sets read and write permissions for the asset cache. This means
  that this asset cache is used to store artifacts as well as to restore
  artifacts from it.

`VCPKG_BINARY_SOURCES` is the environment variable used to configure binary
caches in vcpkg. In this example, it is set to `clear;x-gha,readwrite`. This
enables the GitHub Actions Cache backend for the binary cache. An extra step is
required in your workflow to enable this backend successfully.

The following step should be included as-is in your GitHub Action workflow
steps. This step exports two environment variables required by the `x-gha`
backend to work and should be run before any task that involves vcpkg.

```yml
- name: Enable GitHub Actions Cache backend
  uses: actions/github-script@v7
  with:
  script: |
    core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
    core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');
```

Learn more about how all of these work by reading the documentation on the
[asset cache](../users/assetcaching.md) and [binary
cache](../reference/binarycaching.md) features.

## Set up a workflow to test your registry's ports

After you have set up a binary cache and asset cache for your CI environment,
the next step is to set up a workflow to test all your registry's ports. You can
decide whether this workflow runs on a schedule or if it is triggered by new
commits or pull requests.

The main vcpkg registry uses the `vcpkg ci` command, which has been tailored to
the vcpkg project's needs and is not intended to remain stable or be used by
consumers of vcpkg. As such, it is not suited to use for testing your own vcpkg
registries. We recommend to follow the steps outlined in this article instead.

### Use a manifest file to include all your ports

Instead of using the `vcpkg ci` command, we suggest using a manifest file to
create a build that depends on all the packages in your registry.

The following example, creates a manifest file to test all the ports in a
hypothetical vcpkg registry. Replace the list of dependencies to include all the
ports in your registry and place it in the root of your repository.

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
third-party registries, in which case, you need to add those registries in a
`vcpkg-configuration.json` file. While vcpkg can resolve packages from the main
registry without additional configuration, we strongly recommend that you explicitly
add it to the registries list for version control purposes.
This ensures that you have control of the set of underlying port
versions. Check out the [`vcpkg x-update-baseline`
command](../commands/update-baseline.md) to help managing the baseline of your
registries.

`vcpkg-configuration.json`

```json
{
  "default-registry": null,
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "42bb0d9e8d4cf33485afb9ee2229150f79f61a1f",
      "packages": ["*"]
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
    path: "vcpkg"

- name: Bootstrap vcpkg
  run: "${{ github.workspace }}/vcpkg/bootstrap-vcpkg.sh"
  shell: bash
```

Once these steps are completed you should have a vcpkg executable to work with.

### Run vcpkg install to build your ports

The last step is to tell vcpkg to build all your ports. You may have noticed
that your own registry is absent from the `vcpkg-configuration.json` created a
couple of steps above. The reason is that you want to test the version of the
ports currently in the working directory as opposed to the versions published in
your repository.

To that goal, you need to add your registry's ports as [overlay
ports](../concepts/overlay-ports.md) by setting the `VCPKG_OVERLAY_PORTS`
environment variable to your registry's `ports` directory.

The snippet below shows how to set up your registry's ports as overlay ports and
runs `vcpkg install` in manifest mode to install all of your custom ports.

```yml
  - name: some vcpkg task
    run: "${{ github.workspace }}/vcpkg/vcpkg install"
    env: 
      X_VCPKG_ASSET_SOURCES: "clear;x-azurl,https://my.domain.com/container,{{ secrets.SAS }},readwrite"
      VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"
      VCPKG_OVERLAY_PORTS: "${{ github.workspace }}/ports"
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

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v4
    
    - name: Acquire vcpkg
      uses: actions/checkout@v4
      with:
        repository: "Microsoft/vcpkg"
        path: vcpkg

    - name: Bootstrap vcpkg
      run: "${{ github.workspace }}/vcpkg/bootstrap-vcpkg.sh"
      shell: bash

    - name: Enable GitHub Actions Cache backend
      uses: actions/github-script@v7
      with:
        script: |
          core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
          core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');

    - name: Build ports
      run: ${{ github.workspace }}/vcpkg/vcpkg install
      env:
        X_VCPKG_ASSET_SOURCES: "clear;x-azurl,https://your.domain.com/container,${{ secrets.SAS }},readwrite"
        VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"
        VCPKG_OVERLAY_PORTS: "${{ github.workspace }}/ports"
      shell: bash
```

This is the basic structure for a CI workflow to test your registry's ports. You may
require some extra work to [authenticate to private
repositories](../consume/gha-authentication.md) or to your [NuGet
feed](../reference/binarycaching.md#nuget-credentials).

You may also want to add steps to automate the generation of the `vcpkg.json`
file or a step that verifies that ports newly added to your registry are not
left out of the tests.

## Next steps

The following articles may be useful to you when setting up a CI environment.

* [Authenticate to private Git registries](../consume/gha-authentication.md)
* [Set up an asset cache](../consume/asset-caching.md)
* [Binary cache reference](../users/binarycaching.md)
* [Asset cache reference](../users/assetcaching.md)
* [Overlay ports](../concepts/overlay-ports.md)
* [Manifest mode](../concepts/manifest-mode.md)
* [`vcpkg.json`](../reference/vcpkg-json.md)
* [`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md)
* [vcpkg configuration environment variables](../users/config-environment.md)
