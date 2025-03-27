---
title: "Authenticate to NuGet package feeds"
description: Explain authentication methods for accessing NuGet package feeds with vcpkg
author: vicroms
ms.author: viromer
ms.date: 03/03/2025
ms.topic: tutorial
---
# Authenticate to NuGet package feeds

This article explains how to engage vcpkg features that require you to authenticate to NuGet package feeds.

## Binary cache provider: NuGet

NuGet package feeds are supported by many cloud-storage providers, like [Azure Artifacts](<https://www.visualstudio.com/docs/package/nuget/publish>)
and [GitHub Package registry](<https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-nuget-registry>).
They also provide access control capabilities, which make them suitable for limiting access within your organization or
work group.

## Prerequisites

- A NuGet packages feed

## 1 - Add `nuget.exe` to PATH

vcpkg acquires a copy of the `nuget.exe` executable to use for binary caching operations.

Run the `vcpkg fetch nuget` command to obtain the location of the NuGet executable and add it to path.

### Example of `vcpkg fetch nuget` output

```PowerShell
vcpkg fetch nuget
C:\dev\vcpkg\downloads\tools\nuget-6.10.0-windows\nuget.exe
```

Modify your `PATH` environment variable to add the directory containing `nuget.exe`.

### Example setting `PATH` in a PowerShell session

```PowerShell
$env:PATH="$env:PATH;C:\dev\vcpkg\downloads\tools\nuget-6.10.0-windows"
```

## 2 - Add your private feed as a NuGet packages source <a name="nuget-add-source"></a>

### Method 1: Preload credentials with the NuGet CLI

This method creates a NuGet packages source that stores your feed URL and the necessary credentials.

> [!NOTE]
> Be aware that encrypted passwords are only supported on Windows. Moreover, they can only be decrypted on the same machine
> and by the same user who originally encrypted them.

1. Add your feed as a NuGet source and include its authentication credentials.

```PowerShell
nuget.exe sources add -Name <feed-name> -Source <feed-url> -UserName <username> -Password <password>
```

Replace `<username>` and `<password>` with your authentication credentials.  Replace `<feed-url>` with your feed's URL.

> [!WARNING]
> Storing passwords in clear text is strongly discouraged. For more information on managing credentials securely, refer
> to the [security best practies for consuming packages from private
> feeds](/nuget/consume-packages/consuming-packages-authenticated-feeds#security-best-practices-for-managing-credentials).

On non-Windows you may need to add the `-StorePasswordInClearText` option to the command-line.

### Method 2: Create a `nuget.config` file <a name="nuget-config">

You can create a `nuget.config` file to configure NuGet packages sources. Use the template below as a base to add your own
NuGet feeds as package sources.

#### Template `nuget.config`

> [!WARNING]
> Storing passwords in clear text is strongly discouraged. Please note that encrypted passwords are only supported on Windows.
> Furthermore, they can only be decrypted when used on the same machine and by the same user who originally encrypted them.
> For more information on managing credentials securely, refer to the [security best practies for consuming packages
> from private feeds](/nuget/consume-packages/consuming-packages-authenticated-feeds#security-best-practices-for-managing-credentials).

> [!TIP]
> If a non-encrypted password is passed for `Password` the error message [**"The parameter is incorrect" will occur**](<https://github.com/NuGet/Home/issues/3245>).

```XML
<?xml version="1.0" encoding="utf-8"/>
<configuration>
  <config>
    <add key="defaultPushSource" value="<feed url>">
  </config>
  <packageSources>
    <clear/>
    <add key="myNugetFeed" value="<feed url>" />
  </packageSources>
  <packageSourcesCredentials>
    <!-- must match a key defined in the packageSources section -->
    <myNugetFeed>
      <add key="Username" value="%NUGET_USERNAME%" />
      <add key="Password" value="%NUGET_PASSWORD%" />
    </myNugetFeed>
  </packageSourcesCredentials>
</configuration>
```

Replace `<feed url>` with your NuGet feed's URL. You can reference environment variables by surronding them with `%`, for
example `%NUGET_USERNAME%` and `%NUGET_PASSWORD%`.

> [!WARNING]
> Storing passwords in clear text is strongly discouraged.

On non-Windows, you may need to use `ClearTextPassword` instead of `Password`.

Read the [NuGet config file documentation](/nuget/reference/nuget-config-file) to learn more.

## 3 - (Optional) Provide an API Key

Some NuGet package feed providers require that you push packages to the feed using an API key. For example, GitHub Packages
requires a GitHub Personal Access Token as its API Key; while Azure Artifacts needs that the API key is not-empty, but can
have any arbitrary value.

### Method 1: Set the API Key using the NuGet CLI

Use the `nuget setapikey` command to set the API key for a specific feed URL.

#### Example of setting API Key in PowerShell

```PowerShell
.$(vcpkg fetch nuget) setapikey $GITHUB_PAT_TOKEN -Source https://nuget.pkg.github.com/$GITHUB_OWNER/index.json
```

### Method 2: Set the API key using a NuGet config file

If you're using a `nuget.config` file to add your package feed sources, you can set their API keys in the same file by adding
an [`<apiKeys>`](/nuget/reference/nuget-config-file#apikeys) section.

### Example of setting API Key using a `nuget.config` file

```XML
<apiKeys>
  <add key="<feed url>" value="%API_KEY%">
</apiKeys>
```

## 4 - Configure binary caching to use your NuGet feed

Set the `VCPKG_BINARY_SOURCES` environment variable as follows:

```PowerShell
$env:VCPKG_BINARY_SOURCES="nuget,<feed url>,readwrite"
```

If you're using a `nuget.config` file, use the following configuration instead:

```PowerShell
$env:VCPKG_BINARY_SOURCES="nugetconfig,<path to nuget.config>,readwrite"
```

The `readwrite` permission is optional and can be changed to `read` or `write` only.

## Next steps

Here are other tasks to try next:

- [Change the default binary cache location](binary-caching-default.md)
- [Set up a local binary cache](binary-caching-local.md)
- [Set up a binary cache in your GitHub Actions workflow using GitHub Packages]binary-caching-github-packages.md)
- [Set up a binary cache in your GitHub Actions workflow using GitHub Actions Cache](binary-caching-github-actions-cache.md)
