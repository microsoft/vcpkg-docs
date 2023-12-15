---
title: Troubleshoot versioning and feature selection issues
description: Troubleshooting guide for common versioning issues
author: JavierMatosD
ms.author: javiermat
ms.date: 11/30/2023
ms.prod: vcpkg
ms.topic: troubleshooting-general
---

# Versioning troubleshooting guide

This guide is intended for users experiencing issues with [versioning](./versioning.md).

## Inspecting the version file for a port

This process involves checking the available versions of a package in the vcpkg repository. Here's how to do it:

How to inspect:
1. Navigate to the `vcpkg/versions` directory
2. Locate the port folder
   - Within the `versions` directory, ports are organized alphabetically in folders. Find the folder corresponding to the first letter of your desired port. For example, for `fmt`, look in the `f-` folder.
3. Open the ports version file:
   - In the appropriate alphabetical folder, locate the JSON file for your port. For `fmt`, this would be `fmt.json.`

What you will find:

In the port's version file, you'll see a list of available versions with details like version numbers and corresponding commit hashes. This information is crucial for understanding what versions you can specify in your project's `vcpkg.json` file.

How to use this information:
- Selecting a version: Choose a version from the list that meets your projects needs.
- Updating your manifest: Reflect this in your projects `vpckg.json` file under the appropriate dependency.
- Reinstalling packages: After updating the manifest file, run `vcpkg install` to install newly specified version.

For more details on versioning, see our reference documentation:
- [versioning concepts](./versioning.concepts.md)
- [versioning](./versioning.md)

For more details on using a manifest, see [manifest](./manifests.md)

## <a name="non-existent-version"></a> Cause: Requesting a non-existent version of a package
<!-- 
Steps to reproduce:
1. Open or create a vcpkg.json file for your project.
2. Specify a dependency with a version that does not exist in the vcpkg repository. For example, set the version of the fmt library to a non-existent version like "version>=": "100.0.0".
3. vcpkg install
-->

When a version specified in the manifest file does not exist in the vcpkg version database, vcpkg will fail to resolve the dependency and throw an error resembling the following:

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
  - Edit your vcpkg.json file.  
  - Change the specified version to one that is available in the vcpkg repository. For example, change from "version>=": "100.0.0" to "version>=": "10.1.1".  
4. Run vcpkg install
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

vcpkg will emit the following error:

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

When vcpkg is cloned with a limited commit history (shallow clone), it lacks the necessary commit history to resolve specific version constraints or baselines. This results in errors where vcpkg cannot find the required versions of packages or the specified baseline, as the commit history needed for these versions or baselines is not available in the shallow clone.

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

## Issue isn't listed here

If your issue isn't listed here, visit [our repository](https://github.com/microsoft/vcpkg/issues) to create a new issue.
