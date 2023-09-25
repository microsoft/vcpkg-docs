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

> [!NOTE]
> This tutorial uses NuGet feeds hosted in Azure Artifacts but the same instructions can be used for
> other NuGet feed providers, for example: GitHub Packages, with minimal changes.

vcpkg supports using NuGet package feeds to upload and restore binary packages in a convenient way.

NuGet package feeds have access control capabilities which make them ideal to limit access to resources across your
organization or workgroup. NuGet feeds are supported by several cloud-storage providers, such as,
[Azure Artifacts](<https://www.visualstudio.com/docs/package/nuget/publish>)
and [GitHub package
registry](<https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-nuget-registry>).

In this tutorial, you'll learn how to:

> [!div class="checklist"]
>
> * [Set up an Azure Artifacts NuGet feed](#1---set-up-an-azure-artifacts-nuget-feed)
> * [Add a NuGet source](#2---add-a-nuget-source)
> * [Configure vcpkg to use your NuGet feed](#3---configure-vcpkg-to-use-your-nuget-feed)

## Prerequisites

* A terminal
* An Azure DevOps account
* [NuGet](<https://dist.nuget.org/win-x86-commandline/latest/nuget.exe>)
* [vcpkg](../get_started/setup-vcpkg.md)

## 1 - Set up an Azure Artifacts NuGet feed

Follow the instructions to [set up an Azure Artifacts NuGet
feed](/azure/devops/artifacts/get-started-nuget?view=azure-devops&preserve-view=true).

## 2 - Add a NuGet source

### [`nuget.config`](#tab/nuget-config)

Create a `nuget.config` following the template below, replace `<feed url>` and `<feed name>` with
the proper values for your NuGet feed:

`nuget.config`

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <config>
    <add key="defaultPushSource" value="<feed url>" />
  </config>
  <apiKeys>
    <add
      key="<feed url>"
      value="AzureDevOps" />
  </apiKeys>
  <packageSources>
    <clear />
    <add 
       key="<feed name>"
       value="<feed url>" />
  </packageSources>
</configuration>
```

vcpkg requires that you set a `defaultPushSource` in your `nuget.config` file, use your NuGet feed's
URL as the default source to push binary packages. 

If you're uploading your packages to an Azure Artifacts NuGet feed, use `AzureDevOps` as your
source's API Key. Replace the value with your feed's proper API Key if you have one.

Add the `<clear />` source to ignore other previously configured values. If you want, you can define multiple
sources in this file, use a `<add key="<feed name>" value="<feed url>" />` entry for each source.

### [NuGet CLI](#tab/nuget-cli)

Use the `vcpkg fetch nuget` command to get the NuGet executable vcpkg uses.

```PowerShell
vcpkg fetch nuget
```

Add your NuGet feed as a source

```PowerShell
nuget sources add -Name vcpkg-cache -Source <your feed URL>
nuget setapikey AzureDevOps -Source vcpkg-cache
```

If you're uploading your packages to an Azure Artifacts NuGet feed, use `AzureDevOps` as your
source's API Key. Replace the value with your feed's proper API Key if you have one.

---

## 3 - Configure vcpkg to use your NuGet feed

Set the `VCPKG_BINARY_SOURCES` environment variable as follows:

### [`nuget.config`](#tab/configure-nuget-config)

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear;nugetconfig,<nuget.config path>"
```

Replace `<nuget.config path>` with the path to your `nuget.config` file.

### [NuGet CLI](#tab/configure-nuget-cli)

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear;nuget,<feed URL>"
```

Replace `<feed URL>` with your NuGet feed's URL.

---

And that's it! vcpkg will now upload or restore packages from your NuGet feed in Azure
Artifacts.

## Next steps

> [!div class="nextstepaction"]
> [Authenticate to private NuGet feeds](../users/binarycaching.md#nuget-credentials)
