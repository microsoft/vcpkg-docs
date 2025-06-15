---
title: "Authenticate to third-party services"
description: Explain authentication methods for third-party services consumed by vcpkg
author: vicroms
ms.author: viromer
ms.date: 03/03/2025
ms.topic: tutorial
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---
# Authenticate to third-party services

This article explains how to engage vcpkg features that require you to authenticate to third-party sevices.

## <a name="asset-cache-azure-blob-storage"></a> Asset cache provider: Azure Blob Storage

The [`x-azurl`](../users/assetcaching.md#x-azurl) expects an Azure Blob Storage URL endpoint and a Shared Access Signature
Token (SAS Token) to authenticate when uploading and retrieving asset artifacts.

Read the Azure documentation to [learn more about SAS Tokens](/rest/api/storageservices/delegate-access-with-shared-access-signature).

### Prerequisites

* An active [Azure account](https://azure.microsoft.com/free/cognitive-services/). If you don't have one, you can [create
  a free account](https://azure.microsoft.com/free/).
* An [Azure Blob Storage account](https://portal.azure.com/#create/Microsoft.StorageAccount-ARM).
* A [container](/azure/storage/blobs/storage-quickstart-blobs-portal#create-a-container) to store and organize the files
  within your storage account.

### 1 - Create a Shared Access Signature Token in the Azure portal

1. Open the [Azure portal](https://portal.azure.com/#home) and navigate to your container.
    1. From the portal home click on Storage Accounts,
    2. Select the account for your asset cache and expand it,
    3. on the right side menu, find your container under Data Storage > Containers and click  it to open its own page.
2. Create an Account key SAS Token for your container.
    1. In the container page, click on Settings > Shared access tokens
    2. Select Account Key as the signing method
    3. Configure the token permissions, ensure that the token has read and write permissions enabled.
    4. Set the expiration for your token, by default, SAS Tokens have a 30-days lifetime, but you can increase it.
    5. Click on Generate SAS token and URL
    6. Copy the Blob SAS token and SAS URL and store them somewhere secure. This information is only displayed once when 
       creating the token and cannot be viewed again after you leave this page.

### 2 - Configure your asset cache to use your Azure Blob Storage container and SAS token

To use your Azure Blob Container as an asset caching provider, you need to add a configuration string to the [`X_VCPKG_ASSET_SOURCES`
environment variable](../users/config-environment.md#x_vcpkg_asset_sources).

The configuration string follows the format [`x-azurl,<storage URL>,<SAS token>[,<permissions>]`](../users/assetcaching.md#x-azurl).
Where `<storage URL>` is your Azure Storage Account URL, which you an obtain from the SAS URL; `<SAS token>` is the token
generated in the previous step; and `<permissions>` can be one of `read`, `write`, or `readwrite`.

```powershell
set STORAGE_SAS_TOKEN="sp=rw&s...%3D"
set X_VCPKG_ASSET_SOURCES="x-azurl,mystorageaccount.blob.core.windows.net,${STORAGE_SAS_TOKEN},readwrite"
```

Read the [asset caching documentation](../concepts/asset-caching.md) to learn more about configuring asset caching sources.

## Binary cache provider: Azure Artifacts + Universal Packages

**TBD preamble**

### Prerequisites

* [Azure CLI](/cli/azure/install-azure-cli).
* An [Azure DevOps project](/azure/devops/organizations/projects/create-project).
* An [Azure Artifacts feed](/azure/devops/artifacts/start-using-azure-artifacts#create-a-new-feed)

### 1 - Authenticate using Azure CLI

> [!IMPORTANT]
> The Azure CLI executable needs to be part of your `PATH` variable so that vcpkg can find it and use it.

Read the instructions to [authenticate using the Azure command-line tool](/cli/azure/authenticate-azure-cli) using the
method that better suits your use case.

Example: Interactive authentication

```PowerShell
az login
```

vcpkg uses the credentials stored by Azure CLI to access your Azure Artifacts feed.

### 2 - Configure your binary cache to use your Azure Artifacts Universal Packages feed

Add a configuration string to the [`VCPKG_BINARY_SOURCES`](../users/config-environment.md#vcpkg_binary_sources) environment
variable.

The configuration string follows the format [`x-az-universal,<organization>,<project>,<feed>[,<permissions>]`](../reference/binarycaching.md#x-azuniversal).
Where `<organization>` and `<project>` refer to your Azure DevOps project, you can leave the `<project>` argument empty to
use an organization level feed;  `<feed>` is your Azure Artiracts feed's name. The `<permissions>` argument is optional,
and can be one of `read`, `write`, or `readwrite`.

```powershell
set VCPKG_BINARY_SOURCES="x-az-universal,vcpkg,demo,example-cache,readwrite"
```

## Binary cache provider: GitHub Actions cache

For Continuous Integration (CI) workflows running in GitHub Actions, using the GitHub Actions cache is a convenient way
to enable binary caching without requiring third-party storage.

### Prerequisites

- A project running a GitHub Actions workflow

### 1 - Export the GitHub Actions Cache environment variable

vcpkg needs the `ACTIONS_CACHE_URL` and `ACTIONS_RUNTIME_TOKEN` to be exported as environment variables in your workflow.

Copy the following lines before invoking vcpkg in your workflow file:

```YAML
- name: Export GitHub Actions cache environment variables
  uses: actions/github-script@v7
  with:
    script: |
      core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
      core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');
```

### 2 - Configure your binary cache to use GitHub Actions Cache

Set the [`VCPKG_BINARY_SOURCES`](../users/config-environment.md#vcpkg_binary_sources) environment variable in your workflow
as follows:

```YAML
env: 
    VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"
```

The [`x-gha`](../reference/binarycaching.md#github-actions-cache) provider only takes the optional `<permissions>` argument,
its value can be one of`read`, `write`,or `readwrite`.

## Binary cache provider: NuGet

NuGet package feeds are supported by many cloud-storage providers, like [Azure Artifacts](<https://www.visualstudio.com/docs/package/nuget/publish>)
and [GitHub Package registry](<https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-nuget-registry>).
They also provide access control capabilities, which make them suitable for limiting access within your organization or
work group.

### Prerequisites

- A NuGet packages feed

### 1 - Add `nuget.exe` to PATH

vcpkg acquires a copy of the `nuget.exe` executable to use for binary caching operations.

Run the `vcpkg fetch nuget` command to obtain the location of the NuGet executable and add it to path.

Example of `vcpkg fetch nuget` output:

```powershell
vcpkg fetch nuget
C:\dev\vcpkg\downloads\tools\nuget-6.10.0-windows\nuget.exe
```

Modify your `PATH` environment variable to add the directory containing `nuget.exe`.

Example in PowerShell:

```powershell
$env:PATH="C:\dev\vcpkg\downloads\tools\nuget-6.10.0-windows;$env:PATH"
```

### <a name="nuget-add-source"></a> 2 - Add your private feed as a NuGet packages source

> ![NOTE]
> Be aware that encrypted passwords are only supported on Windows. Moreover, they can only be decrypted on the same machine
> and by the same user who originally encrypted them.

Add your feed as a NuGet source and include its authentication credentials.

```PowerShell
nuget.exe sources add -Name <feed-name> -Source <feed-url> -UserName <username> -Password <password>
```

Set `<username>` and `<password>` to your authentication credentials and `<feed-url>` to point at your feed's URL.

> ![WARNING]
> Storing passwords in clear text is strongly discouraged. For more information on managing credentials securely, refer
> to the [security best practies for consuming packages from private feeds](/nuget/consume-packages/consuming-packages-authenticated-feed#security-best-practices-for-managing-credentials).

On non-Windows you may need to add the `-StorePasswordInClearText` option to the command-line.

## Binary cache provider: Nuget + Azure Artifacts

### Create a feed

1. Sign in to your Azure DevOps organization and navigate to your project.
2. Open the "Artifacts" tab.
3. Click the "+ Create Feed" button.
4. Configure your feed's name, visibility and scope. We recommend unchecking the "Include packages from common public sources" for vcpkg binary caching feeds.
5. Click the "Create" button.

:::image type="content" source="../resources/create-nuget-feed.png" alt-text="Screenshot of the Create new feed window":::

### Connect to a feed

1. Sign in to your Azure DevOps organization and navigate to your project.
2. Open the "Artifacts" tab
3. Select your feed from the dropwdown menu.
4. Select "Connect to Feed", and then select Visual Studio from the left pane.
5. Copy the Name and Source values from Machine setup

:::image type="content" source="../resources/connect-to-nuget-feed.png" alt-text="Screenshot of the Machine setup to connect with Visual Studio":::

### Add your feed as a NuGet source

#### Method 1: Create a Personal Access Token to authenticate

1. Sign in to your Azure DevOps organization
2. From your home page, click the Settings icon and then Personal Access Tokens
    :::image type="content" source="../resources/ado-options-pat.png" alt-text="Screenshot of the User Settings menu with the Personal Access Tokens option highlighted":::
3. Click the "+New Token" button
4. Set your token's name, expiration date, and scopes. In the Packages scope, make sure to give the token readwrite access. Then click "Create".
   :::image type="content" source="../resources/ado-create-pat.png" alt-text="Screenshot of the Create a new personal access tokens window":::
5. Copy the token and store it in a secure location. NOTE: The token cannot be displayed again after you close this window.
  :::image type="content" source="../resources/ado-copy-pat.png" alt-text="Screenshot of the window showing the newly created personal access token":::
6. Follow the steps to [add a private feed as a NuGet packages source](#nuget-add-source)
7. Use your Personal Access Token as the password to authenticate to your NuGet feed.

### Set an API Key for your feed

Azure Artifacts requires that packages include an API Key. However, the key can be any arbitrary string.

