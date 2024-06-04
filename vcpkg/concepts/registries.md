---
title: Registries concepts
description: Concepts about vcpkg registries and their capabilities.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: concept-article
---

# Registries concepts

Registries are collections of ports and their versions. The curated registry is
the one hosted at <https://github.com/Microsoft/vcpkg>. vcpkg lets you create
custom registries, which may be hosted by a variety of public or private providers.

There are currently two options to implement your own registries: a Git-based
registry or a filesystem-based registry.

## Built-in registry

The built-in registry refers to the implicit registry typically used in classic
mode scenarios, and edited directly in the directory `VCPKG_ROOT`.

If vcpkg was acquired using `git clone`, this will refer to the registry in `VCPKG_ROOT` itself,
and is expected to be a clone of <https://github.com/Microsoft/vcpkg> created before running vcpkg.

Otherwise (vcpkg was acquired using the 'one liner' installer or the 'Visual Studio bundle'),
this will be equivalent to a git registry with a `"repository"` of
`"https://github.com/Microsoft/vcpkg"`.

## Git registries

Git registries are simple Git repositories. They can be shared publicly or
privately via normal mechanisms for Git repositories. The [vcpkg
repository](https://github.com/microsoft/vcpkg) is an example of a Git registry.

Using Git registries offers the best experience for custom registries since you
have full control over the versions and contents of your registry.

## Filesystem registries

Filesystem registries, as the name implies, live on your filesystem. They are a
collection of ports located in a filesystem location and offer a primitive form
of version control using a separate path per version.

These type of registries are more suited to be a testing ground for your
packages. Or to provide an alternative for registries in version control systems
that are not Git.

## Next steps

Here are some tasks to try next:

* [Create your own Git-based registry](../produce/publish-to-a-git-registry.md)
* [Install packages from a custom registry](../consume/git-registries.md)
* [Read the registries reference documentation](../maintainers/registries.md)
