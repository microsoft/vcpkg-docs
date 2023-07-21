---
title: Configure Git authentication in GitHub Actions
description: Learn to configure Git credentials for private repositories when using vcpkg on GitHub Actions
author: vicroms
ms.author: viromer
ms.date: 7/20/2023
ms.prod: vcpkg
---
# Configure Git authentication in GitHub Actions

When using vcpkg with private registries it is necessary to provide access
credentials to authenticate Git operations executed internally by vcpkg.

In this article we will show you how to authenticate to a private vcpkg registry
hosted on GitHub to use in your GitHub Actions workflows.

## Step 1: Create a Personal Access Token for your private repository

The first step is to create a Personal Access Token (PAT) for your GitHub account.

- Log into GitHub using an account with access to the private repository.
- Go to <https://github.com/settings>
- Click on _Developer Settings_
- Click on _Tokens (classic)_
- Click the _Generate new token (classic)_
- Give your token appropiate description, expiration date, and permissions. Make
  sure that the `repo` permission is enabled, this will give the PAT full access
  to your private repositories.
- Click on _Generate token_
- Copy and paste the generated PAT in a secure location, you won't be able to
  view it again after leaving the current page
- (Optional) If your organization uses SSO, you need to enable it for the PAT

### Step 2: Create a secret to use in your workflow

> !NOTE
> Never store a GitHub Personal Access Token in your repository files

Now that you have a PAT, you need to create a repository secret to use in your workflows.

- Go to your repository's page on GitHub
- Click on the _Settings_ tab
- Expand the  _Secrets and variables_ menu in the _Security_ section
- Click on _Actions_
- Click on _New repository secret_
- Paste your PAT and give the secret an appropiate name, for example: `private_registry_pat`
- Click on _Add secret_

### Step 3: Add Git authentication in your workflow

Now is time to add an authentication step to your workflow, add the following lines
in your workflow's YAML file. This step should run before vcpkg is invoked.

```YAML
    - name: Authenticate Git
      shell: bash
      run: git config --global credential.helper '!f() { echo username=unused; echo password=${{secrets.private_registry_pat}}; }; f'
```
