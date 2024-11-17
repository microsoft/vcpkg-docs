---
title: Registries concepts
description: Concepts about vcpkg registries and their capabilities.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: concept-article
---

# Registries concepts

vcpkg hosts a selection of libraries packaged into [ports](ports.md) at
<https://github.com/Microsoft/vcpkg>, this collection of ports is called the
*curated registry*. However, vcpkg is not limited to just installing ports from
the curated registry. Users are capable of extending the selection of ports by
creating custom registries.

A registry is a collection of ports and helper files arranged in a specific
structure. By following the registry structure, ports contained in a registry
can be installed with the same amount of features
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

For vcpkg to interface with a registry, it must comform with the structure
below:

* A directory named `ports`, contains the collection of ports, with each
  subdirectory containing a specific port matching the subdirectory name. For
  example, the files for port `foo` must be located in `ports/foo`.
* A directory named `versions` must contain the files that comprise the [versions
  database](#versions-database).

### Versions database

All registries, regardless of their kind, must contain a `versions` folder at the
root of the registry which contains the _versions database_.

There are two components to the versions database:

* the baseline file, 
* the version files.

The baseline file is a JSON file named `baseline.json` located at the root of
the `versions` directory. The purpose of the baseline file is to describe the
set of versions that are considered to be the latest for each port in the
registry. The layout of the baseline file depends on the kind of registry.

For each port in the registry, a corresponding _version file_ must exist.
The versions file is a JSON file with the same name as its corresponding port,
for example, a port named `foo` must have a corresponding `foo.json` version
file.

vcpkg expects the versions files to be stored at the following location
`versions/<prefix>/<port name>.json` inside the registry, where `<prefix>` is
the first letter of the port name followed by a hyphen. For example, the
version file for port `foo` must be stored in `versions/f-/foo.json`.

The purpose of the version file is two-fold:

* List all available versions of each port
* Point to the retrieval locations of each of version.

Same as the baseline file, the layout of the version file depends on the kind of
registry.

### Example: registry structure

```
ports/
  foo/
    portfile.cmake
    vcpkg.json
versions/
  f-/
    foo.json
  baseline.json
```

## Built-in registry

The built-in registry is a special kind of registry. The built-in registry
is the default registry used in some operation modes, like [classic
mode](classic-mode.md). When a [default
registry](../reference/vcpkg-configuration-json.md#default-registry) is not
specified, vcpkg implicity uses the built-in registry as the default.

The built-in registry most often refers to the local copy of the curated
registry created when you `git clone` the vcpkg repository from
<https://github.com/Microsoft/vcpkg>. Some operations expect that the
[`VCPKG_ROOT` environment variable](../users/config-environment.md#vcpkg_root)
points to a built-in registry.

If vcpkg is acquired through the "one liner" or Visual Studio installer, the
built-in registry becomes equivalent to a [Git registry](#git-registries)
pointing to the <https://github.com/Microsoft/vcpkg> repository.

## Git registries

A Git registry is a repository that follows the registry structure and leverages
Git's capatilities to provide versioning for the ports in the registry. The
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
details on how to implement a Git registry.

## Filesystem registries

Filesystem registries are an implementation of a registry that lives on a
filesystem. They follow the common registry structure but don't make use of Git
to offer versioning capabilities, instead they use a primitive form of version
control using a distinct path for each version of its ports.

These type of registries are more suited to be a testing ground for your ports.
Or to provide an alternative for registries in version control systems that are
not Git. Since no tooling is offered to manipulate the version database files,
this method is not recommended for large collections of ports.

See the [registries
reference](../maintainers/registries.md#filesystem-registries) for details on
how to implement a filesystem registry.

## Next steps

Here are some tasks to try next:

* [Create your own Git-based registry](../produce/publish-to-a-git-registry.md)
* [Install packages from a custom registry](../consume/git-registries.md)
* [Read the registries reference documentation](../maintainers/registries.md)
