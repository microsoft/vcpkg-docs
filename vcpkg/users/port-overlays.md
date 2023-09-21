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
The default catalog contains vcpkg team and user maintained libraries designed for general use. However, some libraries do not exist in the default catalog due to a variety of reasons: a library missing a version, a custom library, a library without CI support, a missing library. 

vcpkg uses port overlays so that you can use a library without requiring you to add it to the default catalog or maintain a custom registry. Port overlays are effective when don't want to worry about versioning and simply want to use a library.

## Overlay Ports
An overlay port refers to a new port or a drop-in replacement for an existing port. In either case, the overlay port will have the highest precedence in vcpkg dependency resolution and will always be selected. However, multiple ports added as overlays can introduce naming conflicts. 

For example, `overlay-dir-A` and `overlay-dir-B` may both introduce the same overlay port `zlib`. vcpkg chooses `zlib` based on the lookup order in the list of overlay port paths.

## Configuration Syntax
Overlay port paths can be specified directly on the vcpkg command line, added as a part of the manifest in `vcpkg-configuration` or introduced as an environmental variable. Path lookup order is also the same as above: first from a list in the command line, the manifest, and finally the environment.

You can add the path to a directory containing several ports or the path to the port directory itself. 

For example, `--overlay-ports=C:\custom-ports\sqlite3` refers to a single port and `--overlay-ports=C:\custom-ports` refers to a directory of ports. A valid port must contain both `vcpkg.json` and `portfile.cmake`.

Syntax:
* Command-line: Add one or multiple `--overlay-ports=<directory>` options to your vcpkg command
* [Manifest](): Populate the `"overlay-ports"` array in `vcpkg-configuration.json`
* [Environmental variable](): Set `VCPKG_OVERLAY_PORTS` to a list of paths

### Example: Overlay Ports Example

Consider the following directory structure:
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

### Example: Using overlay ports to replace system package manager

To use an overlay port to replace a dependency from the system package manager, see our [blog post]().
