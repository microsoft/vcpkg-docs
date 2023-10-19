---
title: "Tutorial: Adding ports to the vcpkg registry"
description: Tutorial guides the user through the process of adding a port to the vcpkg registry.
zone_pivot_group_filename: zone-pivot-groups.json
zone_pivot_groups: shell-selections
author: JavierMatosD
ms.author: javiermat
ms.topic: tutorial
ms.date: 10/17/2023
ms.prod: vcpkg
#CustomerIntent: As a beginner C++ developer, I want to learn how to add a port to the vcpkg registry.
---

# Tutorial: Adding a port to vcpkg registry

This tutorial guides you on how to package a library for vcpkg using a custom overlay. We recommended that you read the [Packaging a library](get-started-packaging.md) tutorial before proceeding. 

## Prerequisites

- A terminal
- [Git](https://git-scm.com/downloads)

## 1 - Fork the vcpkg Repository

1. Navigate to the [vcpkg GitHub repository](https://github.com/vcpkg).
2. Click the "Fork" button at the top right corner of the page.
3. Choose your GitHub account as the destination for the fork.

## 2 - Add your fork as a remote

Navigate to the directory where you've cloned the original vcpkg repository, then add your fork as a remote. Replace <Your-GitHub-Username> with your GitHub username:

```console
git remote add myfork https://github.com/<Your-GitHub-Username>/vcpkg.git
```

You cant check that the remote was added correctly by running:

```console
git remote -v
```

You should see `myfork` listed among the remotes.

## 3 - Create a topic branch

Before making changes, it's usually good to create a new branch:

```console
git checkout -b <topic-branch-name>
```

## 4 - Copy overlay port

Add the `vcpkg-sample-library` port that was created in the [packaging a library](get-started-packaging.md) tutorial to the `vcpkg\ports` directory:

```console
cp -R <path/to/vcpkg-sample-library> ports/vcpkg-sample-library
```

## 5 - Commit and push changes

1. Commit the changes:
   
   ```console
    git add ports/<library-name>
    git commit -m "Add vcpkg-sample-library to vcpkg"
   ```

2. Run `vcpkg x-add-version`:
   ```console
   vcpkg x-add-version vcpkg-sample-library
   ```

3. Push the changes to your fork:
   
   ```console
   git push myfork <topic-branch-name>
   ```

## 6 - Create a pull request

1. Navigate to your forked repository on GitHub.
2. Click on "Compare & pull request" button.
3. Verify the changes, add a descriptive title and comments, and then click "Create pull request."

That's it! You've successfully added a `port` to vcpkg's curated registry.