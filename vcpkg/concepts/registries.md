---
title: Registries concepts
description: Concepts about vcpkg registries and their capabilities.
author: vicroms
ms.author: viromer
ms.date: 11/10/2024
ms.topic: concept-article
---
# Registries concepts

## Overview

vcpkg hosts a selection of libraries packaged into [ports](ports.md) at
<https://github.com/Microsoft/vcpkg>. This collection of ports is called the
*curated registry*. However, vcpkg is not limited to the curated registry. Users can extend the selection of ports by
creating custom registries.

A registry is a collection of ports and helper files arranged in a specific
structure. By following the registry structure, ports contained in a registry
can be installed with the same features
([versioning](../users/versioning.md), [binary
caching](../users/binarycaching.md)) offered to ports in the curated registry.

There are currently three kinds of registries:

* The [built-in registry](#built-in-registry),
* [Git registries](#git-registries), and
* [filesystem registries](#filesystem-registries).

vcpkg can be instructed to consider ports available in custom registries by
using a [`vcpkg-configuration.json`
file](../reference/vcpkg-configuration-json.md#registries). See the [Tutorial:
Install a dependency from a Git-based registry](../consume/git-registries.md)
article for a tutorial on how to use custom registries in your projects.

## Registry structure

For vcpkg to interface with a registry, it must conform to the following structure:

* A directory named `ports` contains the collection of ports, with each
  subdirectory containing a specific port matching the subdirectory name. For
  example, the files for port `foo` is located in `ports/foo`.
* A directory named `versions` contains the files that comprise the [versions
  database](#versions-database).
  
### Example: registry structure

```text
ports/
  foo/
    portfile.cmake
    vcpkg.json
versions/
  f-/
    foo.json
  baseline.json
```

## Versions database

All registries contain a `versions` directory at the
root of the registry which contains the *versions database*.

There are two components to the versions database:

* The baseline file
* The version files

The baseline file is a JSON file named `baseline.json` located at the root of
the `versions` directory.

The versions files are JSON files with the same names as the available ports.
They must exist at `versions/<prefix>/<port name>.json`, where `<prefix>` is
the first letter of the port name followed by a hyphen. For example, the
versions file for port `foo` must be at `versions/f-/foo.json`.

The purpose of the versions file is two-fold:

* List all available versions of each port
* Point to the retrieval locations of each version.

The format of the version file depends on the kind of the registry.

## Baseline file

All registries, regardless of their kind, must contain a file named
`baseline.json` located at `versions/baseline.json`.

The purpose of the baseline file is to describe the set of versions that are
considered to be the latest for all ports in the registry. It is expected that
this file is updated each time a new version of a port is added to the registry.

The file is a JSON file composed of a single object whose properties are named baseline objects. Each baseline object's
properties are port names, whose values are version entries. The [registries reference](../maintainers/registries.md)
article describes the layout of baseline files in more detail.

Each baseline version entry is an object with the following properties:

* `baseline`: the value is the port's version matching its corresponding
  `vcpkg.json` file.
* `port-version`: the value is the port's `port-version` matching its
  corresponding `vcpkg.json` file.

### Example baseline file

```json
{
  "default": {
    "foo": { "baseline": "1.0.0", "port-version": 0 },
    "bar": { "baseline": "2024-08-01", "port-version": 1 },
    "baz": { "baseline": "vista-xp", "port-version": 0 }
  }
}
```

See the reference documentation for:

* Baseline file layout for [Git
  registries](../maintainers/registries.md#git-baseline)
* Baseline file layout for [filesystem
  registries](../maintainers/registries.md#filesystem-baseline)

## Versions files

Each port in the registry has a corresponding versions file.
Versions files are stored at `versions/<prefix>/<port name>.json` , where `<prefix>` is the first letter of
the port name followed by a hyphen. For example, the versions file for port
`foo` is at `versions/f-/foo.json`.

The purpose of the versions file is two-fold:

* List all available versions of each port
* Point to the retrieval locations of each of these versions

The layout of the versions file is an object containing a `"versions"` array, with
each entry in that array being a version object.  A version object must contain
the following properties:

* A version property: The property's key and value must match the ones used by
  the port in its `vcpkg.json` file. The key must be one of `version`,
  `version-semver`, `version-date`, or `version-string`; the value must be the
  version as it appears in the port's manifest file (`vcpkg.json`).
* `port-version`: the value is the port's `port-version` as it appears in the
  port's `vcpkg.json` file.
* `git-tree`: (only on Git registries) the value is the git-tree SHA
  corresponding to the port's directory. This is a SHA calculated by hashing the
  contents of the port's directory; this git-tree SHA can be used by Git to
  retrieve the contents of the port matching the provided git-tree. This makes
  it possible for vcpkg to retrieve old versions of ports from the registries
  Git history. Read the [git registries](#git-registries) section to learn how
  to obtain this value for any given port version.
* `path`: (only on filesystem registries) the value is the full path to a
  directory containing the port files for the specific version.

### Example of a filesystem registry version file

```json
{
  "versions": [
    {
      "path": "$/ports/foo/1.2.0",
      "version": "1.2.0",
      "port-version": 0
    },
    {
      "path": "$/ports/foo/1.1.0",
      "version": "1.1.0",
      "port-version": 0
    },
    {
      "path": "$/ports/foo/1.0.0",
      "version": "1.0.0",
      "port-version": 0
    }
  ]
}
```

See the reference documentation for:

* Versions file layout for [Git
  registries](../maintainers/registries.md#git-version-file)
* Versions file layout for [filesystem
  registries](../maintainers/registries.md#filesystem-version-file)

## Built-in registry

The built-in registry is a special kind of registry. It is the default registry used in [classic mode](classic-mode.md).
In [manifest mode](manifest-mode.md), when a [default registry](../reference/vcpkg-configuration-json.md#default-registry)
is not specified, vcpkg implicitly uses the built-in registry.

The built-in registry refers to the local copy of the curated
registry created when you `git clone` the vcpkg repository from
<https://github.com/Microsoft/vcpkg>. Some operations expect that the
[`VCPKG_ROOT` environment variable](../users/config-environment.md#vcpkg_root)
points to a built-in registry.

If vcpkg is acquired through the "one liner" or Visual Studio installer, the
built-in registry becomes equivalent to a [Git registry](#git-registries)
pointing to the <https://github.com/Microsoft/vcpkg> repository.

## Git registries

A Git registry is a repository that follows the registry structure and leverages
Git's capabilities to provide versioning for the ports in the registry. The
curated registry at <https://github.com/Microsoft/vcpkg> is an implementation of
a Git registry.

Git registries can be hosted in any Git repository provider, allowing you to
use your chosen Git hosting service to control access to your custom registry
while also making your registry easy to share.

Git registries are the recommended method for implementing a custom registry.
Since the versioning mechanism is the same used by the curated registry, Git
registries can make use of the [`x-add-version`](../commands/add-version.md) to
manage your version database files.

See the [registries reference](../maintainers/registries.md#git-registries) for
implementation details of Git registries.

### <a name="git-add-version"> Adding a new version to a Git registry

The [`x-add-version`](../commands/add-version.md) command can be used to to add
a new port or a new version to the registry. When adding versions using this
command there are a couple of things to keep in mind:

> [!IMPORTANT]
> When adding a new version, always remember to update the port's declared version to one not already published, to avoid
> rewriting the version history.

When you are making changes to a port, the first step should be to increase its
version in the `vcpkg.json` file. If your changes include an update to the
package's [upstream](../concepts/glossary.md#upstream) version, remember to set
the `port-version` to `0`; otherwise, remember to increase the `port-version` by
one.

The [`x-add-version` command](../commands/add-version.md) requires that all your
port changes are commited to the repository before updating the version
database.

#### Example: adding a new port version to a Git registry

```Console
git add ports/foo/.
git commit -m "Temporary commit"
vcpkg x-add-version --x-builtin-ports-root=./ports --x-builtin-registry-versions-dir=./versions foo
added version 1.0.0#1 to path/to/registry/versions/f-/foo.json
added version 1.0.0#1 to path/to/registry/versions/baseline.json
```

The redirection options `--x-builtin-ports-root` and
`--x-builtin-registry-versions-dir` should point to your registry's `ports` and
`versions` directories respectively.

Once, the `x-add-version` command runs successfully, amend the last commit to
include the versions file changes.

```bash
git commit --amend -m "Update foo to new version"
```

## Filesystem registries

Filesystem registries are an implementation of a registry that lives on a
filesystem. They follow the common registry structure but don't make use of Git
to offer versioning capabilities. Instead, they use a primitive form of version
control using a distinct path for each version of its ports.

These types of registries are more suited to be a testing ground for your ports or to provide an alternative for
registries in version control systems that are not Git. Since no tooling is offered to manipulate the version database
files, this method is not recommended for large collections of ports.

See the [registries
reference](../maintainers/registries.md#filesystem-registries) for details on
how to implement a filesystem registry.

## Consume registries

To consume a custom registry in your project, you need to create a
[configuration file (`vcpkg-configuration.json`)](../reference/vcpkg-configuration-json.md) next
to your project's [manifest file (`vcpkg.json`)](../reference/vcpkg-json.md).

### Default registry

When [resolving port names](package-name-resolution.md), the default registry
works as the fallback when a port's name is not found in any other registry
specified in the [`registries` array](#registries-array).

As a convenience for users that are not engaging with custom registries, vcpkg
implicitly adds the [built-in registry](#built-in-registry) as the default
registry. To change this behavior, the default registry can be set to any
registry or disabled completely using the
[`default-registry`](../reference/vcpkg-configuration-json.md#default-registry)
property.

#### Example: Set a custom registry as default

`vcpkg-configuration.json`

```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/Microsoft/vcpkg",
    "baseline": "84a143e4caf6b70db57f28d04c41df4a85c480fa"
  }
}
```

#### Example: Disable the default registry

`vcpkg-configuration.json`

```json
{
  "default-registry": null
}
```

### Registries array

To extend the selection of ports available to install with vcpkg, you can
specify additional registries using the [`registries`
array](../reference/vcpkg-configuration-json.md#registries).

#### Example: Add custom registries to the configuration

> [!NOTE]
> Depending on the registry kind, you may need to provide different information
> in the `registries` array. See the [`vcpkg-configurtion.json`
> reference](../reference/vcpkg-configuration-json.md#registries) to learn which
> properties are required for each registry kind.

`vcpkg-configuration.json`

```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/Microsoft/vcpkg",
    "baseline": "84a143e4caf6b70db57f28d04c41df4a85c480fa"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://my.privateregistry.git",
      "baseline": "caf6b70db5784a143e4f28d05c480fa4c41df4a8",
      "packages": [ "foo" ]
    },
    {
      "kind": "filesystem",
      "path": "C:/path/to/registry",
      "baseline": "baseline1",
      "packages": [ "bar" ]
    }
  ]
}
```

## Recommended practices for registries

### Don't rewrite version history

Once a version has been published to the versions files, do not change its
associated `git-tree` in a git registry or directory in a filesystem registry.

One of vcpkg's design principles is that the versions of installed dependencies
do not change without user intervention. Rewriting the versions file history by
changing a `git-tree` entry violates this principle.

If the existing version has issues, prefer to create a new
[`port-version`](../reference/vcpkg-json.md#port-version).

### Don't delete versions files

When removing a port from your registry, remove its contents from the ports
directory and its entry in the baseline file. But *do not* remove its associated
versions file.

Even if a port no longer exists in the registry, as long as the versions file
remains, users of the port can install old versions by using [version
`overrides`](../users/versioning.md#overrides).

## Next steps

Here are some tasks to try next:

* [Create your own Git-based registry](../produce/publish-to-a-git-registry.md)
* [Install packages from a custom registry](../consume/git-registries.md)
* [Read the registries reference documentation](../maintainers/registries.md)
