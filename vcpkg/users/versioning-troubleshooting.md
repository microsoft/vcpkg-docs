---
title: Troubleshoot versioning and feature selection issues
description: Troubleshooting guide for common versioning issues
author: JavierMatosD
ms.author: javiermat
ms.date: 01/10/2024
ms.topic: troubleshooting-general
---

# Versioning troubleshooting guide

This guide is intended for users experiencing issues with [versioning](./versioning.md).

## <a name="inspect-versions-file"></a> Inspecting the version file for a port

> [!NOTE]
> The process described below is meant to work for ports from the [vcpkg registry](<https://github.com/Microsoft/vcpkg>). See our [registry](../concepts/registries.md) documentation to learn how the versioning database is implemented in custom registries.

To inspect the versions database of a specific port:  
1. Navigate to the `vcpkg/versions` directory.
1. Locate the port's folder:
   - Find the folder corresponding to the first letter of the port. For example, for `fmt` open the folder named `f-`.
1. Open the ports version file:
   - Locate the JSON file with the same name of the port. For example, the `fmt` versions file is named `fmt.json.`

The port's version file contains a list of available versions with details like version tags and their corresponding [Git tree-object hash](https://git-scm.com/book/en/v2/Git-Internals-Git-Objects#_tree_objects). This information is required by vcpkg to retrieve specific port versions. Only versions contained in this list are available to use in your manifest files.

For more details on versioning, see our reference documentation:
- [versioning concepts](./versioning.concepts.md)
- [versioning](./versioning.md)

For more details on using a manifest, see [manifest](../concepts/manifest-mode.md)

## <a name="non-existent-version"></a> Cause: Requesting a non-existent version of a package
<!-- 
Steps to reproduce:
1. Open or create a vcpkg.json file for your project.
2. Specify a dependency with a version that does not exist in the vcpkg repository. For example, set the version of the fmt library to a non-existent version like "version>=": "100.0.0".
3. vcpkg install
-->

When a version specified in the manifest file does not exist in the vcpkg version database, vcpkg fails to resolve the dependency and produces an error message resembling the following:

```console
error: no version database entry for fmt at 100.0.0
Available versions:
  10.1.1
  10.1.0
  10.0.0
  9.1.0#1
  9.1.0
  9.0.0
  8.1.1#2
  8.1.1#1
  ...
See `vcpkg help versioning` or https://learn.microsoft.com/vcpkg/users/versioning for more information.
```

To resolve the issue:

1. Update the versions database:  
   - The version you want may not be in your local copy of the versions database. In that case, run the `git pull` command to update the [vcpkg registry](<https://github.com/Microsoft/vcpkg>) to the latest commit.  
2. Check available versions:  
   - Choose one of the versions available in the versions database.  
3. Update manifest file:
   - Edit your `vcpkg.json` file.
   - Change the specified version to one that is available in the vcpkg repository. For example, change from "version>=": "100.0.0" to "version>=": "10.1.1".  
4. Run vcpkg install:
   - Execute the `vcpkg install` command again in your terminal or command prompt.  

## <a name="version-scheme-conflict"></a> Cause: Specifying version constraint across different schemes
<!--
Steps to reproduce:
1. Open or create a vcpkg.json file for your project.
2. Specify boost-regex as a dependency with version>= 1.75.0
3. vcpkg install
-->
A version scheme conflict occurs when the version specified in the `vcpkg.json` file for a dependency uses a different versioning scheme than the one used in the vcpkg repository's baseline version. This results in an error as vcpkg cannot compare versions across different schemes.

If a declared `version>=` constraint uses a different version scheme than the one used in the baseline version, vcpkg is unable to determine which version is greater or equal than the other.

For example, if you specify:

```json
{
  "dependencies": [
    {
      "name": "boost-regex",
      "version>=": "1.75.0"
    }
  ]
}
```

vcpkg outputs the following error message:

```console
error: version conflict on boost-regex:x64-windows:  required 1.75.0, which cannot be compared with the baseline version 1.83.0.

The versions have incomparable schemes:
  boost-regex@1.83.0 has scheme relaxed
  boost-regex@1.75.0 has scheme string

This can be resolved by adding an explicit override to the preferred version. For example:

  "overrides": [
    { "name": "boost-regex", "version": "1.83.0" }
  ]

See `vcpkg help versioning` or https://learn.microsoft.com/vcpkg/users/versioning for more information.
```

Resolutions:
1. Use a compatible version scheme:
   - Inspect the version database in the vcpkg repository under `versions/b-/boost-regex.json` to find a version of `boost-regex` that uses the same versioning scheme as the baseline.
   - Update the `version>=` constraint in your `vcpkg.json` to a version that uses a compatible scheme.
2. Override to the desired version:
   - If you need a specific version of boost-regex that uses a different versioning scheme, you can override it in your manifest.
   - Modify your `vcpkg.json` to include an overrides section specifying the desired version:
    ```json
    {
      "dependencies": [
        { "name": "boost-regex" }
      ],
      "overrides": [
        { "name": "boost-regex", "version": "1.75.0" }
      ]
    }
    ```

## <a name="shallow-clone-version-constraint"></a> Cause: Inadequate commit history in shallow clone
<!--
Steps to reproduce:
1. Perform a shallow clone of the vcpkg repository using Git with limited depth, for example: `git clone --depth=1 https://github.com/microsoft/vcpkg.git`.
2. In your project, create or edit the `vcpkg.json` file to include a dependency with a specific version constraint.
3. from the project directory, vcpkg x-update-baseline --add-initial-baseline
4. Run `vcpkg install`
-->

When vcpkg is cloned with a limited commit history (shallow clone), it lacks the necessary commit history to resolve specific version constraints or baselines. The Git tree-object hashes used by vcpkg to retrieve specific versions of ports are only available when the full commit history is checked out. vcpkg detects when it has been cloned into a shallow repository and produces an error message when it fails to retrieve a port version.

For example, using a `vcpkg.json` with a specific baseline like the following:

```json
{
  "dependencies": [
    {
      "name": "fmt"
    }
  ],
  "overrides": [
    {
      "name": "fmt",
      "version": "7.1.3#1"
    }
  ],
  "builtin-baseline": "bb588985e37484d543fc849d0d79434e0d45bb3c"
}
```

Will result in the following error:

```console
error: failed to execute: "C:\Program Files\Git\cmd\git.exe" "--git-dir=C:\dev\demo\vcpkg\.git" "--work-tree=C:\dev\demo\vcpkg\buildtrees\versioning_\versions\fmt\4f8427eb0bd40da1856d4e67bde39a4fda689d72_26648.tmp" -c core.autocrlf=false read-tree -m -u 4f8427eb0bd40da1856d4e67bde39a4fda689d72
vcpkg was cloned as a shallow repository in: C:\dev\demo\vcpkg\.git
Try again with a full vcpkg clone.
error: git failed with exit code: (128).
fatal: failed to unpack tree object 4f8427eb0bd40da1856d4e67bde39a4fda689d72
note: while checking out port fmt with git tree 4f8427eb0bd40da1856d4e67bde39a4fda689d72
```

This error indicates that the commit (`4f8427eb0bd40da1856d4e67bde39a4fda689d72`) required for the specific version of the package `fmt` is not available in the shallow clone.

Resolutions:

1. Convert to a full clone
   - The easiest solution to this problem is to convert to  full clone:
  
   ```console
   git fetch --unshallow
   ```

2. Using GitHub Actions (Default Shallow Clones)
   - GitHub Actions often defaults to shallow clones. To work around this, you can modify the GitHub Actions workflow to perform a full clone. Add the following step before using vcpkg:
  
    ```yaml
    - name: Convert to Full Clone
      run: git fetch --unshallow
    ```

## <a name="unexpected-default-features"></a> Cause: Unexpected inclusion of default features in transitive dependencies

When managing dependencies with vcpkg, you might find that transitive dependencies are installed with their default features, even when you might not want those features for your project. This situation can lead to unexpected bloat or functionality in your final build.

### Scenario

You have a dependency on library `Y`, which in turn depends on library `X`. Library `X` has default features, including `foo`, that you want to exclude from your project. Your top-level manifest for library `Y` might look something like this:

```json
{
  "name": "my-project",
  "dependencies": [
    {
      "name": "Y",
      "features": ["featureB"],
      "default-features": false
    }
  ]
}
```

You expect that `X` will be installed without its default features due to the `"default-features": false` setting. However, `X` is still installed with the default feature `foo`.

### Reason

The `default-features` property is only considered at the top-level manifest. This means that default features of transitive dependencies (like `X` in this scenario) are still included unless explicitly disabled at the top level. When library `Y` is resolved, `vcpkg` does not propagate the `"default-features": false` setting to the transitive dependency `X`, resulting in the installation of `X` with its default features.

### Resolution

To ensure that transitive dependencies like `X` are installed without their default features, you need to promote them to direct dependencies in your top-level manifest and explicitly disable their default features. Modify your `vcpkg.json` to include `X` directly with `"default-features": false`:

```json
{
  "name": "my-project",
  "dependencies": [
    {
      "name": "Y",
      "features": ["featureB"]
    },
    {
      "name": "X",
      "default-features": false
    }
  ]
}
```

This approach ensures that `X` is installed without its default features, as now `X` is a direct dependency with an explicit instruction to exclude default features.

For more detailed information on how default features work and how to manage them, see the [default features concept article](../concepts/default-features.md) article.

## Issue isn't listed here

If your issue isn't listed here, visit [our repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
