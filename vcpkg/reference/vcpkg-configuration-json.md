---
title: vcpkg-configuration.json Reference
description: Reference documentation for the vcpkg-configuration.json file format
ms.date: 01/10/2024
ms.topic: reference
---
# vcpkg-configuration.json Reference

The `vcpkg-configuration.json` file forms part of a project's
[manifest](../concepts/manifest-mode.md), along with
[`vcpkg.json`](vcpkg-json.md). All fields in the `vcpkg-configuration.json` file
are only used from the top-level project -- the `vcpkg-configuration.json` files
in any dependencies are ignored.

In [Manifest Mode](../concepts/manifest-mode.md), `vcpkg-configuration.json` can
be in a separate file beside [`vcpkg.json`](vcpkg-json.md) or it can be embedded
in the [`"vcpkg-configuration" field`](vcpkg-json.md#vcpkg-configuration).

In [Classic Mode](../concepts/classic-mode.md), vcpkg will use the
`vcpkg-configuration.json` file in the
[root](../commands/common-options.md#vcpkg-root) of the vcpkg instance.

For an overview of using registries with vcpkg, see [Using Registries](../consume/git-registries.md).

The latest JSON Schema is available at [https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg-configuration.schema.json](https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg-configuration.schema.json). IDEs with JSON Schema support such as Visual Studio and Visual Studio Code can use this file to provide autocomplete and syntax checking. For most IDEs, you should set `"$schema"` in your `vcpkg-configuration.json` to this URL.

## Example

```json
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg-configuration.schema.json",
  "default-registry": {
    "kind": "git",
    "repository": "https://internal/mirror/of/github.com/Microsoft/vcpkg",
    "baseline": "eefee7408133f3a0fef711ef9c6a3677b7e06fd7"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/MicrosoftDocs/vcpkg-docs",
      "reference": "vcpkg-registry",
      "baseline": "768f6a3ad9f9b6c4c2ff390137690cf26e3c3453",
      "packages": [ "beicode", "beison" ]
    }
  ],
  "overlay-ports": [
    "./team-ports",
    "./custom-ports"
   ],
  "overlay-triplets": [ "./my-triplets" ]
}
```

This example adds a private registry, `https://github.com/MicrosoftDocs/vcpkg-docs/tree/vcpkg-registry`, as the source for the libraries `beicode` and `beison`. All other ports are found from an internal mirror of the Curated Catalog hosted at `https://internal/mirror/of/github.com/Microsoft/vcpkg`.

The example also configures custom overlays for ports and triplets that are present in the source code repository.

## Top-level Fields

| Name | Type   | Description |
|------|--------|-------------|
| [default-registry](#default-registry) | [Registry][] or null | Registry used for all ports without a specific registry |
| [overlay-ports](#overlay-ports) | string[] | List of paths to use as overlay port locations |
| [overlay-triplets](#overlay-triplets) | string[] | List of paths to use as overlay triplets |
| [registries](#registries) | [Registry][][] | Additional registries to use for subsets of ports |

### <a name="default-registry"></a> `"default-registry"`

The registry to use for all ports without a more specific registry. A [Registry][] or null. Optional.

Ports that do not match any [`"packages"`](#registry-packages) pattern are resolved to the default registry. If the default registry is specified as `null`, ports that do not match will fail to resolve. If the default registry is omitted, it will implicitly be set to a [Builtin Registry][] using the value of [`"builtin-baseline"`](vcpkg-json.md#builtin-baseline) as the [`"baseline"`](#registry-baseline).

### <a name="registries"></a> `"registries"`

Additional registries to use for specific ports. An array of [Registries](#registry). Optional.

### <a name="overlay-ports"></a> `"overlay-ports"`

A list of port overlay paths. An array of strings. Optional.

Each path in the array must name either:

- A directory containing a port, with  `vcpkg.json` and `portfile.cmake` files, or
- A directory containing directories named after the ports (`zlib`'s `vcpkg.json` must be at `zlib/vcpkg.json`).

Relative paths are resolved relative to the `vcpkg-configuration.json` file. Absolute paths can be used but are discouraged.

### <a name="overlay-triplets"></a> `"overlay-triplets"`

A list of triplet overlay paths. An array of strings. Optional.

Each path in the array must point to a directory of triplet files ([see triplets documentation](../users/triplets.md)). Relative paths are resolved relative to the `vcpkg-configuration.json` file. Absolute paths can be used but are discouraged.

## <a name="registry"></a> Registry Fields

| Name | Required | Type   | Description |
|------|----------|--------|-------------|
| [baseline](#registry-baseline) | Git and Builtin Registries | string | Minimum version constraint on all ports from this registry |
| [kind](#registry-kind) | Yes | string | Kind of registry being used |
| [packages](#registry-packages) | Yes, if not default | string | List of ports to come from this registry |
| [path](#registry-path) | Filesystem Registry | string | Path to the Filesystem registry |
| [reference](#registry-reference) | No | string | Git reference to use for available versions |
| [repository](#registry-repository) | Git Registry | string | URI of the Git registry |

[Registry]: #registry

### <a name="registry-kind"></a> [Registry][]: `"kind"`

The kind of registry being used. A string. Required.

| `"kind"` Value | Registry Kind |
| ------|---|
| `"filesystem"` | [Filesystem Registry][] |
| `"git"` | [Git Registry][] |
| `"builtin"` | [Builtin Registry][] |

### <a name="registry-baseline"></a> [Registry][]: `"baseline"`

The registry-specific identifier for the minimum versions to use from this registry. A string. Required.

For [Git Registries][Git Registry] and for the [Builtin Registry][], this is a 40-character git commit sha in the registry's repository that contains a `versions/baseline.json`.

For [Filesystem Registries][Filesystem Registry], it can be any valid json property name that the registry defines on its `baseline.json`. If a filesystem registry does not declare a baseline, the value used is `"default"`.

### <a name="registry-reference"></a> [Registry][]: `"reference"`

The Git reference used to list available versions of a [Git Registry][]. A string. Optional.

If not specified, defaults to `HEAD`. This field can be a topic branch to access versions that are not yet fully published.

### <a name="registry-repository"></a> [Registry][]: `"repository"`

The URI of the [Git Registry][]. A string. Required for Git Registries.

The string can be any URI format that Git understands:

- `"https://github.com/microsoft/vcpkg"`
- `"git@github.com:microsoft/vcpkg"`
- `"/dev/vcpkg-registry"`

Relative paths have unspecified behavior that will change in future versions of vcpkg.

### <a name="registry-path"></a> [Registry][]: `"path"`

The path to the [Filesystem Registry][]. A string. Required for Filesystem Registries.

Relative paths are resolved relative to the `vcpkg-configuration.json`.

### <a name="registry-packages"></a> [Registry][]: `"packages"`

The list of port patterns assigned to this registry. An array of strings. Required for all registries outside [`"default-registry"`](#default-registry).

Each entry must be either:
* The name of a package
* A package name prefix, followed by `*` (_Added in tool version 2022-12-14_)

Package patterns may contain only lowercase letters, digits, and `-`, with an optional trailing `*`.

**Examples of valid patterns:**
* `*`: Matches all port names
* `boost`: Matches only the port `boost`
* `b*`: Matches ports that start with the letter `b`
* `boost-*`: Matches ports that start with the prefix `boost-`

**Examples of invalid patterns:**
* `*a` (`*` must be the last character in the prefix)
* `a**` (only one `*` is allowed)
* `a+` (`+` is not a valid pattern character)
* `a?` (`?` is not a valid pattern character)

See the [Using Registries documentation](../concepts/package-name-resolution.md) for more information on how port names are resolved.

[Git Registry]: ../concepts/registries.md#git-registries
[Filesystem Registry]: ../concepts/registries.md#filesystem-registries
[Builtin Registry]: ../concepts/registries.md#built-in-registry
