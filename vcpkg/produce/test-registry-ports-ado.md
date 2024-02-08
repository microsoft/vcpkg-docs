---
title: Test your custom registry ports using vcpkg with Azure DevOps
description: This tutorial shows users how to set up continuous integration environment for their registry's vcpkg ports using Azure DevOps
author: vicroms
ms.author: viromer
ms.date: 1/10/2024
ms.topic: how-to

#customer intent: As an advanced vcpkg user, I want to add continuous integration to test the vcpkg ports in my custom Git registry
---

# Test your custom registry ports using vcpkg with Azure DevOps

Once you have set up a custom registry of vcpkg ports, you may want to add
continous integration to validate that all your dependencies can be built
successfully.

The main vcpkg registry at
[Microsoft/vcpkg](<https://github.com/Microsoft/vcpkg>) is tested by the vcpkg
team using a continuous integration (CI) pipeline with Azure DevOps. This
ensures that adding new packages or updating existing ones does not break consumers.

In this article, we show you how to set up a CI environment to test the vcpkg
ports in your own registry.

In this article, you'll learn to:

> [!divclass]
>
> * Set up a binary cache and asset cache for your Azure DevOps pipeline
> * Set up a pipeline to test your registry's ports

## Prerequisites

* An Azure DevOps account
* Your own [vcpkg Git registry](../produce/publish-to-a-git-registry.md)
* Completion of the [binary
  caching](../consume/binary-caching-nuget.md)
  and [asset caching](../consume/asset-caching.md) tutorials.
* Knowledge about ADO pipelines

## Set up a binary cache and asset cache for your Azure DevOps pipelines

Building a large collection of ports is an expensive task both in terms of
time and computing power. We strongly recommend that before engaging CI for
your ports, you invest in setting up a binary cache and an asset cache for your
Azure DevOps pipelines.

A binary cache provides the most benefit for CI scenarios by ensuring that
unmodified packages aren't rebuilt on every CI run. An asset cache mirrors
artifacts downloaded for your packages during a run and uses the cached
artifacts in subsequent runs. It can also help mitigate issues where the
upstream repository is unreliable: for example, a broken download URL.

For detailed instructions on how to set up these caches read our [binary
caching](../consume/binary-caching-nuget.md) and [asset
caching](../consume/asset-caching.md) articles.

## Example: Enable asset and binary caching in an Azure DevOps pipeline

```yml
variables:
  - name: VCPKG_ASSET_SOURCES
    value: "clear;x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,,readwrite"
  - name: VCPKG_BINARY_SOURCES
    value: "clear;nuget,https://my.domain.com/vcpkgBinaryCache/nuget/v3/index.json,readwrite"

steps: 
- task: NuGetAuthenticate@1
```

This example shows how to set up a binary cache and asset cache in an Azure
DevOps pipeline. You should adapt this snippet to use on your own pipeline's
YAML file.

Breaking down this snippet:

`VCPKG_ASSET_SOURCES` is the environment variable used to configure asset caches
in vcpkg. In this example, it is set to
`x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,,readwrite`.
 The `x-azurl` backend instructs vcpkg to use an Azure Artifacts NuGet feed as the
 storage provider. The `x-azurl` is followed by three parameters separated by
 commas (`,`).

* `https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json` is the Nuget feed
  URL.
* The second argument is intentionally left empty. Instead of providing a SAS
  token for the NuGet feed, the `NugetAuthenticate@1` task is used to
  authenticate to your ADO NuGet feeds.
* `readwrite` sets read and write permissions for the asset cache. This means
  that this asset cache is used to store artifacts as well as to restore
  artifacts from it.

`VCPKG_BINARY_SOURCES` is the environment variable used to configure binary
caches in vcpkg. In this example, it is set to
`clear;nuget,https://my.domain.com/vcpkgBinaryCache/nuget/v3/index.json,readwrite`.
This enables the NuGet backend for the binary cache using the NuGet feed at
`https://my.domain.com/vcpkgBinaryCache/nuget/v3/index.json`. Some additional
steps may be required to authenticate to your NuGet feed, read the
[tutorial](../consume/binary-caching-nuget.md#provide-authentication-credentials)
for instructions to set up NuGet authentication with ADO.

The following task should be added as-is in your pipeline in order to
authenticate to your Azure Artifacts NuGet feeds.

```yml
- task: NuGetAuthenticate@1
```

If you're using a different host for your NuGet feeds read the documentation on
how to [authenticate
NuGet](../consume/binary-caching-nuget.md#provide-authentication-credentials).

Learn more about how all of these work by reading the documentation on the
[asset cache](../users/assetcaching.md) and [binary
cache](../users/binarycaching.md) features.

## Set up a pipeline to test your registry's ports

After you have set up a binary cache and asset cache for your CI environment,
the next step is to set up a pipeline to test all your registry's ports. You can
decide whether this pipeline runs on a schedule or if it is triggered by new
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
      "packages": "*"
    }
  ]
}
```

Read the [`vcpkg.json`](../reference/vcpkg-json.md) and
[`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md) reference
articles to learn more. And the [manifest mode
documentation](../consume/manifest-mode.md) to learn how these work together.

