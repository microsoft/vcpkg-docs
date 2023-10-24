---
title: Registries concepts
description: Concepts about vcpkg registries and their capabilities.
author: vicroms
ms.author: viromer
ms.date: 10/24/2023
ms.prod: vcpkg
ms.topic: concept-article
---

# Registries concepts

Registries are collections of ports and their versions. The curent catalog of
ports in vcpkg is distributed via the registry at
<https://github.com/Microsoft/vcpkg>. vcpkg lets you create your own custom
registries which you can make either public or private, and host them in a
variety of storage providers.

There are currently two options to implement your own registries: a Git-based
registriy or a filesystem-based registries.

## Built-in registry
The built-in registry refers to the main vcpkg registry at
<https://github.com/Microsoft/vcpkg>. Depending on the vcpkg operation mode this
can mean your local clone of the vcpkg repository or the remote repository
hosted in GitHub.

## Git registries
Git registries are simple Git repositories. They can be shared publicly or
privately via normal mechanisms for Git repositories. The [vcpkg
repository](https://github.com/microsoft/vcpkg) is an example of a Git registry.

Using Git registries offers the best experience for custom registries, since you
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
* [Read the registries reference documentatin](../maintainers/registries.md)
