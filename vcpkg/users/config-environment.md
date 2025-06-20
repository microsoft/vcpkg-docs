---
title: Environment variables
description: Use environment variables to control how vcpkg works and where it looks for files.
ms.date: 01/10/2024
ms.topic: reference
---
# Environment variables

## VCPKG_DOWNLOADS

This environment variable can be set to an existing directory to use for storing downloads instead of the internal
`downloads/` directory. It should always be set to an absolute path.

## VCPKG_FEATURE_FLAGS

This environment variable can be set to a comma-separated list of off-by-default features in vcpkg. These features are
subject to change without notice and should be considered highly unstable.

There are no off-by-default feature flags at this time.

## EDITOR

This environment variable can be set to the full path of an executable to be used for `vcpkg edit`. Please see
`vcpkg help edit` for command-specific help.

## VCPKG_ROOT

This environment variable can be set to a directory to use as the root of the vcpkg instance. This will only be used if
the vcpkg executable is not located within a valid root and the command line switch `--vcpkg-root` is unused.

## VCPKG_VISUAL_STUDIO_PATH

This environment variable can be set to the full path to a Visual Studio instance on the machine. This Visual Studio instance
will be used if the triplet does not override it via the [`VCPKG_VISUAL_STUDIO_PATH`](triplets.md#vcpkg_visual_studio_path) triplet setting.

Example: `D:\2017`

## VCPKG_DEFAULT_TRIPLET

This environment variable can be set to a triplet name which will be used for unqualified triplet references in command lines.

## VCPKG_DEFAULT_HOST_TRIPLET

This environment variable can be set to a triplet name which will be used for unqualified host port references in command lines and all host port references in dependency lists. See [the host-dependencies documentation](host-dependencies.md) for more information.

## VCPKG_OVERLAY_PORTS

This environment variable adds additional [overlay ports or overlay port directories](../concepts/overlay-ports.md) considered after those listed on the command line. Multiple values are separated with the platform dependent PATH separator (Windows `;` | others `:`)


Example (Windows): `C:\custom-ports\boost;C:\custom-ports\sqlite3;C:\other-ports`

## VCPKG_OVERLAY_TRIPLETS

This environment variable allows users to add directories to search for triplets.

List paths to overlays using the platform dependent PATH separator (Windows `;`, others `:`)

See [Example: overlay triplets](../users/examples/overlay-triplets-linux-dynamic.md) for an example.

## VCPKG_FORCE_SYSTEM_BINARIES

This environment variable, if set, suppresses the downloading of CMake and Ninja and forces the use of the system binaries.

## VCPKG_FORCE_DOWNLOADED_BINARIES

This environment variable, if set, ignores the use of the system binaries and will always download and use the version defined by vcpkg.

## VCPKG_KEEP_ENV_VARS

This environment variable can be set to a list of environment variables, separated by `;`, which will be propagated to
the build environment.

The values of the kept variables will not be tracked in package ABIs and will not cause rebuilds when they change. To
pass in environment variables that should cause rebuilds on change, see [`VCPKG_ENV_PASSTHROUGH`](triplets.md#vcpkg_env_passthrough).

Example: `FOO_SDK_DIR;BAR_SDK_DIR`

## VCPKG_MAX_CONCURRENCY

This environment variables limits the amount of concurrency used by underlying buildsystems. If unspecified, this defaults to logical cores + 1.

## VCPKG_DEFAULT_BINARY_CACHE

This environment variable redirects the default location to store binary packages. See [Binary Caching][] for more details.

## VCPKG_BINARY_SOURCES

This environment variable adds or removes binary sources. See [Binary Caching][] for more details.

## VCPKG_NUGET_REPOSITORY

This environment variable changes the metadata of produced NuGet packages. See [Binary Caching](../reference/binarycaching.md#nuget) for more details.

## VCPKG_USE_NUGET_CACHE

This environment variable allows using NuGet's cache for every nuget-based binary source. See [Binary Caching](../reference/binarycaching.md#nuget) for more details.

## X_VCPKG_NUGET_PREFIX

Adds a prefix to the name of all the binary packages pushed or restored from
[NuGet binary caches](../reference/binarycaching.md#nuget).

For example, when `X_VCPKG_NUGET_PREFIX` is set to `vcpkg_demo-` the
`zlib_x64-windows.1.2.13-vcpkg8918746ce8b60474e5ebe68e53355fa70eb05119be913a1d1dc0b930b3b7b6e8.nupkg`
binary package becomes
`vcpkg_demo-zlib_x64-windows.1.2.13-vcpkg8918746ce8b60474e5ebe68e53355fa70eb05119be913a1d1dc0b930b3b7b6e8.nupkg`.

## X_VCPKG_ASSET_SOURCES

[!INCLUDE [experimental](../../includes/experimental.md)]

This environment variable allows using a private mirror for all SHA512-tagged assets. See [Asset Caching](assetcaching.md) for more details.

## VCPKG_NO_CI

Setting `VCPKG_NO_CI` disables vcpkg's CI environment detection heuristics.

[Binary Caching]: ../reference/binarycaching.md

## VSLANG

This environment variable sets the language vcpkg uses to display messages. It should be set to one of the 14 supported LCIDs (locale identifier, 4-byte value corresponding to a language).

For example: 1033 corresponds to the English (US) language.
For a full list of supported LCIDs see [Localization](https://github.com/microsoft/vcpkg-tool/blob/main/docs/localization.md).
