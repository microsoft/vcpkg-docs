---
title: Authenticate to private Git repositories
description: Learn to configure which credentials are used when accessing remote resources with vcpkg.
ms.date: 10/31/2024
ms.topic: concept-article
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
---
# Authenticate to private Git repositories

[Registries](../concepts/registries.md) and [`vcpkg_from_git()`](../maintainers/functions/vcpkg_from_git.md) directly use the Git command line tools to fetch remote resources. Some of these resources may be protected from anonymous access and need authentication or credentials.

## Pre-seed Git credentials

You can pre-seed Git credentials using the [`git credential approve` command](https://git-scm.com/docs/git-credential):

::: zone pivot="shell-powershell"

```PowerShell
"url=https://github.com`npath=Microsoft/vcpkg`nusername=unused`npassword=$MY_PAT`n" | git credential approve
```

::: zone-end
::: zone pivot="shell-cmd"

```cmd
(echo "url=https://github.com"& echo "path=Microsoft/vcpkg"& echo "username=unused"& echo "password=%MY_PAT%") | git credential approve
```

::: zone-end
::: zone pivot="shell-bash"

```sh
echo "url=https://github.com"$'\n'"path=Microsoft/vcpkg"$'\n'"username=unused"$'\n'"password=$MY_PAT"$'\n' | git credential approve
```

::: zone-end

## Bearer auth

For systems which need bearer auth, you can use `git config`:

> [!NOTE]
> You must make these config changes with `--global`

```console
git config --global --unset-all http.<uri>.extraheader
git config --global http.<uri>.extraheader "AUTHORIZATION: bearer <System_AccessToken>"
```

The `<uri>` can be filled in with a variety of options, for example `https://dev.azure.com/MYORG/`. For more details, see the [`git config`](https://git-scm.com/docs/git-config#Documentation/git-config.txt-httplturlgt) documentation.

(Original source in: [Best way to authenticate against a git repository in a build process](https://github.com/Microsoft/azure-pipelines-agent/issues/1601#issuecomment-394511048)).

**Azure DevOps users:** You may need to enable access via [Job authorization scope](/azure/devops/pipelines/process/access-tokens#job-authorization-scope) and reference the repo in your yaml pipeline:

```yaml
resources: 
  repositories:
    - repository: <FRIENDLYNAME>
      type: git
      name: <ORG>/<REPO>
      tag: tags/<TAG>

...

jobs:
 - job: Build
   uses:
     repositories: [<FRIENDLYNAME>]
```

## Pass credentials in an environment variable (not recommended)

Using `VCPKG_KEEP_ENV_VARS` or `VCPKG_ENV_PASSTHROUGH_UNTRACKED`, you can pass credentials in via the environment.

```sh
export VCPKG_KEEP_ENV_VARS=MY_TOKEN_VAR
export MY_TOKEN_VAR=abc123
```

This can then be used in your private ports with the [`vcpkg_from_git()`](../maintainers/functions/vcpkg_from_git.md), [`vcpkg_from_github()`](../maintainers/functions/vcpkg_from_github.md) or [`vcpkg_from_gitlab()`](../maintainers/functions/vcpkg_from_gitlab.md) helpers.

```cmake
# vcpkg-from-git-example/portfile.cmake
set(MY_TOKEN_VAR "")
if(DEFINED ENV{MY_TOKEN_VAR})
    set(MY_TOKEN_VAR "$ENV{MY_TOKEN_VAR}@")
endif()
vcpkg_from_git(
    URLS "https://${MY_TOKEN_VAR}host.com/normal/url/path"
    ...
)
```

```cmake
# vcpkg-from-github-example/portfile.cmake
vcpkg_from_github(
    AUTHORIZATION_TOKEN "$ENV{MY_TOKEN_VAR}"
)
```

For private ports, we recommend using `vcpkg_from_git()` instead of `vcpkg_from_github()/vcpkg_from_gitlab()` and the pre-seeding method above.

## Pass Jenkins gitUsernamePassword credentials

The simplest and most secure option to Git authentication to GitHub from Jenkins is using [GitHub App](https://github.com/jenkinsci/github-branch-source-plugin/blob/master/docs/github-app.adoc) and the following:

```groovy
withCredentials([gitUsernamePassword(credentialsId: 'jenkins-github-app')]) {
  withEnv(['VCPKG_KEEP_ENV_VARS=GIT_ASKPASS']) {
    bat 'cmake'
  }
}
```

This sets the `GIT_ASKPASS` with a path to helper script which responds with git credentials query and instructs `vcpkg` to keep this environment variable. The password is a [GitHub App token](https://github.blog/2021-04-05-behind-githubs-new-authentication-token-formats/) with 1 hour lifetime.
