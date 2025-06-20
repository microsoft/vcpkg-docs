---
title: "Tutorial: Set up a vcpkg binary cache using a NuGet feed"
description: This tutorial shows how to set up a vcpkg binary cache using a NuGet feed as the backend
author: vicroms
ms.author: viromer
ms.topic: tutorial
ms.date: 7/16/2024
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---

# Tutorial: Set up a vcpkg binary cache using a NuGet feed

> [!NOTE]
> This tutorial uses a NuGet feed hosted in Azure Artifacts but the same instructions can be used for
> other NuGet feed providers, for example: GitHub Packages, with some changes.

vcpkg supports using NuGet package feeds to upload and restore binary packages in a convenient way.

NuGet package feeds have access control capabilities which make them ideal to limit access to resources across your
organization or workgroup. NuGet feeds are supported by several cloud-storage providers, such as,
[Azure Artifacts](<https://www.visualstudio.com/docs/package/nuget/publish>)
and [GitHub package
registry](<https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-nuget-registry>).

In this tutorial, you'll learn how to:

> [!div class="checklist"]
>
> * [Set up a NuGet feed](#1---set-up-a-nuget-feed)
> * [Add a NuGet source](#2---add-a-nuget-source)
> * [Configure vcpkg to use your NuGet feed](#3---configure-vcpkg-to-use-your-nuget-feed)

## Prerequisites

::: zone pivot="shell-cmd, shell-powershell"

* A terminal
* [vcpkg](../get_started/get-started.md#1---set-up-vcpkg)
* A NuGet package feed, or if you don't, an Azure DevOps account to follow along

::: zone-end

::: zone pivot="shell-bash"

* A terminal
* [vcpkg](../get_started/get-started.md#1---set-up-vcpkg)
* A NuGet package feed, or if you don't, an Azure DevOps account to follow along
* The `mono` package installed in your system

::: zone-end

## 1 - Set up a NuGet feed

Skip this step if you already have an existing NuGet packages feed.

Follow the instructions to [set up an Azure Artifacts NuGet
feed](/azure/devops/artifacts/get-started-nuget?view=azure-devops&preserve-view=true).

You can also use any other NuGet packages feed provider of your choice.

## 2 - Add a NuGet source

::: zone pivot="shell-bash"
> [!NOTE]
> On Linux you need `mono` to execute `nuget.exe`. You can install `mono` using your distribution's
> system package manager.
::: zone-end

vcpkg acquires its own copy of the `nuget.exe` executable that it uses during binary caching
operations. This tutorial uses the vcpkg-acquired `nuget.exe`. The `vcpkg fetch nuget` command
outputs the location of the vcpkg-acquired `nuget.exe`, downloading the executable if necessary.

Run the following command to add your NuGet feed as a source, replace `<feed name>` with any name of
your choosing and `<feed url>` with the URL to your NuGet feed.

::: zone pivot="shell-powershell"

```PowerShell
.$(vcpkg fetch nuget) sources add -Name <feed name> -Source <feed url>
```

::: zone-end
::: zone pivot="shell-cmd"

Execute the command below to fetch the path to the NuGet executable:

```console
vcpkg fetch nuget
```

This will provide an output that looks something like `C:\path\to\nuget.exe`. Make a note of this path.
Using the path obtained from the previous step, run the following command:

```console
C:\path\to\nuget.exe sources add -Name <feed name> -Source <feed url>
```

::: zone-end
::: zone pivot="shell-bash"

```bash
mono `vcpkg fetch nuget | tail -n 1` sources add -Name <feed name> -Source <feed url>
```

::: zone-end

### Provide an API key

Some providers require that you push your NuGet packages to the feed using an API key. For example,
GitHub Packages requires a GitHub PAT (Personal Access Token) as the API key; if you're using Azure
Artifacts the API key is `AzureDevOps` instead.

Use the following command to set the API key for all the packages pushed to your NuGet feed, replace
`<apiKey>` with your feed's API key.

::: zone pivot="shell-powershell"

```PowerShell
.$(vcpkg fetch nuget) setapikey <apikey> -Source <feed url>
```

::: zone-end
::: zone pivot="shell-cmd"

Execute the command below to fetch the path to the NuGet executable:

```console
vcpkg fetch nuget
```

This will provide an output that looks something like `C:\path\to\nuget.exe`. Make a note of this path.
Using the path obtained from the previous step, run the following command:

```console
C:\path\to\nuget.exe setapikey <apikey> -Source <feed url>
```

::: zone-end
::: zone pivot="shell-bash"

```bash
mono `vcpkg fetch nuget | tail -n 1` setapikey <apiKey> -Source <feed url>
```

::: zone-end

### Provide authentication credentials

Your NuGet feed may require authentication to let you download and upload packages. If that's the
case you can provide credentials by adding them as parameters to the `nuget sources add` command.

For example:

```Console
nuget sources add -Name my-packages -Source https://my.nuget.feed/vcpkg-cache/index.json -UserName myusername -Password mypassword -StorePasswordInClearText
```

Some providers like Azure Artifacts may require different authentication methods, read the 
[Authenticate to private NuGet feeds](../reference/binarycaching.md#nuget-credentials) article to learn
more.

### Use a `nuget.config` file

Alternatively, you can use a `nuget.config` file to configure your NuGet sources, following the
template below:

`nuget.config`

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <config>
    <add key="defaultPushSource" value="<feed url>" />
  </config>
  <apiKeys>
    <add key="<feed url>" value="<apikey>" />
  </apiKeys>
  <packageSources>
    <clear />
    <add  key="<feed name>" value="<feed url>" disableTLSCertificateValidation="false" />
  </packageSources>
  <packageSourceCredentials>
    <<feed name>>
      <add key="Username" value="<username>" />
      <add key="ClearTextPassword" value="<password>" />
    </<feed name>>
  </packageSourceCredentials>
</configuration>
```

Example `nuget.config` file :

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <config>
    <add key="defaultPushSource" value="https://contoso.org/packages/" />
  </config>
  <apikeys>
    <add key="https://contoso.org/packages/" value="encrypted_api_key" />
  </apikeys>
  <packageSources>
    <clear />
    <add key="Contoso" value="https://contoso.org/packages/" disableTLSCertificateValidation="false" />
  </packageSources>
  <packageSourceCredentials>
    <Contoso>
      <add key="Username" value="user" />
      <add key="ClearTextPassword" value="..." />
    </Contoso>
  </packageSourceCredentials>
</configuration>
```

vcpkg requires that you set a `defaultPushSource` in your `nuget.config` file, use your NuGet feed's
URL as the default source to push binary packages. 

If you're uploading your packages to an Azure Artifacts NuGet feed, use `AzureDevOps` as your
source's API Key by running `nuget setApiKey AzureDevOps -Source <feed url> -ConfigFile <path to nuget.config>`.
Otherwise, replace the value with your feed's proper API Key if you have one.

If you are using a self-signed certificate you may need to disable TLS certificate validation. This option
is only available through the use of a _nuget.config_ file.

Add the `<clear />` source to ignore other previously configured values. If you want, you can define multiple
sources in this file, use a `<add key="<feed name>" value="<feed url>" />` entry for each source.

## 3 - Configure vcpkg to use your NuGet feed

Set the `VCPKG_BINARY_SOURCES` environment variable as follows:

::: zone pivot="shell-powershell"

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear;nuget,<feed url>,readwrite"
```

If you're using a `nuget.config` file, instead do:

```PowerShell
$env:VCPKG_BINARY_SOURCES="clear;nugetconfig,<path to nuget.config>"
```

::: zone-end
::: zone pivot="shell-cmd"

```console
set "VCPKG_BINARY_SOURCES=clear;nuget,<feed url>,readwrite"
```

If you're using a `nuget.config` file, instead do:

```console
set "VCPKG_BINARY_SOURCES=clear;nugetconfig,<path to nuget.config>"
```

::: zone-end
::: zone pivot="shell-bash"

> [!NOTE]
> Setting `VCPKG_BINARY_SOURCES` using the `export` command will only affect the current shell
> session. To make this change permanent across sessions, you'll need to add the `export` command to
> your shell's profile script (e.g., `~/.bashrc` or `~/.zshrc`).

```bash
export VCPKG_BINARY_SOURCES="clear;nuget,<feed url>,readwrite"
```

If you're using a `nuget.config` file, instead do:

```bash
export VCPKG_BINARY_SOURCES="clear;nugetconfig,<path to nuget.config>"
```

::: zone-end

And that's it! vcpkg will now upload or restore packages from your NuGet feed.

## Next steps

> [!div class="nextstepaction"]
> [Authenticate to private NuGet feeds](../reference/binarycaching.md#nuget-credentials)

Here are other tasks to try next:

* [Change the default binary cache location](binary-caching-default.md)
* [Set up a local binary cache](binary-caching-local.md)
* [Set up a binary cache in your GitHub Actions workflow using GitHub Packages](binary-caching-github-packages.md)
