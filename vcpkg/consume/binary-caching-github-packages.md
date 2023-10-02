---
title: "Tutorial: Set up a vcpkg binary cache using GitHub Packages in a GitHub Actions workflow"
description: This tutorial shows how to set up a vcpkg binary cache using a NuGet feed hosted in GitHub Packages inside a GitHub Actions workflow
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.topic: tutorial
ms.date: 09/27/2023
#CustomerIntent: As a vcpkg user, I want to setup binary caching in my GitHub Actions workflow using GitHub Packages as the binary cache storage
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
---
# Tutorial: Set up a vcpkg binary cache using GitHub Packages in a GitHub Actions workflow

> [!NOTE]
> This tutorial uses NuGet feeds hosted in GitHub Packages but the same instructions can be used for
> other NuGet feed providers, for example: Azure Artifacts, with minimal changes.

GitHub Packages offers a convenient repository for your NuGet binary packages produced by vcpkg.
In this tutorial, we show you how to set up a binary cache in your GitHub Actions workflow that uses
GitHub Packages as the remote storage.

In this tutorial, you'll learn how to:

> [!div class="checklist"]
>
> * [Provide a GitHub Personal Access Token](#1---provide-a-github-personal-access-token)
> * [Bootstrap vcpkg](#2---bootstrap-vcpkg)
> * [Set up required environment variables](#3---set-up-required-environment-variables)
> * [Add GitHub Packages as a NuGet source](#4---add-github-packages-as-a-nuget-source)

## Prerequisites

* A code editor
* A GitHub repository using GitHub Actions
* A project using vcpkg

## 1 - Provide a GitHub Personal Access Token

Follow GitHub's instructions to [generate a classic Personal Access Token (PAT)](<https://docs.github.com/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens#creating-a-personal-access-token-classic>) for your repository, make
sure to give it the `packages:write` and `packages:read` permissions.

Then add your GitHub PAT as a [secret available in your repository's GitHub Action's workflows](<https://docs.github.com/actions/security-guides/using-secrets-in-github-actions>). In
this tutorial we assume that you name your secret as `GH_PACKAGES_TOKEN`.

The built-in PAT provided by `GITHUB_TOKEN` only has the `packages:read` permission, you can use
it if you plan to have a read-only binary cache in your workflow.

## 2 - Bootstrap vcpkg

vcpkg acquires its own copy of the `nuget.exe` executable that it uses during binary caching
operations. This tutorial uses the vcpkg-acquired `nuget.exe`.

Add a step to bootstrap vcpkg in your workflow:

::: zone pivot="os-windows"

```YAML
- name: Bootstrap vcpkg
  shell: pwsh
  run: ${{ github.workspace }}/vcpkg/bootstrap-vcpkg.bat
```

::: zone-end

::: zone pivot="os-linux, os-macos"

```YAML
- name: Bootstrap vcpkg
  shell: bash
  run: ${{ github.workspace }}/vcpkg/bootstrap-vcpkg.sh
```

::: zone-end

You may need to replace the location of the vcpkg bootstrap script with the correct one for your
workflow, this tutorial assumes that vcpkg is located in a `vcpkg` folder in the root of the repository.

## 3 - Set up required environment variables

Add the following environment variables to your workflow file (replace `<OWNER>` with your GitHub's username or organization's name):

```YAML
env: 
  USERNAME: <OWNER>
  VCPKG_EXE: ${{ github.workspace }}/vcpkg/vcpkg
  FEED_URL: https://nuget.pkg.github.com/<OWNER>/index.json
  VCPKG_BINARY_SOURCES: "clear;nuget,https://nuget.pkg.github.com/<OWNER>/index.json,readwrite"
```

Youo may need to replace the value of `VCPKG_EXE` with the location of the vcpkg executable
generated in the [bootstrap vcpkg](#2---bootstrap-vcpkg) step.

In this step you are configuring `VCPKG_BINARY_SOURCES` to use your GitHub Packages feed as a binary
caching source, read the [binary caching reference](../users/binarycaching.md) to learn more.

## 4 - Add GitHub Packages as a NuGet source

The `vcpkg fetch nuget` command outputs the location of the vcpkg-acquired `nuget.exe`, downloading
the executable if necessary.

Add the following step in your GitHub Actions workflow file:

::: zone pivot="os-windows"

```YAML
- name: Add NuGet sources
  shell: pwsh
  run: |
    .$(${{ env.VCPKG_EXE }} fetch nuget) `
      sources add `
      -Source "${{ env.FEED_URL }}" `
      -StorePasswordInClearText `
      -Name GitHubPackages `
      -UserName "${{ env.USERNAME }}" `
      -Password "${{ secrets.GH_PACKAGES_TOKEN }}"
    .$(${{ env.VCPKG_EXE }} fetch nuget) `
      setapikey "${{ secrets.GH_PACKAGES_TOKEN }}" `
      -Source "${{ env.FEED_URL }}"
```

::: zone-end

::: zone pivot="os-linux, os-macos"

On Linux, you need `mono` to execute `nuget.exe`. GitHub Actions runners using Ubuntu come with `mono`
preinstalled. Otherwise, you can install `mono` using your distribution's system package manager.

```YAML
- name: Add NuGet sources
  shell: bash
  env: 
    VCPKG_EXE: ${{ github.workspace }}/vcpkg/vcpkg
    USERNAME: <OWNER>
    FEED_URL: https://nuget.pkg.github.com/<OWNER>/index.json
  run: |
    mono `${{ env.VCPKG_EXE }} fetch nuget | tail -n 1` \
      sources add \
      -Source "${{ env.FEED_URL }}" \
      -StorePasswordInClearText \
      -Name GitHubPackages \
      -UserName "${{ env.USERNAME }}" \
      -Password "${{ secrets.GH_PACKAGES_TOKEN }}"
    mono `${{ env.VCPKG_EXE }} fetch nuget | tail -n 1` \
      setapikey "${{ secrets.GH_PACKAGES_TOKEN }}" \
      -Source "${{ env.FEED_URL }}"
```

::: zone-end

You may need to replace `GH_PACKAGES_TOKEN` with the correct secret name you gave to the PAT
generated in the [provide a GitHub Personal Access Token step](#1---provide-a-github-personal-access-token).

And that's it! vcpkg will now upload or restore packages from your NuGet feed hosted on GitHub
Packages inside your GitHub Acions workflow.

## Next steps

> [!div class="nextstepaction"]
> [Authenticate to private NuGet feeds](../users/binarycaching.md#nuget-credentials)

Here are other tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a local binary cache](binary-caching-local.md)
* [Set up a binary cache using a NuGet feed](binary-caching-nuget.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Actions Cache](binary-caching-github-actions-cache.md)
