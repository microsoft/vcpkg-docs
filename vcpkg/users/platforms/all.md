---
title: Supported target platforms
description: An overview of platforms that can be targeted with vcpkg.
ms.date: 07/11/2024
ms.topic: concept-article
---
# Supported target platforms

vcpkg can build libraries for a virtually limitless number of target platforms thanks to its customizable [triplets feature](../../concepts/triplets.md). While [dozens of triplets](https://github.com/microsoft/vcpkg/tree/master/triplets) ship with vcpkg by default, including a multitude of community-provided options, you can also define your own. For instance, you can target Windows, macOS, Linux, Android, iOS, or your preferred embedded operating system. Target architectures and other compilation options can also be set using triplets.

vcpkg also supports cross-compilation to targets different than the host operating system. We include dedicated articles for some of these with more tailored guidance, including [Android](android.md), [MinGW](mingw.md), and [Xbox](xbox.md).
