---
title: Port overlays
description: Port overlays
author: data-queue
ms.author: danshaw2
ms.date: 9/20/2023
ms.prod: vcpkg
ms.topic: conceptual
---

# Port Overlays

## Overview
The default catalog in vcpkg comprises libraries maintained by both the vcpkg team and users. The catalog does not contain the entire universe of libraries, every possible version or patch.

To facilitate the usage of these missing libraries without having to either add them to the default catalog or manage a custom registry, vcpkg uses port overlays. Port overlays are effective when you don't want to worry about versioning and simply want to use a library.

## Overlay Ports
An overlay port typically refers to a new port or a drop-in replacement for an existing port. In either case, the overlay port takes priority in vcpkg dependency resolution, ensuring it gets selected. However, a potential pitfall is the naming conflict that can arise when multiple overlays introduce the same port.

For instance, `overlay-dir-A` and `overlay-dir-B` may both introduce the same overlay port `zlib`. In this situation, vcpkg resolves `zlib` based on the lookup order in the list of overlay port paths.

## Configuration Syntax

A directory refers to a set of overlay ports and can be specified as follows: 
- Single overlay port: `<directory>/sqlite3` refers to a single port
- Directory of overlay ports: `<directory>` refers to a directory of ports
A valid port must contain both `vcpkg.json` and `portfile.cmake`.

You can add an overlay port in several ways:
* Command-line: Add one or multiple `--overlay-ports=<directory>` options to your vcpkg command
* [Manifest](../reference/vcpkg-configuration-json.md#overlay-ports): Populate the `"overlay-ports"` array in `vcpkg-configuration.json`
* [Environmental variable](./config-environment.md#vcpkg_overlay_ports): Set `VCPKG_OVERLAY_PORTS` to a list of directory paths

Path lookup order follows first from a list in the command line, the manifest, and finally the environment.

### Example: Overlay Ports Example

Given this directory structure:
```
team-ports/
|-- sqlite3/
|---- vcpkg.json
|---- portfile.cmake
|-- rapidjson/
|---- vcpkg.json
|---- portfile.cmake
|-- curl/
|---- vcpkg.json
|---- portfile.cmake

my-ports/
|-- sqlite3/
|---- vcpkg.json
|---- portfile.cmake
|-- rapidjson/
|---- vcpkg.json
|---- portfile.cmake

vcpkg
|-- ports/
|---- <upstream ports>
|-- vcpkg.exe
```

Running:
```
vcpkg.exe install sqlite3 --overlay-ports=my-ports --overlay-ports=team-ports
```
Installs:
- `sqlite3` from `my-ports`

Running:
```
vcpkg/vcpkg.exe install sqlite3 rapidjson curl 
    --overlay-ports=my-ports/rapidjson 
    --overlay-ports=vcpkg/ports/curl
    --overlay-ports=team-ports
```
Installs:
- `sqlite3` from `team-ports`
- `rapidjson` from `my-ports`
- `curl` from `vcpkg/ports

### Example: Using overlay ports to use a system package manager dependency

To use a system package manager dependency over a vcpkg one, refer to our [blog post](https://devblogs.microsoft.com/cppblog/using-system-package-manager-dependencies-with-vcpkg/).
