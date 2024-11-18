---
title: Registries reference
description: This article describes specific details on how to implement custom registries
author: vicroms
ms.author: viromer
ms.date: 11/10/2024
ms.topic: reference
---
# Registries reference

## Overview

This article describes the implementation details for each kind of custom
registry. Specifically, this file concerns with the recommended layout for each
kind of registry and the expected contents of their respective [versions
database](../concepts/registries.md#versions-database).

> [!NOTE]
> This article contains information on how to implement custom registries. For
> information on consuming custom registries in your project see the [Using
> registries](../consume/git-registries.md) article. 

## Git registries

### <a name="git-baseline"> Baseline file layout in Git registries

*Top-level fields*

The top-level object in a `baseine.json` file is a dictionary, each key in this
dictionary is a _named baseline_. Due to implementation details of Git
registries, it is required that a _named baseline_ with the name "default"
exists and that it contains a mapping of all the ports in the registry to their
baseline version.

| Name           | Type           | Description |
| -------------- | -------------- | ----------- |
| `default`      | BaselineObject | The default baseline, required for Git registries. |
| Named baseline | BaselineObject | Additional baselines. The field name corresponds to the baseline name. |

*BaselineObject*

The baseline object is a dictionary, with each key corresponding to a port name
in the registry and its value being the latest version of the port.

| Name      | Type                  | Description |
| --------- | --------------------- | ----------- |
| Port name | BaselineVersionObject | A mapping of a port name to its latest version |

*BaselineVersionObject*

| Name | Type | Description |
| ---- | ---- | ----------- |
| `baseline` | string | A string corresponding to the latest available version of the port in the registry. |
| `port-version` | integer | An integer corresponding to the latest port version of the port in the registry |

#### Example of a `baseline.json` file in a Git registry

In a registry containing a single port named `foo` at version `1.0.0#1`, the
`baseline.json` file contents should be:

```json
{
  "default": {
    "foo": { 
      "baseline": "1.0.0", 
      "port-version": "1"
    }
  }
}
```

### <a name="git-version-file"></a> Version file layout in Git registries

The `versions` directory contains all the information about which versions of
packages are contained in the registry, along with the method to retrieve those
versions from the repository's history.

*Top-level fields*

| Name       | Type            | Description |
| ---------- | --------------- | --------------------------- |
| `versions` | VersionObject[] | An array of version objects. Contains an entry for each version of the port in the history of the registry. |

*VersionObject*

| Name       | Type   | Description |
| ---------- | ------ | ----------- |
| `git-tree` | string | A Git calculated SHA used to retrieve the port version |
| [`version`<br>`version-semver`<br>`version-date`<br>`version-string`](#version) | string | Upstream version information |
| [port-version](#port-version) | integer | Port files revision |


#### Example of a Git registry version file

```json
{
  "versions": [
    {
      "git-tree": "963060040c3ca463d17136e39c7317efb15eb6a5",
      "version": "1.2.0",
      "port-version": 0
    },
    {
      "git-tree": "548c90710d59c174aa9ab10a24deb69f1d75ff8f",
      "version": "1.1.0",
      "port-version": 0
    },
    {
      "git-tree": "67d60699c271b7716279fdea5a5c6543929eb90e",
      "version": "1.0.0",
      "port-version": 0
    }
  ]
}
``` 

#### <a name="obtain-git-tree-sha"></a> Obtaining a `git-tree` SHA

vcpkg uses Git's capabilities to retrieve specific versions of a port contained
in its commit history. The method used is to retrieve the `git-tree` object from
the repository as specified in the version file of a port.

Each port directory in a Git registry has a unique SHA associated with it
(referred as `git-tree` in the version files). The SHA is calculated using the
contents of the directory; each time a change is commited to the repository that
modifies a directory, its SHA is recalculated. 

Git allows you to retrieve the contents of a given directory at any point in its
history, provided that you know their specific SHA. By making use of this
feature, vcpkg can index specific port versions with their respective SHA
(`git-tree`).

To obtain the SHA of a port directory at any given revision the following Git
command can be used:

```Console
git -C <path/to/ports> ls-tree --format='%(objectname)' <commit sha> -- <portname>
``` 

Example:

```Console
git -C %VCPKG_ROOT%/ports ls-tree --format='%(objectname)' HEAD -- curl
6ef1763f3cbe570d6378632c9b5793479c37fb07
```

The command returns the SHA of the directory containing the `curl` port at the
current revision (`HEAD`).

It is possible to show the contents of the `git-tree` using the command `git show <git-tree>`:

```Console
git show 6ef1763f3cbe570d6378632c9b5793479c37fb07
tree 6ef1763f3cbe570d6378632c9b5793479c37fb07

0005_remove_imp_suffix.patch
0020-fix-pc-file.patch
0022-deduplicate-libs.patch
cmake-config.patch
cmake-project-include.cmake
dependencies.patch
export-components.patch
portfile.cmake
redact-input-vars.diff
usage
vcpkg-cmake-wrapper.cmake
vcpkg.json
```

Or the contents of a specific file with `git show <git-tree>:<file>`:

```Console
git show 6ef1763f3cbe570d6378632c9b5793479c37fb07:usage
curl is compatible with built-in CMake targets:

    find_package(CURL REQUIRED)
    target_link_libraries(main PRIVATE CURL::libcurl)
```

Mantaining the database files up to date using these Git commands in a manual
process can be a difficult task. For that reason we recommend using the
[`x-add-version` command](../commands/add-version.md), which automates the
process as long as the repository follows the recommended [registry
structure](../concepts/registries.md#registry-structure). See the [Tutorial:
Publish packages to a private vcpkg registry using
Git](../produce/publish-to-a-git-registry.md) article for an example of how to
publish a port in a Git registry.

### Filesystem registries

### <a name="filesystem-baseline"></a> Baseline file layout in filesystem registries

*Top-level fields*

The top-level object in a `baseine.json` file is a dictionary, each key in this
dictionary is a _named baseline_. Baselines should contain mappings of all the
ports in the registry to their baseline version.

| Name           | Type           | Description |
| -------------- | -------------- | ----------- |
| Named baseline | BaselineObject | Additional baselines. The field name corresponds to the baseline name. |

*BaselineObject*

The baseline object is a dictionary, with each key corresponding to a port name
in the registry and its value being the latest version of the port.

| Name      | Type                  | Description |
| --------- | --------------------- | ----------- |
| Port name | BaselineVersionObject | A mapping of a port name to its latest version |

*BaselineVersionObject*

| Name | Type | Description |
| ---- | ---- | ----------- |
| `baseline` | string | A string corresponding to the latest available version of the port in the registry. |
| `port-version` | integer | An integer corresponding to the latest port version of the port in the registry |

The layout of the baseline file in a filesystem registry is the same as for [Git
registries](#git-baseline-layout). The only difference being that filesystems
don't require a `default` baseline.

#### Example of a `baseline.json` file in a Git registry

```json
{
  "my-baseline": {
    "foo": {
      "baseline": "1.0.0",
      "port-version": 1
    }
  }
}
```
### <a name="filesystem-version-file"></a> Version file layout in Git registries

The `versions` directory contains all the information about which versions of
packages are contained in the registry, along with the method to retrieve those
versions from a filesystem location.

*Top-level fields*

| Name       | Type            | Description |
| ---------- | --------------- | --------------------------- |
| `versions` | VersionObject[] | An array of version objects. Contains an entry for each version of the port in the registry. |

*VersionObject*

| Name       | Type   | Description |
| ---------- | ------ | ----------- |
| `path`     | string | The filesystem location where the port files for the corresponding version are located |
| [`version`<br>`version-semver`<br>`version-date`<br>`version-string`](#version) | string | Upstream version information |
| [port-version](#port-version) | integer | Port files revision |

When specifying the `path` of a registry, the `$` character can be used to
reference the root of the registry. Otherwise, absolute paths can be used
instead.

#### Example of a filesystem registry version file

```json
{
  "versions": [
    {
      "git-tree": "$/ports/foo/1.2.0",
      "version": "1.2.0",
      "port-version": 0
    },
    {
      "git-tree": "$/ports/foo/1.1.0",
      "version": "1.1.0",
      "port-version": 0
    },
    {
      "git-tree": "$/ports/foo/1.0.0",
      "version": "1.0.0",
      "port-version": 0
    }
  ]
}
```

## Next steps

Here are some tasks to try next:

* [Read the registries conceptual documentation](../concepts/registries.md)
* [Create your own Git-based registry](../produce/publish-to-a-git-registry.md)
* [Install packages from a custom registry](../consume/git-registries.md)
