---
title: Classic mode
description: Use vcpkg in Classic mode to share a central instance of installed libraries.
ms.date: 11/30/2022
ms.topic: concept-article
---
# Classic mode

vcpkg has two modes for consuming packages - Classic mode and [Manifest mode](manifests.md). For most users, we recommend Manifest mode.

In Classic mode, vcpkg maintains a central *installed tree* inside the vcpkg instance built up by individual [`vcpkg install`](../commands/install.md) and [`vcpkg remove`](../commands/remove.md) commands. This central set of packages can then be shared by any number of projects.

Because the set of installed packages is not associated with an individual project, Classic mode operates similarly to tools like `brew` or `apt`. However, the set is still associated with a vcpkg instance, and each instance of vcpkg (such as separate `git clone` copies) will have its own set of classic mode packages installed.
