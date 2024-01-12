---
title: vcpkg help
description: Reference for the vcpkg help command. Provides detailed information and help topics for vcpkg commands.
author: JavierMatosD
ms.author: javiermat
ms.date: 09/14/2023
---

# vcpkg help

## Synopsis

```console
vcpkg help [topic]
```

## Description

The help command displays information about various vcpkg commands and topics on the terminal. You can specify a particular topic to get information about or use it without any arguments to see a list of all available commands.

### Examples

For a list of all commands:

```Console
> vcpkg help

usage: vcpkg <command> [--switches] [--options=values] [arguments] @response_file
  @response_file         Contains one argument per line expanded at that location

Package Installation:
  export                 Creates a standalone deployment of installed ports
  install                Installs a package
  remove                 Uninstalls a package
  x-set-installed        Installs, upgrades, or removes packages such that that installed matches
                         exactly those supplied
  upgrade                Rebuilds all outdated packages

Package Discovery:
  x-check-support        Tests whether a port is supported without building it
  depend-info            Displays a list of dependencies for ports
  list                   Lists installed libraries
  owns                   Searches for the owner of a file in installed packages
  x-package-info         Display detailed information on packages
  portsdiff              Diffs changes in port versions between commits
  search                 Searches for packages available to be built
  update                 Lists packages that can be upgraded

Package Manipulation:
  add                    Adds dependency to manifest
  x-add-version          Adds a version to the version database
  create                 Creates a new port
  edit                   Edits a port, optionally with %EDITOR%, defaults to "code"
  env                    Creates a clean shell environment for development or compiling
  format-manifest        Prettyfies vcpkg.json
  hash                   Gets a file's SHA256 or SHA512
  x-init-registry        Creates a blank git registry
  new                    Creates a new manifest
  x-update-baseline      Updates baselines of git registries in a manifest to those registries' HEAD
                         commit

Other:
  ci                     Tries building all ports for CI testing
  x-ci-verify-versions   Checks integrity of the version database
  contact                Displays contact information to send feedback
  fetch                  Fetches something from the system or the internet
  integrate              Integrates vcpkg with machines, projects, or shells

For More Help:
  help topics            Displays full list of help topics
  help <topic>           Displays specific help topic
  help commands          Displays full list of commands, including rare ones not listed here
  help <command>         Displays help detail for <command>

For more help (including examples) see https://learn.microsoft.com/vcpkg
```

For help with a specific command:

```Console
> vcpkg help add

Synopsis: Adds dependency to manifest
Examples:
  vcpkg add port <port name>
  vcpkg add port png
  vcpkg add artifact <artifact name>
  vcpkg add artifact cmake
Options:
  --x-asset-sources=...  Asset caching sources. See 'vcpkg help assetcaching'
  --binarysource=...     Binary caching sources. See 'vcpkg help binarycaching'
  --x-buildtrees-root=...
                         Buildtrees directory (experimental)
  --downloads-root=...   Downloads directory (default: %VCPKG_DOWNLOADS%)
  --host-triplet=...     Host triplet. See 'vcpkg help triplet' (default:
                         %VCPKG_DEFAULT_HOST_TRIPLET%)
  --x-install-root=...   Installed directory (experimental)
  --overlay-ports=...    Directories of overlay ports (also: %VCPKG_OVERLAY_PORTS%)
  --overlay-triplets=... Directories of overlay triplets (also: %VCPKG_OVERLAY_TRIPLETS%)
  --x-packages-root=...  Packages directory (experimental)
  --triplet=...          Target triplet. See 'vcpkg help triplet' (default: %VCPKG_DEFAULT_TRIPLET%)
  --vcpkg-root=...       The vcpkg root directory (default: %VCPKG_ROOT%)
  --version=...          A version or version range to match; only valid for artifacts
```

For help with a specific topic:

```Console
> vcpkg help versioning

Versioning allows you to deterministically control the precise revisions of dependencies used by
your project from within your manifest file.

The following versioning schemes are accepted.:
  version                A dot-separated sequence of numbers (1.2.3.4)
  version-date           A date (2021-01-01.5)
  version-semver         A Semantic Version 2.0 (2.1.0-rc2)
  version-string         An exact, incomparable version (Vista)

Each version additionally has a "port-version" which is a nonnegative integer. When rendered as
text, the port version (if nonzero) is added as a suffix to the primary version text separated by a
hash (#). Port-versions are sorted lexicographically after the primary version text, for example:
1.0.0 < 1.0.0#1 < 1.0.1 < 1.0.1#5 < 2.0.0

Manifests can place three kinds of constraints upon the versions used:
  builtin-baseline       The baseline references a commit within the vcpkg repository that
                         establishes a minimum version on every dependency in the graph. For
                         example, if no other constraints are specified (directly or transitively),
                         then the version will resolve to the baseline of the top level manifest.
                         Baselines of transitive dependencies are ignored.

  version>=              Within the "dependencies" field, each dependency can have a minimum
                         constraint listed. These minimum constraints will be used when transitively
                         depending upon this library. A minimum port-version can additionally be
                         specified with a '#' suffix.

  overrides              When used as the top-level manifest (such as when running `vcpkg install`
                         in the directory), overrides allow a manifest to short-circuit dependency
                         resolution and specify exactly the version to use. These can be used to
                         handle version conflicts, such as with `version-string` dependencies. They
                         will not be considered when transitively depended upon.

Vcpkg will select the minimum version found that matches all applicable constraints, including the
version from the baseline specified at top-level as well as any "version>=" constraints in the
graph.
The best approach to keep your libraries up to date is to update your baseline reference. This will
ensure all packages, including transitive ones, are updated. However if you need to update a package
independently, you can use a "version>=" constraint.
Additionally, package publishers can use "version>=" constraints to ensure that consumers are using
at least a certain minimum version of a given dependency. For example, if a library needs an API
added to boost-asio in 1.70, a "version>=" constraint will ensure transitive users use a sufficient
version even in the face of individual version overrides or cross-registry references.
Example manifest:
{
    "builtin-baseline": "a14a6bcb27287e3ec138dba1b948a0cdbc337a3a",
    "dependencies": [
        { "name": "zlib", "version>=": "1.2.11#8" },
        "rapidjson"
    ],
    "overrides": [
        { "name": "rapidjson", "version": "2020-09-14" }
    ]
}
Extended documentation available at 'https://learn.microsoft.com/vcpkg/users/versioning'.
```
## Options

All vcpkg commands support a set of [common options](common-options.md).

