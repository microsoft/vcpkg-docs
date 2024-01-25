---
title: Glossary of vcpkg terms
description: This article provides a glossary of common terms-of-art used in vcpkg's documentation.
author: vicroms
ms.author: viromer
ms.date: 10/23/2023
ms.topic: glossary
---

# Glossary: vcpkg

Some of the terms used in this documentation have similar meanings when
discussed by the community. Because of that, and to avoid confusion and
ambiguity this article provides specific meanings for these terms.

## L

### Library

A piece of software (source code, binary files, documentation, license, etc.)
that is intended to be reused by other software. Most ports in the [vcpkg
registry](<https://github.com/Microsoft/vcpkg>) install C/C++ libraries.

## M

### Manifest

A file named [`vcpkg.json`](../reference/vcpkg-json.md) that describes the
metadata, such as dependencies, of a port or a project.

See the [manifests documentation](../concepts/manifest-mode.md) to learn more.

## O

### Overlay port/triplet

A locally available port or triplet. Usually used to provide a port or triplet
that is not part of a registry, or to override the name of a port or triplet in
a registry.

See the [overlays documentation](../concepts/overlay-ports.md) to learn more.

## P

### Package

A package can contain a library, collection of libraries, build scripts,
software tools, or other components necessary for their use. The goal of vcpkg
is to install these packages [available during your projects
builds](../concepts/build-system-integration.md).

### Port

A vcpkg-specific term, a port contains:

* Metadata about a package: name, version, dependencies, supported features,
  etc.
* Instructions to acquire, build if necessary, and install the package.

See the [ports documentation](../concepts/ports.md) to learn more.
