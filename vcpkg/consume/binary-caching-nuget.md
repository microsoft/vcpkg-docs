---
title: "Tutorial: Set up a vcpkg binary cache using a NuGet feed"
description: This tutorial shows how to set up a vcpkg binary cache using a NuGet feed as the backend
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.topic: tutorial
ms.date: 09/11/2023
---

# Tutorial: Set up a vcpkg binary cache using a NuGet feed

vcpkg supports using NuGet package feeds to upload and restore binary packages in a convenient way.

NuGet package feeds have access control capabilities which make them ideal to limit access to resources across your
organization or workgroup. NuGet feeds are supported by several cloud-storage providers, such as,
[Azure Artifacts](<https://www.visualstudio.com/docs/package/nuget/publish>)
and [GitHub package registry](<https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-nuget-registry>).

In this tutorial, you'll learn how to:

> [!div class="checklist"]
>
> * [Set up an Azure Artifacts NuGet feed]
> * [Add a NuGet source]
> * [Configure vcpkg to use your NuGet feed]

## Prerequisites

* A terminal
* An Azure DevOps account
* [NuGet](<https://dist.nuget.org/win-x86-commandline/latest/nuget.exe>)
* [vcpkg](../get_started/setup-vcpkg.md)

## 1 - Set up an Azure Artifacts NuGet feed

Follow the instructions to [set up an Azure Artifacts NuGet
feed](/azure/devops/artifacts/get-started-nuget?view=azure-devops&preserve-view=true).

## 2 - Add a NuGet source

Follow the instructions to connect to your Azure Artifacts Nuget feed using NuGet.exe to obtain your
feed's source URL.

If you don't have NuGet installed, you can use the `vcpkg fetch nuget` command to get a NuGet executable.

```PowerShell
vcpkg fetch nuget
```

2 - Add your NuGet feed as a source

```PowerShell
nuget sources add -Name vcpkg-cache -Source <your feed URL>
nuget setapikey AzureDevOps -Source vcpkg-cache
```

## 3 - Configure vcpkg to use your NuGet feed

Set the `VCPKG_BINARY_SOURCES` environment variable as follows:

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear;nuget,<your feed URL>"
```

And that's it! vcpkg will now upload or restore packages from your NuGet feed in Azure
Artifacts.

## Next steps

> [!div class="nextstepaction"]
> [Authenticate to private NuGet feeds](../users/binarycaching.md#nuget-credentials)
