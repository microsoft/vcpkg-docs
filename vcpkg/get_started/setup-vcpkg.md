1. Clone the repository

    The first step is to clone the vcpkg repository from GitHub. The repository contains scripts to acquire the vcpkg executable and a registry of curated open-source libraries maintained by the vcpkg community. To do this, run:

    ```console
    git clone https://github.com/microsoft/vcpkg.git
    ```

    The vcpkg open-source registry is a set of over 2,000 free libraries that have been tested together and optimized to work smoothly with vcpkg. While the vcpkg repo does not contain the source code for these libraries, it does contain build recipes and metadata about each library.

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

    The bootstrap script performs prerequisite checks and downloads the vcpkg executable.

    That's it! vcpkg is set up and ready to use.
