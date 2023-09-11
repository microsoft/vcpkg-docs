---
title: Tutorial: Set up a NuGet-backed binary cache
description: This tutorial shows how to set up a vcpkg binary cache using a NuGet feed as the backend
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.topic: tutorial
ms.date: 09/11/2023
---

# Tutorial: Set up a NuGet-backed binary cache

vcpkg supports using NuGet package feeds to upload and restore binary packages in a convenient way.

NuGet package feeds have access control capabilities which make them ideal to limit access to resources across your
organization or workgroup. NuGet feeds are supported by several cloud-storage providers, such as,
[Azure Artifacts](<https://www.visualstudio.com/docs/package/nuget/publish>)
and [GitHub package registry](<https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-nuget-registry>).

In this tutorial, you learn how to:

> [!div class="checklist"]
>
> * [Set up an Azure Artifacts NuGet feed]
> * [Provide credentials for your NuGet feed]
> * [Configure vcpkg to use your NuGet feed]

If you don’t have an Azure DevOps subscription, create a free trial account...
<!-- TODO: Add the free account information if it exists -->

## Prerequisites

* Azure DevOps account
* [NuGet](<https://dist.nuget.org/win-x86-commandline/latest/nuget.exe>)
* [vcpkg](TODO: link to "installing vcpkg")

## Set up an Azure Artifacts NuGet fee

TODO: Add introduction sentence(s)
[Include a sentence or two to explain only what is needed to complete the procedure.]
TODO: Add ordered list of procedure steps
1. Step 1
1. Step 2
1. Step 3

## Provide credentials for your NuGet feed

TODO: Add introduction sentence(s)
[Include a sentence or two to explain only what is needed to complete the procedure.]
TODO: Add ordered list of procedure steps
1. Step 1
1. Step 2
1. Step 3

## Configure vcpkg to use your NuGet feed

TODO: Add introduction sentence(s)
[Include a sentence or two to explain only what is needed to complete the procedure.]
TODO: Add ordered list of procedure steps
1. Step 1
1. Step 2
1. Step 3


<!---Code requires specific formatting. Here are a few useful examples of
commonly used code blocks. Make sure to use the interactive functionality
where possible.

For the CLI or PowerShell based procedures, don't use bullets or
numbering.

Here is an example of a code block for Java:

```java
cluster = Cluster.build(new File("src/remote.yaml")).create();
...
client = cluster.connect();
```

or a code block for Azure CLI:

```azurecli-interactive 
az vm create --resource-group myResourceGroup --name myVM --image win2016datacenter --admin-username azureuser --admin-password myPassword12
```

or a code block for Azure PowerShell:

```azurepowershell-interactive
New-AzureRmContainerGroup -ResourceGroupName myResourceGroup -Name mycontainer -Image mcr.microsoft.com/windows/servercore/iis:nanoserver -OsType Windows -IpAddressType Public
```
-->

<!-- 8. Clean up resources ------------------------------------------------------------------------

Required: To avoid any costs associated with following the tutorial procedure, a Clean up resources (H2) should come just before Next step or Related content (H2)

If there is a follow-on Tutorial that uses the same resources, make that option clear so that a reader doesn't need to recreate those resources. 

-->

<!-- Use this exact H2 -->
## Clean up resources

If you're not going to continue to use this application, delete
<resources> with the following steps:

1. From the left-hand menu...
2. ...click Delete, type...and then click Delete

<!-- 9. Next step/Related content ------------------------------------------------------------------------ 

Optional: You have two options for manually curated links in this pattern: Next step and Related content. You don't have to use either, but don't use both.
  - For Next step, provide one link to the next step in a sequence. Use the blue box format
  - For Related content provide 1-3 links. Include some context so the customer can determine why they would click the link. Add a context sentence for the following links.

-->

## Next step

TODO: Add your next step link(s)

> [!div class="nextstepaction"]
> [Write concepts](article-concept.md)

<!-- OR -->

## Related content

TODO: Add your next step link(s)

- [Write concepts](article-concept.md)

<!--
Remove all the comments in this template before you sign-off or merge to the main branch.
-->