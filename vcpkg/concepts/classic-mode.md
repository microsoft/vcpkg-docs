---
title: Classic mode
description: Use vcpkg in Classic mode to share a central instance of installed libraries.
author: vicroms
ms.author: viromer
ms.date: 02/10/2024
ms.topic: concept-article
---

# Classic mode

vcpkg has two operation modes: classic mode and
[manifest mode](manifest-mode.md). For most users, we recommend manifest mode.

In classic mode, vcpkg maintains a central *installed tree* inside the vcpkg
instance built up by individual [`vcpkg install`](../commands/install.md) and
[`vcpkg remove`](../commands/remove.md) commands. This central set of packages
can then be shared by any number of projects.

Because the set of installed packages is not associated with an individual
project, classic mode operates similarly to tools like `brew` or `apt`. However,
the set is still associated with a vcpkg instance, and each instance of vcpkg
(such as separate `git clone` copies) will have its own set of classic mode
packages installed.
