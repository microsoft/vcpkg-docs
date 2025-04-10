---
title: vcpkg in MSBuild projects
description: Integrate vcpkg into an MSBuild or Visual Studio project.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: concept-article
---
# vcpkg in MSBuild projects

## Integration methods

### User-wide integration

To use vcpkg in your MSBuild projects run the following command:

```Console
vcpkg integrate install
```

You only need to run the [`vcpkg integrate
install`](../../commands/integrate.md#vcpkg-integrate-install) command the first time you want to
enable MSBuild integration. This enables MSBuild integration for all your existing and future
projects.

If you have multiple instances of vcpkg, you can use the `vcpkg integrate install` command to update
which vcpkg instance is used within MSBuild. Use [`vcpkg integrate
remove`](../../commands/integrate.md#vcpkg-integrate-remove) to remove MSBuild user-wide
integration.

This integration method automatically adds vcpkg-installed packages to the following project
properties: Include Directories, Link Directories, and Link Libraries. Additionally, this creates a
post-build action that ensures that any required DLLs are copied into the build output folder. This
works for all solutions and projects using Visual Studio 2015 or newer.

This is all you need to do for the vast majority of libraries. However, some libraries perform
conflicting behaviors, such as redefining `main()`. Since you need to choose per-project which of
these conflicting options you want, you must manually add those libraries to your linker inputs.

Here are some examples where manual linking is necessary (not an exhaustive list):

- Gtest provides `gtest`, `gmock`, `gtest_main`, and `gmock_main`
- SDL2 provides `SDL2main`
- SFML provides `sfml-main`
- Boost.Test provides `boost_test_exec_monitor`

To get a full list for all your installed packages, run `vcpkg owns manual-link`.

### Import `.props` and `.targets`

vcpkg can also be integrated into MSBuild projects by explicitly importing the
`scripts/buildsystems/vcpkg.props` and `scripts/buildsystems/vcpkg.targets` files into each
`.vcxproj`. By using relative paths, this enables vcpkg to be consumed by a submodule and
automatically acquired by users when they run `git clone`.

The easiest way to add these to every project in your solution is to create `Directory.Build.props`
and `Directory.Build.targets` files at the root of your repository.

The following examples assume they are at the root of your repository with a submodule of
`microsoft/vcpkg` at `vcpkg`.

#### Example `Directory.Build.props`

```xml
<Project>
 <Import Project="$(MSBuildThisFileDirectory)vcpkg\scripts\buildsystems\msbuild\vcpkg.props" />
</Project>
```

#### Example `Directory.Build.targets`

```xml
<Project>
 <Import Project="$(MSBuildThisFileDirectory)vcpkg\scripts\buildsystems\msbuild\vcpkg.targets" />
</Project>
```

See the [Customize your
build](/visualstudio/msbuild/customize-your-build#directorybuildprops-and-directorybuildtargets)
section of the official MSBuild documentation for more information on `Directory.Build.targets` and
`Directory.Build.props`.

### Pass MSBuild properties to triplets and portfiles

You can pass the value of MSBuild properties to vcpkg builds as environment variables by using the
[`SetEnv` task](visualstudio/msbuild/setenv-task). You must set these environment variables before the
`VcpkgTripletSelection` task.

The following example shows a MSBuild project passing the value of the `MyProp` property to vcpkg as an environment variable,
to make it usable inside triplets and portfiles.

#### Example `Directory.Build.props`

```xml
<Project>
  <PropertyGroup>
    <VcpkgRoot>C:\dev\vcpkg\</VcpkgRoot>
    <MyProp Condition="'$(Platform)' == 'x64'">X64_VALUE</MyProp>
    <MyProp Condition="'$(Platform)' == 'x86'">X86_VALUE</MyProp>
  </PropertyGroup>
 <Import Project="$(VcpkgRoot)scripts\buildsystems\msbuild\vcpkg.props" />
</Project>
```

#### Example `Directory.Build.targets`

```xml
<Project>
  <Import Project="$(VcpkgRoot)scripts\buildsystems\msbuild\vcpkg.targets" />
  <Target Name="_SetVcpkgEnvVars" BeforeTargets="VcpkgTripletSelection">
    <Message Text="Setting MY_PROP to $(MyProp)" />
    <SetEnv Name="MY_PROP" Value="$(MyProp)" Prefix="false" />
  </Target>
</Project>
```

#### Example triplet `x64-windows-custom.cmake`

```cmake
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

# Pass the environment variable to port builds
set(VCPKG_ENV_PASSTHROUGH_UNTRACKED MY_PROP)
```

#### Example portfile.cmake

```cmake
set(VCPKG_POLICY_EMPTY_PACKAGE enabled)

MESSAGE(STATUS "MY_PROP is $ENV{MY_PROP}")
```

### Linked NuGet package

> [!NOTE]
> This approach is not recommended for new projects, since it makes them difficult to share
> with others. For a portable, self-contained NuGet package, see the [`export
> command`](../../commands/export.md).

VS projects can also be integrated through a NuGet package. This will modify the project file, so we
do not recommend this approach for open source projects.

```powershell
PS D:\src\vcpkg> .\vcpkg integrate project
Created nupkg: D:\src\vcpkg\scripts\buildsystems\vcpkg.D.src.vcpkg.1.0.0.nupkg

With a project open, go to Tools->NuGet Package Manager->Package Manager Console and paste:
    Install-Package vcpkg.D.src.vcpkg -Source "D:/src/vcpkg/scripts/buildsystems"
```

> [!NOTE]
> The generated NuGet package does not contain the actual libraries. It instead acts like a
> shortcut (or symlink) to the vcpkg install and will "automatically" update with any changes
> (install/remove) to the libraries. You do not need to regenerate or update the NuGet package.

## Common Configuration

### `VcpkgEnabled` (Use Vcpkg)

This can be set to "false" to explicitly disable vcpkg integration for the project

### `VcpkgConfiguration` (Vcpkg Configuration)

If your configuration names are too complex for vcpkg to guess correctly, you can assign this
property to `Release` or `Debug` to explicitly tell vcpkg what variant of libraries you want to
consume.

### `VcpkgEnableManifest` (Use Vcpkg Manifest)

This property must be set to `true` in order to consume from a local `vcpkg.json` file. If set to
`false`, any local `vcpkg.json` files will be ignored.

This currently defaults to `false`, but will default to `true` in the future.

### <a name="vcpkgtriplet"></a> `VcpkgTriplet` (Triplet)

This property controls the triplet to consume libraries from, such as `x64-windows-static` or
`arm64-windows`.

If this is not explicitly set, vcpkg will deduce the correct triplet based on your Visual Studio
settings. vcpkg will only deduce triplets that use dynamic library linkage and dynamic CRT linkage;
if you want static dependencies or to use the static CRT (`/MT`), you will need to set the triplet
manually.

You can see the automatically deduced triplet by setting your MSBuild verbosity to Normal or higher:

> *Shortcut: Ctrl+Q "build and run"*
>
> Tools -> Options -> Projects and Solutions -> Build and Run -> MSBuild project build output
> verbosity

See also [Triplets](../triplets.md)

### <a name="vcpkghosttriplet"></a> `VcpkgHostTriplet` (Host Triplet)

This can be set to a custom triplet to use for resolving host dependencies.

If unset, this will default to the "native" triplet (x64-windows).

See also [Host dependencies](../host-dependencies.md).

### `VcpkgInstalledDir` (Installed Directory)

This property defines the location vcpkg will install and consume libraries from.

In manifest mode, this defaults to `$(VcpkgManifestRoot)\vcpkg_installed\$(VcpkgTriplet)\`. In
classic mode, this defaults to `$(VcpkgRoot)\installed\`.

### `VcpkgApplocalDeps` (App-locally deploy DLLs)

This property enables or disables detection and copying of dependent DLLs from the vcpkg installed
tree to the project output directory.

### `VcpkgXUseBuiltInApplocalDeps` (Use built-in app-local deployment)

This property, when enabled, uses vcpkg's experimental built-in app-local DLL deployment
implementation when app-locally deploying DLLs. This property will be removed and have no effect
when the built-in implementation is no longer experimental.

This property has no effect when `$(VcpkgApplocalDeps)` is false.

## Manifest mode configuration

To use [manifests](../../concepts/manifest-mode.md)
([`vcpkg.json`](../../reference/vcpkg-json.md)) with MSBuild, first you need to
use one of the integration methods above. Then, add a vcpkg.json above your
project file (such as in the root of your source repository) and set the property
`VcpkgEnableManifest` to `true`. You can set this property via the IDE in
**Project Properties** > **Vcpkg** > **Use Vcpkg Manifest**. You may need to
reload the IDE to see the vcpkg Property Page.

vcpkg will run during your project's build and install any listed dependencies to
`vcpkg_installed/$(VcpkgTriplet)/` adjacent to the `vcpkg.json` file; these libraries will then
automatically be included in and linked to your MSBuild projects.

### Known issues

- Visual Studio 2015 does not correctly track edits to the `vcpkg.json` and
  `vcpkg-configuration.json` files, and will not respond to changes unless a `.cpp` is edited.

### <a name="vcpkg-additional-install-options"></a> `VcpkgAdditionalInstallOptions` (Additional Options)

When using a manifest, this option specifies additional command line flags to pass to the underlying
vcpkg tool invocation. This can be used to access features that have not yet been exposed through
another option.

### `VcpkgManifestInstall` (Install Vcpkg Dependencies)

This property can be set to `false` to disable automatic dependency restoration during project
build. Dependencies must be manually restored via the vcpkg command line separately.
