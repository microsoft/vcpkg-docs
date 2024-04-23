---
title:  Triplets
description: This article explains the concept of triplets in vcpkg and their capabilities.
author: vicroms
ms.author: viromer
ms.date: 01/11/2024
ms.topic: concept-article
---

# Triplets

*Triplet* is a standard term used in cross-compiling as a way to completely
capture the target environment (CPU, OS, compiler, runtime, etc.) in a single,
convenient name.

In vcpkg, triplets describe an imaginary "target configuration set" for every
library. Within a triplet, libraries are generally built with the same
configuration, but it is not a requirement. For example, you could have one
triplet that builds `openssl` statically and `zlib` dynamically, one that
builds them both statically, and one that builds them both dynamically (all for
the same target OS and architecture).

A single build will consume files from up to two triplets: the target triplet
and the host triplet. If you need to apply different settings for different
libraries, you must make a single custom triplet with that combination of
settings. See [per-port
customization](../users/triplets.md#per-port-customization) for how to accomplish this.

vcpkg comes with pre-defined triplets for many common platforms and
configurations. Run `vcpkg help triplet` to get the list of available in your
environment.

## Triplet selection

To select a target triplet:

* In [classic mode](../concepts/classic-mode.md):
  * Qualify package references with the triplet name, such as `zlib:x64-windows-static-md`.
  * Pass [`--triplet=<triplet>`](../commands/common-options.md#triplet).
* In CMake:
  * Set [`VCPKG_TARGET_TRIPLET`](../users/buildsystems/cmake-integration.md#vcpkg_target_triplet).
* In MSBuild:
  * Set [`VcpkgTriplet`](../users/buildsystems/msbuild-integration.md#vcpkgtriplet).

To select the host triplet for the current machine:

* In [classic mode](../concepts/classic-mode.md):
  * Pass [`--host-triplet=<triplet>`](../commands/common-options.md#host-triplet).
* In CMake:
  * Set [`VCPKG_HOST_TRIPLET`](../users/buildsystems/cmake-integration.md#vcpkg_host_triplet).
* In MSBuild:
  * Set [`VcpkgHostTriplet`](../users/buildsystems/msbuild-integration.md#vcpkghosttriplet).

## Community triplets

Triplets contained in the `triplets\community` folder are not tested by the
curated registry's continuous integration, but are commonly requested by the
community. Because we do not have continuous coverage, port updates may break
compatibility with community triplets. We gladly accept and review contributions
that aim to solve issues with these triplets.

When using a community triplet, a message like the following one will be printed
during a package installation:

```console
-- Using community triplet x86-uwp. This triplet configuration is not guaranteed to succeed.
-- [COMMUNITY] Loading triplet configuration from: D:\src\vcpkg\triplets\community\x86-uwp.cmake
```

## <a name="custom-triplets"></a> Add or replace triplets

You can extend vcpkg by replacing in-the-box triplets or creating new triplets
for your particular project.

First, copy a built-in triplet file from the `triplets\` directory into a
different filesystem location. Then, add that directory to the list of overlay
triplet paths when interacting with vcpkg.

* In [Manifest mode](../concepts/manifest-mode.md), you can use
  [`$.vcpkg-configuration.overlay-triplets`](../reference/vcpkg-configuration-json.md#overlay-triplets).
* When using vcpkg from CMake, you can set [`VCPKG_OVERLAY_TRIPLETS`](../users/buildsystems/cmake-integration.md#vcpkg_overlay_triplets).
* When using vcpkg from MSBuild, you can add [`--overlay-triplets=...`][overlay-triplets] to [MSBuild Additional Options](../users/buildsystems/msbuild-integration.md#vcpkg-additional-install-options).
* When using the CLI directly, you can pass [`--overlay-triplets=...`][overlay-triplets].
* You can set the [`$VCPKG_OVERLAY_TRIPLETS`](../users/config-environment.md#vcpkg_overlay_triplets) environment variable to a list of overlay paths.

See our [overlay triplets example](../users/examples/overlay-triplets-linux-dynamic.md)
for a more detailed walkthrough.

[overlay-triplets]: ../commands/common-options.md#overlay-triplets

## Remarks

The default triplet when running any vcpkg command is `%VCPKG_DEFAULT_TRIPLET%`
or a platform-specific choice if that environment variable is undefined.

* Windows: `x64-windows`
* Linux: `x64-linux`
* OSX: `x64-osx`

We recommend using a systematic naming scheme when creating new triplets. The
Android toolchain naming scheme is a good source of inspiration:
<https://developer.android.com/ndk/guides/standalone_toolchain.html>.
