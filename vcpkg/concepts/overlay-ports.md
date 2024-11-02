---
title: Overlay ports
description: This article describes overlay ports in vcpkg. Overlay ports are used to force a specific port definition to be used by vcpkg during the package installation process.
author: data-queue
ms.author: danshaw2
ms.date: 01/10/2024
ms.topic: concept-article
---

# Overlay ports

Usually, vcpkg ports are obtained from [registries](../concepts/registries.md). It is very likely that most of the ports you install come from the official vcpkg registry at <https://github.com/Microsoft/vcpkg>. vcpkg lets you install ports available to you via the file system, we call these ports, overlay ports.

An overlay port can act as a drop-in replacement for an existing port or as a new port that is otherwise not available in a [registry](../maintainers/registries.md). While [resolving package names](../concepts/package-name-resolution.md), overlay ports take priority.

Overlay ports are evaluated in the following order:

* Named subdirectories of directories specified in the command-line via [`--overlay-port-dirs`](../commands/common-options.md#overlay-port-dirs).
* Named subdirectories of directories specified in a `vcpkg-configuration.json` file via [`overlay-port-dirs`](../reference/vcpkg-configuration-json.md).
* Named subdirectories of directories specified by the [`VCPKG_OVERLAY_PORT_DIRS`](../users/config-environment.md#vcpkg_overlay_port_dirs) environment variable.
* The directory specified in the command-line via [`--overlay-ports`](../commands/common-options.md#overlay-ports), or named subdirectories if that directory has no `CONTROL` or `vcpkg.json` file.
* The directory specified in a `vcpkg-configuration.json` file via [`overlay-ports`](../reference/vcpkg-configuration-json.md), or named subdirectories if that directory has no `CONTROL` or `vcpkg.json` file.
* The directory specified by [`VCPKG_OVERLAY_PORTS`](../users/config-environment.md#vcpkg_overlay_ports) environment variable entries, or named subdirectories if that directory has no `CONTROL` or `vcpkg.json` file.

When resolving port names, the first location that contains a matching overlay port is selected.

## Using an overlay port

If an overlay port is specified, first, vcpkg attempts to load that directory as a port. If that succeeds, the directory itself is itself treated as a port, and the name of the overlay is derived from the `CONTROL` or `vcpkg.json` file. Otherwise, subdirectories with the overlay port name are considered.

If an overlay port dir is specified, never attempts to load that as a port, and proceeds to the subdirectories with overlay port names.

Valid ports contain `portfile.cmake`, and either `vcpkg.json` or `CONTROL`.

For example, consider the following directory structure:

* `x/vcpkg.json`, the `"name"` field is set to `"a"`.
* `x/portfile.cmake`, the associated build instructions for `a`.
* `x/b/vcpkg.json`, the `"name"` field is set to `"b"`.
* `x/b/portfile.cmake`, the associated build instructions for `b`.
* `x/c/vcpkg.json`, the `"name"` field is set to `"c"`.
* `x/c/portfile.cmake`, the associated build instructions for `c`.
* `y/d/vcpkg.json`, the `"name"` field is set to `"d"`.
* `y/d/portfile.cmake`, the associated build instructions for `d`.
* `y/e/vcpkg.json`, the `"name"` field is set to `"e"`.
* `y/e/portfile.cmake`, the associated build instructions for `e`.

vcpkg will consider the following ports given the following settings:

* `--overlay-ports=x`: There is one port in this overlay, `a`. The name is derived from `vcpkg.json`.
* `--overlay-ports=x/b`: There is one port in this overlay, `b`. The name is derived from `vcpkg.json`.
* `--overlay-ports=y`: There are two ports in this overlay, `d` and `e`. Their names are derived from the subdirectories of `y`, and the names declared in their `vcpkg.json` must match, or an error will be generated if vcpkg is asked to install `d` or `e`.
* `--overlay-port-dirs=x`: There are two ports from this overlay, `b` and `c`. Their names are derived from the subdirectories of `x`, and the names declared in their `vcpkg.json` must match, or an error will be generated if vcpkg is asked to install `b` or `c`.
* `--overlay-port-dirs=x/b`: There are no ports from this overlay as there are no subdirectories.
* `--overlay-port-dirs=y`: This is the same case as `--overlay-ports=y`, because there is no `y/CONTROL` or `y/vcpkg.json`. There are two ports in this overlay, `d` and `e`. Their names are derived from the subdirectories of `y`, and the names declared in their `vcpkg.json` must match, or an error will be generated if vcpkg is asked to install `d` or `e`.

You can add to overlay port configuration in several ways:

* Command-line: Add one or more `--overlay-port-dirs=<directory>` or `--overlay-ports=<directory>` options to the command-line.
* [Manifest](../reference/vcpkg-configuration-json.md#overlay-ports): Populate the `"overlay-port-dirs"` or `"overlay-ports"` arrays in `vcpkg-configuration.json`.
* [Environmental variable](../users/config-environment.md#vcpkg_overlay_ports): Set `VCPKG_OVERLAY_PORT_DIRS` or `VCPKG_OVERLAY_PORTS` to a path character delimited list.

### Example: Overlay Ports Example

Given this directory structure:

:::image type="complex" source="../resources/ports-overlay-example.png" alt-text="Example with multiple overlay port directories":::
Overlay directory named team-ports contains ports sqlite3, rapidjson and curl. Overlay directory named my-ports contains ports sqlite3 and rapidjson. The vcpkg directory contains the default registry.
:::image-end:::

Run:

```bash
vcpkg install sqlite3 --overlay-ports=my-ports --overlay-ports=team-ports
```

To install:

* `sqlite3` from `my-ports`

Run:

```
vcpkg install sqlite3 rapidjson curl
    --overlay-ports=my-ports/rapidjson
    --overlay-ports=vcpkg/ports/curl
    --overlay-ports=team-ports
```

To install:

* `sqlite3` from `team-ports`
* `rapidjson` from `my-ports`
* `curl` from `vcpkg/ports

### Example: Using overlay ports to use a system package manager dependency

To use a system package manager dependency over a vcpkg one, refer to our [blog post](https://devblogs.microsoft.com/cppblog/using-system-package-manager-dependencies-with-vcpkg/).
