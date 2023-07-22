---
title: Publishing to a Git registry
description: Learn to publish vcpkg ports to a Git-based registry
author: vicroms
ms.author: viromer
ms.date: 7/21/2023
ms.prod: vcpkg
---
# Publishing to a Git registry

## Step 1: Create a Git registry

The first step to create a Git-based registry is to create a Git repository:

```Console
mkdir vcpkg-registry
cd vcpkg-registry
git init
```

## Step 2: Create the registry folders

Git-based registries are actually just Git repositories with a specific layout that vcpkg expects.
A vcpkg Git-based registry requires the following:

* a folder named `ports` to contain the registry's ports,
* a folder named `versions` to contain the versions database,
* a file named `baseline.json`  in the `versions` folder

Your repository can contain any other files you want, for example: scripts, a LICENSE file, a README
file, documentation, etc. These files will be not be used by vcpkg.

In this step we will create these requirements and add them to the Git registry.

```Console
mkdir ports
mkdir versions
```

Next, create a file named `baseline.json` in the `versions` folder with the following contents:

```json
{
  "default": {}
}
```

Finally, let's commit these changes to the repository.

```Console
git add .
git commit -m "Initial commit"
```

Optionally, if you have a remote server, this is a good point to commit your changes.

```Console
git remote add origin https://example.com/vcpkg-registry.git
git push --set-upstream origin HEAD
```

## Step 3: Add ports to the registry

In this step you should add all your ports in the `ports` folder. Each port must have.
its own subdirectory containing at least a `vcpkg.json` file and a `portfile.cmake` file.
Learn more about creating ports in the tutorial on [packaging GitHub
repositories](../examples/packaging-github-repos.md).

When all your ports have been added commit your changes to the repository.

```Git
git add ports/.
git commit -m "Add registry ports"
```

## Step 4: Update the versions database

> [!NOTE]
> This step assumes that you're familiar with some versioning concepts.
> Read the [versioning documentation](../users/versioning.md) to learn about the
> versions database and baseline file.

Let's assume that you have the following ports in the `ports` directory.

```Console
ls ports
beicode/ fmt/
```

The next step is to generate the version database files for your ports and
also update the `baseline.json` file. To do that, let's leverage the
`vcpkg x-add-version` command.

From the root of your registry run the following:

```Console
vcpkg --x-builtin-ports-root=./ports --x-builtin-registry-versions-dir=./versions x-add-version --all --verbose
```

Which produces the output:

```Console
added version 1.0.0 to /home/vcpkg/vcpkg-registry/versions/b-/beicode.json (new file)
added version 1.0.0 to /home/vcpkg/vcpkg-registry/versions/baseline.json
added version 10.0.0 to /home/vcpkg/vcpkg-registry/versions/f-/fmt.json (new file)
added version 10.0.0 to /home/vcpkg/vcpkg-registry/versions/baseline.json
```

The `x-add-version` command updates the versions database,
it is normally used for the official vcpkg registry in <https://github.com/Microsoft/vcpkg>.
But by adding some experimental options it can be made to work on any Git-based registry.

It is important that all pending changes for your ports have been comitted to the registry's
repository before running the command.

The `--x-builtin-ports-root=./ports` and `--x-builtin-registry-versions-dir=./versions`, redirect
vcpkg to treat your registry's `ports` and `versions` folders as if they were the built-in ones (by
built-in we refer to the ports registry that ships with the vcpkg repository). The `--all` options
indicates that the command should scan the `ports` folder for all the ports it can find, and the
`--verbose` option makes it so that the command produces output back to the terminal.

The result of this command is that version database files for each port are created or updated as
necessary, at the same time the `versions/baseline.json` file is updated with the newly added
versions.
