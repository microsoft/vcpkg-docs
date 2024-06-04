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

* Overlay locations specified in the command-line via [`--overlay-ports`](../commands/common-options.md#overlay-ports).
* Overlay locations specified in a `vcpkg-configuration.json` file via [`overlay-ports`](../reference/vcpkg-configuration-json.md).
* Overlay locations specified by the [`VCPKG_OVERLAY_PORTS`](../users/config-environment.md#vcpkg_overlay_ports) environment variable.

When resolving port names, the first location that contains a matching overlay port is selected.

## Using an overlay port

A directory can represent a set of overlay ports and can be specified in one of two ways:

* Single overlay port: `<directory>/sqlite3` refers to a single port
* Directory of overlay ports: `<directory>` refers to a directory of ports
A valid port must contain both `vcpkg.json` and `portfile.cmake`.

You can add an overlay port in several ways:

* Command-line: Add one or multiple `--overlay-ports=<directory>` options to your vcpkg command
* [Manifest](../reference/vcpkg-configuration-json.md#overlay-ports): Populate the `"overlay-ports"` array in `vcpkg-configuration.json`
* [Environmental variable](../users/config-environment.md#vcpkg_overlay_ports): Set `VCPKG_OVERLAY_PORTS` to a list of directory paths

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
