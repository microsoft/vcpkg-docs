---
description: Tutorial guides the user through the process of installing and using packages with vcpkg.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: operating-systems
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 08/23/2023
ms.prod: vcpkg
---

## 1 - Install vcpkg

1. Clone the repository

    The first step is to clone the vcpkg repository from GitHub. The repository contains scripts to acquire the vcpkg executable and a registry of curated open-source libraries maintained by the vcpkg community. To do this, run:

    ```console
    git clone https://github.com/microsoft/vcpkg.git
    ```

    The vcpkg open-source registry is a set of over 2,000 free libraries that have been tested together and optimized to work smoothly with vcpkg. While the vcpkg repo does not contain the source code for these libraries, it does contain build recipes and metadata about each library. These files will be included when vcpkg is installed to your system.

2. Run the bootstrap script

    Now that you have cloned the vcpkg repository, navigate to the `vcpkg` directory and execute the bootstrap script:

    ::: zone pivot="os-windows"

    ```console
    cd vcpkg && ./bootstrap-vcpkg.bat
    ```

    ::: zone-end
    ::: zone pivot="os-linux,os-macos"

    ```console
    cd vcpkg && ./bootstrap-vcpkg.sh
    ```

    ::: zone-end

    The bootstrap script performs prerequisite checks, downloads the vcpkg executable, and manages telemetry settings.

3. Enable terminal auto-completion (optional)

    You'll want to integrate vcpkg into your terminal to enjoy tab-completion features. To do this, run:

    ::: zone pivot="os-windows"

    ```console
    ./vcpkg integrate powershell
    ```

    ::: zone-end
    ::: zone pivot="os-linux"

    ```console
    ./vcpkg integrate bash
    ```

    ::: zone-end
    ::: zone pivot="os-macos"

    ```console
    ./vcpkg integrate zsh
    ```

    ::: zone-end

    The `vcpkg integrate` command sets up shell integration for vcpkg, which adds vcpkg tab-completion support.

    That's it! vcpkg is set up and ready to use.
