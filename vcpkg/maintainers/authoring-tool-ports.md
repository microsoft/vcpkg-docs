---
title: Authoring Tool Ports
description: Learn to create a Tool Port for usage in other ports.
ms.date: 11/30/2022
---
# Authoring tool ports

Ports providing a binary or tool for other ports are tool ports. For example the `vcpkg-tool-meson` port exposes the required meson python script to run meson based builds in a versioned manner. Tool ports are generally ment to replace `vcpkg_find_acquire_program` calls since the latter is not versioned and triggers an CI world rebuild if changed.

Tool ports are implemented in different ways depending on their expected usage. 
1. If a tool is expected to be provided in binary form and not ment for the user to be used the port should be named `vcpkg-tool-<toolname>`. The port should work like `vcpkg_find_acquire_program` and only download/provide the tool if another port requests its usage. This is done via `vcpkg-port-config.cmake` which has to included all logic to find or provide the tool. The artifacts of the tool have to be put into `${DOWNLOADS}/tools/<tool_name>-<tool_version>`. (This means that the tool port is actually just a [script port](./authoring-script-ports.md))
2. If a tool is required to be build from source but is not to be used by the user the tools build artifacts should be stored into `${CURRENT_PACKAGES_DIR}/manual-tools/${PORT}`. Furthermore a `vcpkg-port-config.cmake` needs to be installed setting a variable to path of the relevant tool.
3. If a tool is expected to be used outside of vcpkg and inside the users `CMakeLists.txt`. The tools build artifacts are installed to `${CURRENT_PACKAGES_DIR}/tools/${PORT}` and the port name should follow the normal vcpkg naming rules. A `vcpkg-port-config.cmake` is required to be installed like in case 2.

Most (if not all) tool ports are required to be marked with `"supports" : "native"`. Furthermore `VCPKG_FORCE_SYSTEM_BINARIES` should be honored if possible with a version check if the system provided binary fullfills the version requirements of the tool port itself (if any).
For 1. this means to do a simple `find_program`. For case 2./3. this means not building/providing the tool from within vcpkg but still fullfilling the set variables in `vcpkg-port-config.cmake` of the tool port.

If a tool port is a dependency in another port, the tool port should is requried to be marked with `"host" : true`
