---
title: "Tutorial: Authenticate vcpkg Git-based registries in GitHub Actions"
description: Learn to configure Git credentials for private repositories when using vcpkg on GitHub Actions
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: tutorial
#CustomerIntent: As an enterprise vcpkg user I want to authenticate to private Git registries so that I can use them as Git-based repositories
---
# Tutorial: Authenticate vcpkg Git-based registries in GitHub Actions

When using vcpkg with private registries it is necessary to provide access
credentials to authenticate Git operations executed internally by vcpkg.

In this tutorial you will learn how to authenticate a private Git repository to use in your GitHub
Actions workflow.

> [!div class="checklist"]
> * [Create a Personal Access Token for your private repository](#1---create-a-personal-access-token-for-your-private-repository)
> * [Create a GitHub Actions secret to use in your workflow](#2---create-a-github-actions-secret-to-use-in-your-workflow)
> * [Authenticate to private Git repositories in your workflow](#3---authenticate-to-private-git-repositories-in-your-workflow)


## Prerequisites

* A GitHub account
* A private vcpkg Git-registry hosted on GitHub
* vcpkg

## 1 - Create a Personal Access Token for your private repository

Follow the [instructions on GitHub to generate a fine-grained Personal Access Token
(PAT)](<https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens#creating-a-fine-grained-personal-access-token>)
and grant it permission to access the private repository where your vcpkg Git registry is hosted.

## 2 - Create a GitHub Actions secret to use in your workflow

> [!NOTE]
> Never store a GitHub Personal Access Token in your repository files

The next step is to follow [GitHub's instructions to store the PAT as a secret available in your
GitHub Actions
workflows](<https://docs.github.com/en/actions/security-guides/using-secrets-in-github-actions#creating-secrets-for-a-repository>).

## 3 - Authenticate to private Git repositories in your workflow

Now is time to add an authentication step to your workflow, use the following template
in your workflow's YAML file. Replace `PRIVATE_REPO_URL` with the URL of your private repository,
for example: `https://github.com/Microsoft/secret-vcpkg-registry`; replace `PRIVATE_REGISTRY_PAT`
with the name of the secret variable you created in the previous step.

This workflow step must run before any other workflow step that invokes vcpkg.

```YAML
  - name: Authenticate private registries
    shell: bash
    run: git config --global credential.PRIVATE_REPO_URL.helper '!f() { echo username=unused; echo password=${{secrets.PRIVATE_REGISTRY_PAT }}; }; f'
```

### Example with multiple private repositories

The following example shows how to provide Git credentials for multiple private
repositories using the `credential.<url>.helper` pattern.

```YAML
    - name: Authenticate private registries
      shell: bash
      run: |
          git config --global credential.https://github.com/vcpkg/private_registry.helper '!f() { echo username=unused; echo password=${{secrets.private_registry_pat}}; }; f'
          git config --global credential.https://github.com/vcpkg/secret_registry.helper `!f() { echo username=unused; echo password=${{secrets.secret_registry_pat}}; }; f'
```
