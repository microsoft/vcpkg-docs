---
title: vcpkg upgrade
description: Command line reference for the vcpkg upgrade command.
ms.date: 01/10/2024
---
# vcpkg upgrade

## Synopsis

```console
vcpkg upgrade [options]
```

## Description

Removes and reinstalls [Classic mode] packages that have different current available versions.

> [!WARNING]
> This command does not save the previous install state. If an error occurs while building replacement packages, the previous set of packages will not be restored.
>
> For robust version management, use [Manifest mode].

By default, `upgrade` prints the plan of what will be modified. Pass [`--no-dry-run`](#no-dry-run) to perform that plan.

## Example

```console
$ vcpkg upgrade
The following packages will be rebuilt:
  * corrade[core,interconnect,pluginmanager,testsuite,utility]:x64-windows -> 2020.06#5
  * magnum[al-info,anyaudioimporter,anyimageconverter,anyimageimporter,anysceneconverter,anysceneimporter,anyshaderconverter,audio,cglcontext,core,debugtools,distancefieldconverter,eglcontext,fontconverter,gl,gl-info,glfwapplication,glxcontext,imageconverter,magnumfont,magnumfontconverter,meshtools,objimporter,opengltester,primitives,sceneconverter,scenegraph,sdl2application,shaderconverter,shaders,shadertools,text,texturetools,tgaimageconverter,tgaimporter,trade,wglcontext,windowlesscglapplication,windowlesseglapplication,windowlessglxapplication,windowlesswglapplication]:x64-windows -> 2020.06#12
  * openal-soft[core]:x64-windows -> 1.23.0
  * ragel[core]:x64-windows -> 6.10#5
  * sdl2[base,core]:x64-windows -> 2.26.4
Additional packages (*) will be modified to complete this operation.
If you are sure you want to rebuild the above packages, run this command with the --no-dry-run option.
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### <a name="no-dry-run"></a> `--no-dry-run`

Removes the packages that are outdated and reinstalls them at the versions listed in the user's local copy of the public vcpkg registry.

### `--no-keep-going`

Stop at the first failure.

By default, on a package install failure, vcpkg will continue to attempt to install other unrelated packages.

### `--allow-unsupported`

Allow performing upgrades to unsupported packages.

[Classic mode]: ../users/classic-mode.md
[Manifest mode]: ../users/manifests.md
