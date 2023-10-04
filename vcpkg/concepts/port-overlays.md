---
title: Port overlays
description: This article describes overlay ports in vcpkg. Overlay ports are used to force a specific port definition to be used by vcpkg during the package installation process.
author: data-queue
ms.author: danshaw2
ms.date: 9/20/2023
ms.prod: vcpkg
ms.topic: concept-article
---

# Overlay ports

An overlay port can act as a drop-in replacement for an existing port. An overlay port can also add a new port that is otherwise not available in a [registry](../maintainers/registries.md). While [resolving package names](../users/registries.md#package-name-resolution), overlay ports take priority, forcing overlay ports to be selected.

Naming conflicts can arise when multiple overlays introduce the same port. For instance, overlay directories `[dirA, dirB]` may introduce a port with the same name: `dirA/zlib` and `dirB/zlib`. vcpkg resolves `zlib` in order of which directory is specified first. This example selects `dirA/zlib` as the `zlib` port. If overlay port options are specified in multiple places, overlay resolution priority follows first from the command line, then the manifest, and finally, the environment.

## Configuration Syntax

A directory refers to a set of overlay ports and can be specified as follows: 
- Single overlay port: `<directory>/sqlite3` refers to a single port
- Directory of overlay ports: `<directory>` refers to a directory of ports
A valid port must contain both `vcpkg.json` and `portfile.cmake`.

You can add an overlay port in several ways:
* Command-line: Add one or multiple `--overlay-ports=<directory>` options to your vcpkg command
* [Manifest](../reference/vcpkg-configuration-json.md#overlay-ports): Populate the `"overlay-ports"` array in `vcpkg-configuration.json`
* [Environmental variable](../users/config-environment.md#vcpkg_overlay_ports): Set `VCPKG_OVERLAY_PORTS` to a list of directory paths

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

Run:
```bash
vcpkg install sqlite3 --overlay-ports=my-ports --overlay-ports=team-ports
```
To install:
- `sqlite3` from `my-ports`

Run:
```
vcpkg/vcpkg.exe install sqlite3 rapidjson curl 
    --overlay-ports=my-ports/rapidjson 
    --overlay-ports=vcpkg/ports/curl
    --overlay-ports=team-ports
```
To install:
- `sqlite3` from `team-ports`
- `rapidjson` from `my-ports`
- `curl` from `vcpkg/ports

### Example: Using overlay ports to use a system package manager dependency

To use a system package manager dependency over a vcpkg one, refer to our [blog post](https://devblogs.microsoft.com/cppblog/using-system-package-manager-dependencies-with-vcpkg/).