### Acquire vcpkg in your Azure DevOps pipeline

If you use vcpkg as a submodule in your project, you can obtain the vcpkg
repository in the step to checkout your own project as shown below.

```yml
steps:
- checkout: self
  submodules: true
```

Otherwise, you need to acquire vcpkg to use it in your pipeline. Add the following
steps to clone the vcpkg repository.

```yml
resources:
  repositories:
    - repository: vcpkgRepo
      type: github
      name: Microsoft/vcpkg
      endpoint: MyGitHubServiceConnection

steps:
  - checkout: vcpkgRepo
```

In order to clone the vcpkg repository you need to define a repository
resource for your pipeline. The snippet below shows how to add the vcpkg
repository as a resource, you will need to set up a [Service
Connection](https://learn.microsoft.com/en-us/azure/devops/pipelines/library/service-endpoints?view=azure-devops&tabs=yaml)
to connect your pipeline to GitHub.

After checking out the vcpkg repository either as a submodule or by cloning it
from GitHub, you need to run vcpkg's bootstrap script.

```yml
steps:
  - script: vcpkg/bootstrap-vcpkg.sh
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
variables:
  - name: VCPKG_ASSET_SOURCES
    value: "clear;x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,,readwrite"
  - name: VCPKG_BINARY_SOURCES
    value: "clear;nuget,https://my.domain.com/vcpkgBinaryCache/nuget/v3/index.json,readwrite"
  - name: VCPKG_OVERLAY_PORTS
    value: $(Build.Repository.LocalPath)/ports

steps:
- script: $(Build.Repository.LocalPath)/vcpkg/vcpkg install
```

In this example we assume that the `vcpkg.json` file is created in the root of
your registry's repository and that the vcpkg repository is added as a submodule.

Putting it all together your pipelines's YAML file should look similar to this:

`.azure-pipelines/test-ports.yml`

```yml
trigger:
- main

pr:
-main

pool:
  vmImage: ubuntu-latest

variables:
  - name: VCPKG_ASSET_SOURCES
    value: "clear;x-azurl,https://my.domain.com/vcpkgAssetCache/nuget/v3/index.json,,readwrite"
  - name: VCPKG_BINARY_SOURCES
    value: "clear;nuget,https://my.domain.com/vcpkgBinaryCache/nuget/v3/index.json,readwrite"
  - name: VCPKG_OVERLAY_PORTS
    value: $(Build.Repository.LocalPath)/ports

steps:
  - task: NuGetAuthenticate@1

  - checkout: self
    submodules: true

  - script: $(Build.Repository.LocalPath)/vcpkg/bootstrap-vcpkg.sh

  - script: $(Build.Repository.LocalPath)/vcpkg//vcpkg install
    displayName: Test vcpkg ports
```

This is the basic structure for a CI pipeline to test your registry's ports. You
may require some extra work to [authenticate to private
repositories](../consume/gha-authentication.md) or to your [NuGet
feed](../users/binarycaching.md#nuget-credentials).

You may also want to add steps to automate the generation of the `vcpkg.json`
file or a step that verifies that ports newly added to your registry are not
left out of the tests.

## Next steps

The following articles may be useful to you when setting up a CI environment.

* [Authenticate to private Git registries](../consume/gha-authentication.md)
* [Set up a binary cache using NuGet
  Cache](../consume/binary-caching-nuget.md)
* [Set up an asset cache](../consume/asset-caching.md)
* [Binary cache reference](../users/binarycaching.md)
* [Asset cache reference](../users/assetcaching.md)
* [Overlay ports](../concepts/overlay-ports.md)
* [Manifest mode](../concepts/manifest-mode.md)
* [`vcpkg.json`](../reference/vcpkg-json.md)
* [`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md)
* [vcpkg configuration environment variables](../users/config-environment.md)
