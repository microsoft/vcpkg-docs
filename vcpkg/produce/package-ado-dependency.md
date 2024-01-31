---
title: "Tutorial: Package ADO dependency"
description: In this tutorial we show you how to package and ADO dependency for vcpkg.
author: JavierMatosD
ms.author: javiermat
ms.date: 01/30/24
ms.topic: tutorial
# CustomerIntent: As an advanced vcpkg user I want package a private ADO dependency.
---
# Tutorial: Package a private ADO dependency


## Prerequisites:

## 1 - Create a private project

TODO: tell user to create their own organization if needed, then show them how to create a private project

## 2 - Set up Authentication in ADO

1. Generate SSH Key Pair

```bash
ssh-keygen -t rsa -b 4096 -C "ADO-RSA" -f ~/.ssh/id_rsa_ado
```

-t rsa: Specifies the type of key to create, in this case, RSA.
-b 4096: Sets the number of bits in the key, in this case, 4096, which is considered strong and secure.
-C "ADO-RSA": Adds a label to the key for identification, which can be particularly useful when you have multiple keys.
-f ~/.ssh/id_rsa_ado: Specifies the filename for the new key. This command saves the private key to id_rsa_ado and the public key to id_rsa_ado.pub in your ~/.ssh directory.

ssh-keygen -t rsa -b 4096 -C "ADO-RSA" -f ~/.ssh/id_rsa_ado    
Generating public/private rsa key pair.
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /Users/javiermatos/.ssh/id_rsa_ado
Your public key has been saved in /Users/javiermatos/.ssh/id_rsa_ado.pub
The key fingerprint is:
SHA256:8Yc3uyH5wj0d1nENEGncUGYtJd7oM0NB+hN0Fqj9IYg ADO-RSA
The key's randomart image is:
+---[RSA 4096]----+
|           .+OX+=|
|            +B+B.|
|        . ..= =oo|
|         E + * +o|
|        S o + X.+|
|           + oo*.|
|         .o.oo . |
|          oooo.  |
|           .o.   |
+----[SHA256]-----+

2. Add SSH key to Azure Dev Ops
    - Open the id_rsa_ado.pub file with a text editor to view the public key.
    - Copy the entire content of the file.
    - Navigate to your user settings, find the SSH public keys section, and add a new key by pasting the copied content.
    - Include Screenshot!
    - Add the key -> Include screenshot!

3. Load SSH key into the SSH agent.

```bash
ssh-add ~/.ssh/id_rsa_ado    
```

4. Test connection

```bash
 ssh -T git@ssh.dev.azure.com
```

Output:
ssh -T git@ssh.dev.azure.com
remote: Shell access is not supported.
shell request failed on channel 0
 
## 3 - Upload library to ADO repo

1. initialize git repo locally

```bash
cd path/to/your/source-code
git init
```

2. Commit your source code

```bash
git add .
git commit -m "Initial commit of the sample library"
```

3. Retrieve the ssh url from ADO

TODO: add screenshot!

4. Add remote

```bash
git remote add origin git@ssh.dev.azure.com:v3/javiermat/cmake-sample-lib/cmake-sample-lib
```

5. Push library

```bash
git push -u origin master:main
```

## 4 - Package libraby into an overlay port

1. Create an overlay ports directory

```bash
mkdir vcpkg-overlay-ports
cd vcpkg-overlay-ports
```

2. Set up the port files

    - `vcpkg.json`
    ```json
    {
    "name": "vcpkg-sample-library",
    "version": "1.0.0",
    "homepage": "https://github.com/microsoft/vcpkg-docs/tree/cmake-sample-lib",
    "description": "A sample C++ library designed to serve as a foundational example for a tutorial on packaging libraries with vcpkg.",
    "license": "MIT",
    "dependencies": [
    {
        "name" : "vcpkg-cmake",
        "host" : true
    },
    {
        "name" : "vcpkg-cmake-config",
        "host" : true
    },
    "fmt"
    ]
    }
    ```
    The vcpkg.json file serves as a manifest that defines metadata and dependencies for a C++ library, providing vcpkg with the necessary information to build, install, and manage the package.
name: Specifies the name of the library. This is used as the package identifier.
version: Indicates the version number of the library.
homepage: URL to the project's homepage, often its repository. Useful for those who want to know more or contribute.
description: Brief text describing what the library does. This is for documentation and users.
license: Specifies the license under which the library is distributed.
dependencies: An array containing the list of dependencies that the library needs.
name: vcpkg-cmake: Specifies a dependency on vcpkg-cmake, which provides CMake functions and macros commonly used in vcpkg ports.
host: true: Specifies that vcpkg-cmake is a host dependency, meaning it's required for building the package but not for using it.
name: vcpkg-cmake-config: Specifies a dependency on vcpkg-cmake-config, which assists in using CMake config scripts.
fmt: Specifies a run-time dependency on the fmt library. This means fmt is required for both building and using the package.
For more information on vcpkg.json, see the following documentation on manifests.
    - `portfile.cmake`
    ```cmake
    vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

    vcpkg_from_git(
        OUT_SOURCE_PATH SOURCE_PATH
        URL git@ssh.dev.azure.com:v3/javiermat/cmake-sample-lib/cmake-sample-lib
        REF d8a36764e26379277a9765827aaefc6cebd8b7d3
    )

    vcpkg_cmake_configure(
        SOURCE_PATH "${SOURCE_PATH}"
    )

    vcpkg_cmake_install()

    vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib")

    file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

    file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
    ```

    This portfile defines how to download, build, install, and package a specific C++ library from GitHub using vcpkg.
vcpkg_check_linkage(ONLY_STATIC_LIBRARY): Specifies that only static linking is supported for this package.
vcpkg_from_github: Starts the function to download the source code from a GitHub repository.
OUT_SOURCE_PATH SOURCE_PATH: Sets the directory where the source code will be extracted.
REPO JavierMatosD/vcpkg-sample-library: The GitHub repository containing the source code.
REF "${VERSION}": The version of the source code to download.
SHA512 0: Placeholder for the SHA-512 hash of the source code for integrity verification.
HEAD_REF main: Specifies the default branch for the repository.
vcpkg_cmake_configure: Configures the project using CMake, setting up the build.
SOURCE_PATH "${SOURCE_PATH}": The path to the source code downloaded earlier.
vcpkg_cmake_install(): Builds and installs the package using CMake.
vcpkg_cmake_config_fixup(PACKAGE_NAME "my_sample_lib"): Fixes the CMake package configuration files to be compatible with Vcpkg.
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include"): Deletes the include directory from the debug installation to prevent overlap.
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION ...): Installs the LICENSE file to the package's share directory and renames it to copyright.
configure_file("${CMAKE_CURRENT_LIST_DIR}/usage" ...): Copies a usage instruction file to the package's share directory.
For more information, refer to the maintainer guide.

To get REF
mkdir temp
git init
git fetch git@ssh.dev.azure.com:v3/javiermat/cmake-sample-lib/cmake-sample-lib main --depth 1 -n
git rev-parse FETCH_HEAD
`d8a36764e26379277a9765827aaefc6cebd8b7d3`

3. Install your port

```bash
 vcpkg install vcpkg-sample-library --overlay-ports=vcpkg-overlay-ports
```


## Next Steps

And that's it! You have packaged your private ADO dependency.
