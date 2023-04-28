---
title: Xbox
description: Use x64 community triplets for Xbox development.
ms.date: 04/27/2023
---
# Xbox

> [!NOTE]
> XBox is not tested as part of vcpkg repository's CI process, so regressions can occur as part of library updates. PRs improving support are welcome!

## Xbox community triplets

vcpkg includes [community triplets](https://github.com/microsoft/vcpkg/tree/master/triplets/community) for [Microsoft GDK](https://aka.ms/gdk) PC and [Xbox](https://aka.ms/gdkx) development. They make use of the Visual C++ compiler toolset (VS 2019 16.11 or later).

There are two variants of each triplet, selecting between static and dynamic linking.

| Console          | vcpkg community triplets     |
|------------------|------------------------------|
| Xbox One         | x64-xbox-xboxone             |
|                  | x64-xbox-xboxone-static      |
| Xbox Series X\|S | x64-xbox-scarlett            |
|                  | x64-xbox-scarlett-static     |

## Building with the Windows SDK

The Windows SDK (19041) or later includes the system headers required to build against the ``WINAPI_FAMILY_GAMES`` API partition used by the Xbox Game OS. Therefore, most libraries can be built against these triplets without having access to the NDA *Microsoft GDK with Xbox Extensions*.

## Building with the Microsoft GDK with Xbox Extensions

Libraries that make use of DirectX graphics APIs must be built using a custom variant of Direct3D 12.X used by the Xbox Game OS, and this reuqires access to the NDA *Microsoft GDK with Xbox Extensions*. Note that the Xbox Game OS doe snot support Direct3D 11, Direct2D/DirectWrite, OpenGL, GDI, or any older version of Direct3D.

For libraries that require these headers, they make use of **ms-gdkx** "stub" port to indicate this requirement. This port looks for paths pointed to by ``GameDKLatest``, ``GameGXDKLatest``, and ``GameGRDKLatest`` environment variables.

> Note that Xbox developers are responsible for ensuring they and their publisher have appropriate and correct licensing terms for use of any 3rd party software components per the terms of their Title License Agreement. Be sure to seek appropriate legal advice for any use of open-source components in your title.

## Further reading

For more information, see [this blog post](https://walbourn.github.io/vcpkg-now-supports-targeting-xbox/).
