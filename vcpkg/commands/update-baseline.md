---
title: vcpkg x-update-baseline
description: Command line reference for the vcpkg x-update-baseline command. Update baselines for all configured registries.
---
# vcpkg x-update-baseline

**This command is experimental and may change or be removed at any time**

## Synopsis

```no-highlight
vcpkg x-update-baseline [options] [--add-initial-baseline] [--dry-run]
```

## Description

Update baselines for all configured [registries](../users/registries.md).

In [Manifest mode](../users/manifests.md), this command operates on all registries in the `vcpkg.json` and the [`vcpkg-configuration.json`](../users/registries.md#vcpkg-configurationjson). In Classic mode, this command operates on the `vcpkg-configuration.json` in the vcpkg instance (`$VCPKG_ROOT`).

See the [versioning documentation](../users/versioning.md#baselines) for more information about baselines.

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--dry-run`

Print the planned baseline upgrades, but do not modify the files on disk.

### <a name="add-initial-baseline"></a> `--add-initial-baseline`

**Manifest mode only**

Add a [`"builtin-baseline"`](../users/manifests.md#builtin-baseline) field to the `vcpkg.json` if it does not already have one.

Without this flag, it is an error to run this command on a manifest that does not have any [registries](../users/registries.md) configured.
