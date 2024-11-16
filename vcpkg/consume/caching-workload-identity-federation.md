---
title: "Tutorial: Set up caching using Azure Storage and Azure DevOps"
description: This tutorial shows how to set up a vcpkg cache using an Azure Storage account authenticated using Workload Identity Federation.
author: bion
ms.author: bion
ms.topic: tutorial
ms.date: 11/12/2024
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---

# Tutorial: Set up caching using Azure Storage and Azure DevOps

::: zone pivot="shell-cmd"
>[!NOTE]
> These instructions are impractical with 'CMD', please choose 'PowerShell' or 'Bash' above this message.
::: zone-end

vcpkg supports using Azure Storage containers to upload and restore assets and binary packages. However,
configuring pre-shared long-lived SAS tokens for authentication purposes requires error-prone
manual secrets rotation. [Workload Identity Federation](/entra/workload-id/workload-identity-federation)
offers a mechanism where Azure DevOps takes care of secrets rotation and management for you
that can be used with vcpkg.

In this tutorial, you'll learn how to:

> [!div class="checklist"]
>
> * [Create an Entra Managed Identity](#1---create-an-entra-managed-identity)
> * [Set up Workload Identity Federation between Azure Storage and Azure DevOps](#2---set-up-workload-identity-federation-with-azure-devops)
> * [Assign Managed Identity Permissions to the Storage Account Container](#3---assign-managed-identity-permissions-to-the-storage-account-container)
> * [Generate a SAS token, and use it with Caching](#4---generate-a-sas-token-and-use-it-with-caching)

> [!WARNING]
> These instructions grant permission to act as the managed identity to the pipelines where it is
> used. Moreover, vcpkg caches provide a mechanism where a compromised build may be able to cause
> other builds to use compromised assets from caches.
>
> You should not allow machines which run untrusted 3rd party build requests to write to caches
> that are used for official builds.

## Prerequisites

::: zone pivot="shell-cmd"

>[!NOTE]
> These instructions are impractical with 'CMD', please choose 'PowerShell' or 'Bash' at the top
> of the page.

::: zone-end

::: zone pivot="shell-powershell"

* A terminal with [Azure PowerShell](/powershell/azure/install-azps-windows) installed
* [vcpkg](../get_started/get-started.md#1---set-up-vcpkg)
* An Azure Storage subscription and container where you wish binaries to be stored

::: zone-end

::: zone pivot="shell-bash"

* A terminal with [Azure CLI](/cli/azure/install-azure-cli-windows) installed
* [vcpkg](../get_started/get-started.md#1---set-up-vcpkg)
* An Azure Storage subscription and container where you wish binaries to be stored

::: zone-end

## 1 - Create an Entra Managed Identity

Skip this step if you already have configured a
[Managed Identity](/entra/identity/managed-identities-azure-resources/overview) you wish to use.

In the Azure Portal, go to the page for a Resource Group in which you wish for the managed identity
to be created, and press "+ Create". In the search box, enter "User Assigned Managed Identity".
Choose "User Assigned Managed Identity" from "Microsoft" and choose "Create ->
User Assigned Managed Identity". Name the identity, press "Review and Create", and "Create". For
purposes of this documentation, the identity was named `vcpkg-docs-identity`.

## 2 - Set Up Workload Identity Federation with Azure DevOps

>[!NOTE]
> This step is intended to be identical to the instructions in Azure DevOps' documentation:
> https://learn.microsoft.com/azure/devops/pipelines/release/configure-workload-identity?view=azure-devops&tabs=managed-identity#create-a-service-connection-for-managed-identity-authentication-in-azure-devops

In the Azure DevOps portal for the project in which you wish to run Pipelines, select
"Project Settings" in the lower left corner. Then select "Service Connections" on the left.
Then choose "New Service Connection" on the right. Choose the "Azure Resource Manager" radio button,
and press Next. Select "Workload identity federation (manual)" and press next. Name the
service connection and press next. For purposes of this documentation, the service connection was
named `vcpkg-docs-identity-connection`. At this point, Azure DevOps should be showing an issuer
and subject identifier.

In another tab, go to the Azure Portal navigate to the managed identity created in step 1. On the
left select Settings/Federated Credentials, and select 'Add Credential'. In the drop down, select
'Other'. Copy the 'Issuer URL' and 'Subject Identifier' from Azure DevOps into the form. Give the
federated credential a name, and press 'Add'. For purposes of this documentation, the name used was
`azure-devops-credential`.

## 3 - Assign Managed Identity Permissions to the Storage Account Container

Skip this step if you have already granted your managed identity permissions to your Storage Account
Container.

In the Azure Portal, go to the page for the Storage Account to use. On the left, select
Data Storage/Containers, and choose the container you wish to use. This should open the Azure Portal
to the properties for that specific container. On the left select "Access Control (IAM)", and choose
"Add -> Add Role Assignment". Select "Azure Blob Data Reader" for read only access, or
"Azure Blob Data Contributor" for read/write access, and press Next. Then, select the
"Managed Identity" radio button, and click + Select Members. Select the managed identity you created
in step 1. Then select 'Review and Assign' twice.

Then, return to the page for the storage account container, and again on the left select
"Access Control (IAM)", and choose "Add -> Add Role Assignment". Select "Storage Blob Delegator" and
press Next. Then, select the "Managed Identity" radio button, and click + Select Members. Select the
managed identity you create in step 1. Then select 'Review and Assign' twice.

## 4 - Generate a SAS token, and use it with Caching

::: zone pivot="shell-cmd"
>[!NOTE]
> These instructions are impractical with 'CMD', please choose 'PowerShell' or 'Bash' at the top
> of the page.
::: zone-end
::: zone pivot="shell-powershell, shell-bash"
In the yml where you wish to invoke vcpkg in Azure DevOps, use the AzureCLI@2 task to mint a SAS
token and invoke vcpkg, for example:
::: zone-end
::: zone pivot="shell-powershell"
```yaml
steps:
- task: AzureCLI@2
  displayName: 'Build with vcpkg'
  inputs:
    azureSubscription: 'vcpkg-docs-identity-connection' # This is the name of the service connection
    scriptType: 'pscore'                                # you set up in Step 2.
    scriptLocation: 'inlineScript'
    inlineScript: |
      # Determine an end date for the SAS token. This should be based on the maximum length of
      # time you expect your pipeline to take.
      $current = Get-Date -AsUtc
      $end = $current.AddHours(2)
      # Replace container-name and storage-account-name with the values from step 3.
      # Replace 'rcl' permissions with 'rc' or 'rclw' as appropriate for the permissions you wish to use.
      # See https://learn.microsoft.com/rest/api/storageservices/create-service-sas#permissions-for-a-directory-container-or-blob
      # for a list of acceptable permissions.
      $ctx = New-AzStorageContext -StorageAccountName storage-account-name -UseConnectedAccount
      $sas = New-AzStorageContainerSASToken -Context $ctx -Name container-name -Permission rclw -ExpiryTime $end
      # Azure PowerShell changed whether it emits a ? at the beginning of the SAS token in different versions:
      if ($sas.StartsWith("?")) {
        $sas = sas.Substring(1)
      }
      # Now use the SAS token for asset caching; make sure to replace storage-account-name and container-name again:
      $env:X_VCPKG_ASSET_SOURCES = "x-azurl,https://storage-account-name.blob.core.windows.net/container-name,$sas,readwrite"
      # ... or for binary caching:
      vcpkg install --binarysource "clear;x-azblob,https://storage-account-name.blob.core.windows.net/container-name,$sas,readwrite"
```
::: zone-end

::: zone pivot="shell-bash"
```yaml
steps:
- task: AzureCLI@2
  displayName: 'Build with vcpkg'
  inputs:
    azureSubscription: 'vcpkg-docs-identity-connection' # This is the name of the service connection
    scriptType: 'bash'                                  # you set up in Step 2.
    scriptLocation: 'inlineScript'
    inlineScript: |
      # Determine an end date for the SAS token. This should be based on the maximum length of
      # time you expect your pipeline to take.
      export end=`date -u -d "2 hours" '+%Y-%m-%dT%H:%MZ'
      # Replace container-name and storage-account-name with the values from step 3.
      # Replace 'rcl' permissions with 'rc' or 'rclw' as appropriate for the permissions you wish to use.
      # See https://learn.microsoft.com/cli/azure/storage/container?view=azure-cli-latest#az-storage-container-generate-sas-optional-parameters
      # for a list of acceptable permissions.
      # The important arguments that trigger workload identity federation here are '--as-user' and '--auth-mode login'
      export sas=`az storage container generate-sas --name container-name --account-name storage-account-name --as-user --auth-mode login --https-only --permissions rclw --expiry $end -o tsv`
      # Now use the SAS token for asset caching; make sure to replace storage-account-name and container-name again:
      export X_VCPKG_ASSET_SOURCES=x-azurl,https://storage-account-name.blob.core.windows.net/container-name,$sas,readwrite
      # ... or for binary caching:
      vcpkg install --binarysource "clear;x-azblob,https://storage-account-name.blob.core.windows.net/container-name,$sas,readwrite"
```
::: zone-end

## Next steps

Here are other tasks to try next:

* [Set up a local binary cache](binary-caching-local.md)
