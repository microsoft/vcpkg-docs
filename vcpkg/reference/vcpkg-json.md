---
title: vcpkg.json Reference
description: Reference documentation for the vcpkg.json file format.
ms.date: 7/16/2024
ms.topic: reference
---
# vcpkg.json Reference

For an overview of using manifests with vcpkg, see [Manifest
mode](../concepts/manifest-mode.md).

Manifests are strict [JSON](https://www.json.org) documents. They cannot contain C++-style comments (`//`) nor trailing commas. However you can use field names that start with `$` to write your comments in any object that has a well-defined set of keys. These comment fields are not allowed in any objects which permit user-defined keys (such as `"features"`).

The latest JSON Schema is available at [https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json](https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json). IDEs with JSON Schema support such as Visual Studio and Visual Studio Code can use this file to provide autocomplete and syntax checking. For most IDEs, you should set `"$schema"` in your `vcpkg.json` to this URL.

## Example

```json
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
  "dependencies": [
    "boost-system",
    {
      "name": "cpprestsdk",
      "default-features": false
    },
    "libxml2",
    "yajl"
  ]
}
```

This example demonstrates a manifest for an application using `boost-system`, `cpprestsdk`, `libxml2`, and `yajl`. The example also includes a `$schema` reference to enable better IDE validation and autocompletion.

## Top-level Fields

| Name | Required | Type   | Description |
|------|----------|--------|-------------|
| [builtin-baseline](#builtin-baseline) | No | string | Version pins to use when building as top-level |
| [default-features](#default-features) | No | [Feature Object][][] | Require the [features](../concepts/features.md) listed as on-by-default |
| [dependencies](#dependencies) | No | [Dependency][][] | List of dependencies required to build and use this project |
| [description](#description) | Libraries | string or string[] | The project description |
| [documentation](#documentation) | No | string | URI to the upstream project's documentation |
| [features](#features) | No | {string: [Feature](#feature)} | Optional [features](../concepts/features.md) available for users of the project |
| [homepage](#homepage) | No      | string | URI to the upstream project's homepage |
| [license](#license) | No      | string or null | SPDX license expression |
| [maintainers](#maintainers) | No      | string or string[] | Maintainers of the package files |
| [name](#name) | Libraries      | string | The project name |
| [overrides](#overrides) | No | Override[] | Version pins to use when building as top-level |
| [port-version](#port-version) | No | integer | Port files revision |
| [supports](#supports) | No | [Platform Expression][] | Supported platform and build configurations |
| [version<br>version-semver<br>version-date<br>version-string](#version) | Libraries | string | Upstream version information |

### <a id="builtin-baseline"></a> `"builtin-baseline"`

A shortcut for specifying the `"baseline"` for version resolution in the default registry. A string. Optional, only used by top-level projects.

This field indicates the commit of <https://github.com/microsoft/vcpkg> which provides global minimum version information for your manifest. It is required for top-level manifest files using versioning without a specified [`"default-registry"`](vcpkg-configuration-json.md#default-registry). It has the same semantic as defining your default registry to be:

```json
{
  "default-registry": {
    "kind": "builtin",
    "baseline": "<value>"
  }
}
```

See [versioning](../users/versioning.md#baselines) and [Using registries](../consume/git-registries.md) for more semantic details.

### <a name="default-features"></a> `"default-features"`

The set of features needed for reasonable behavior without additional customization.

The default features are automatically selected if either:

1. A port-to-port dependency for the port has [`"default-features": true`](#dependency-default-features) -- the default value.
1. The top-level manifest does not have a dependency for the port with [`"default-features": false`](#dependency-default-features).

Default features handle the specific case of providing a "default" configuration for transitive dependencies that the top-level project may not know about. Ports used by others should almost always use `"default-features": false` for their dependencies.

You can define platform-specific default features by using a [Feature Object][]:

```json
{
  "name": "my-port",
  "default-features": [
    "png",
    {
      "name": "winssl",
      "platform": "windows"
    }
  ]
}
```


See [`"features"`](#features) for more information about features.

### <a id="description"></a> `"description"`

The description of the port. A string or array of strings. Required for libraries, optional for top-level projects.

This is used to help users discover the library as part of a [`search`](../commands/search.md) or `find` command and learn what the library does.

### <a id="dependencies"></a> `"dependencies"`

The list of dependencies required by the project. An array of [Dependency objects][Dependency]. Optional.

This field lists all the dependencies needed to build and use your library or application.

#### Example port dependencies

```json
"dependencies": [
  {
    "name": "arrow",
    "default-features": false,
    "features": [
      "json",
      {
        "name": "mimalloc",
        "platform": "windows"
      }
    ]
  },
  "boost-asio",
  "openssl",
  {
    "name": "picosha2",
    "platform": "!windows"
  }
]
```

### <a id="documentation"></a> `"documentation"`

The URI to the upstream project's documentation. A string. Optional.

### <a name="features"></a> `"features"`

The [features](../concepts/features.md) available for users of the project. A map of names to [Feature objects](#feature). Optional.

Features are boolean flags that add additional behaviors and dependencies to a build. See the [Manifest Concept Documentation](../concepts/features.md) for more information about features.

### <a id="homepage"></a> `"homepage"`

The URI to the project's homepage. A string. Optional.

### <a id="license"></a> `"license"`

The SPDX short license expression of the project. A string or null. Optional.

The `"license"` should either be an [SPDX 3.19 license expression](https://spdx.org/licenses/) or should be `null` to indicate that users must read the deployed `/share/<port>/copyright` file. DocumentRefs are not supported.

>[!NOTE]
> The licensing information provided for each package in the vcpkg registry represents Microsoft's best understanding of the licensing requirements. However, this information may not be definitive. Users are advised to verify the exact licensing requirements for each package they intend to use, as it is ultimately their responsibility to ensure compliance with the applicable licenses.

#### Example license strings

- `MIT`
- `LGPL-2.1-only AND BSD-2-Clause`
- `GPL-2.0-or-later WITH Bison-exception-2.2`

#### EBNF

vcpkg uses the following [EBNF](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) to parse the license field:

```text
idchar = ? regex /[-.a-zA-Z0-9]/ ?
idstring = ( idchar ), { idchar } ;

(* note that unrecognized license and license exception IDs will be warned against *)
license-id = idstring ;
license-exception-id = idstring ;
(* note that DocumentRefs are unsupported by this implementation *)
license-ref = "LicenseRef-", idstring ;

with = [ whitespace ], "WITH", [ whitespace ] ;
and = [ whitespace ], "AND", [ whitespace ] ;
or = [ whitespace ], "OR", [ whitespace ] ;

simple-expression = [ whitespace ], (
  | license-id
  | license-id, "+"
  | license-ref
  ), [ whitespace ] ;

(* the following are split up from compound-expression to make precedence obvious *)
parenthesized-expression =
  | simple-expression
  | [ whitespace ], "(", or-expression, ")", [ whitespace ] ;

with-expression =
  | parenthesized-expression
  | simple-expression, with, license-exception-id, [ whitespace ] ;

(* note: "a AND b OR c" gets parsed as "(a AND b) OR c" *)
and-expression = with-expression, { and, with-expression } ;
or-expression = and-expression, { or, and-exression } ;

license-expression = or-expression ;
```

### <a id="maintainers"></a> `"maintainers"`

The list of package maintainers. A string or array of strings. Optional.

We recommend using the form "_Givenname_ _Surname_ \<_email_\>".

### <a id="name"></a> `"name"`

The name of the project. A string. Required for libraries, optional for top-level projects.

The name must be lowercase ASCII letters, digits, or hyphens (`-`). It must not start nor end with a hyphen. Libraries are encouraged to include their organization or framework name as a prefix, such as `msft-` or `boost-` to help users find and describe associated libraries.

For example, a library with an official name of `Boost.Asio` might be given the name `boost-asio`.

### <a name="overrides"></a> `"overrides"`

Exact version pins to use for specific dependencies. An array of Override objects. Optional.

`"overrides"` from transitive manifests (i.e. from dependencies) are ignored. Only overrides defined by the top-level project are used.

| Name | Required | Type   | Description |
|------|----------|--------|-------------|
| name | Yes      | string | The port name |
| [version](#version) | Yes | string | Upstream version information to pin. |
| [version-semver<br>version-date<br>version-string](#version) | Yes | string | Deprecated alternatives to `version` naming particular schemes. |
| [port-version](#port-version) | No | integer | Port files revision to pin. Deprecated in favor of being placed into the version itself. |

`"port-version"` should be specified as a `#N` suffix in `"version"`. For example, `"version": "1.2.3#7"` names version 1.2.3, port-version 7.

See also [versioning](../users/versioning.md#overrides) for more semantic details.

#### Example of version overrides

```json
  "overrides": [
    {
      "name": "arrow", "version": "1.2.3#7"
    },
    {
      "name": "openssl", "version": "1.1.1h#3"
    }
  ]
```

### <a id="port-version"></a> `"port-version"`

A version suffix distinguishing revisions to the packaging files. An integer. Defaults to `0`.

The `"port-version"` should be incremented whenever a new version of the port is published that does not change the upstream source version. When the upstream source version is changed, the [version field](#version) should change and the `"port-version"` should be reset to `0` (or removed).

See [versioning](../users/versioning.md#version-schemes) for more details.

### <a name="supports"></a> `"supports"`

Supported platform and build configurations. A [Platform Expression][]. Optional.

This field documents that a project is not expected to build or run successfully on certain platform configurations.

For example, if your library doesn't support building for Linux, you would use `"supports": "!linux"`.

### `"vcpkg-configuration"`

Allows to embed vcpkg configuration properties inside the `vcpkg.json` file. Everything inside the `vcpkg-configuration` property is treated as if it were defined in a `vcpkg-configuration.json` file. For more details, see the [`vcpkg-configuration.json`](../reference/vcpkg-configuration-json.md#registries) documentation.

Having a `vcpkg-configuration` defined in `vcpkg.json` while also having a `vcpkg-configuration.json` file is not allowed and will result in the vcpkg command terminating with an error message.

#### Example embedded `vcpkg-configuration`

```json
{
  "name": "test",
  "version": "1.0.0",
  "dependencies": [ "beison", "zlib" ],
  "vcpkg-configuration": {
    "registries": [
      {
        "kind": "git",
        "baseline": "768f6a3ad9f9b6c4c2ff390137690cf26e3c3453",
        "repository": "https://github.com/MicrosoftDocs/vcpkg-docs",
        "reference": "vcpkg-registry",
        "packages": [ "beicode", "beison" ]
      }
    ],
    "overlay-ports": [ "./my-ports/fmt", 
                       "./team-ports"
    ]
  }
```

### <a id="version"></a> `"version"`, `"version-semver"`, `"version-date"`, `"version-string"`

The version of the upstream project. A string. Required for libraries, optional for top-level projects.

A manifest must contain at most one version field. Each version field corresponds to a different versioning _scheme_:

- `"version"` - Relaxed [Semantic Version 2.0.0][], allowing more or less than 3 primary numbers. Example: `1.2.3.4.10-alpha1`
- `"version-semver"` - Strict [Semantic Version 2.0.0][]. Example: `2.0.1-rc5`
- `"version-date"` - A date formatted as `YYYY-MM-DD` with an optional trailing dot-separate numeric sequence. Used for packages which do not have numeric releases (for example, Live-at-HEAD). Example: `2022-12-09.314562`
- `"version-string"` - An arbitrary string. Used for packages that don't have orderable versions. This should be rarely used, however all ports created before the other version fields were introduced use this scheme.

See [versioning](../users/versioning.md#version-schemes) for more details.

[Semantic Version 2.0.0]: https://semver.org/#semantic-versioning-specification-semver

## <a name="dependency"></a> Dependency Fields

Each dependency is a string or an object with the following fields:

| Name | Required | Type   | Description |
|------|----------|--------|-------------|
| [default-features](#dependency-default-features) | No | bool | Require the features listed as on-by-default |
| [features](#dependency-features) | No | [Feature Object][][] | The list of additional features to require |
| [host](#dependency-host) | No | bool | Require the dependency for the host machine instead of the target |
| [name](#dependency-name) | Yes      | string | The name of the dependency |
| [platform](#dependency-platform) | No | [Platform Expression][] | Qualifier for which platforms to use the dependency |
| [version>=](#dependency-version-gt) | No | string | Minimum required version. Port-version is identified with a `#N` suffix, for example, `1.2.3#7` names port-version 7. |

Strings are interpreted as an object with _name_ defined to the string value.

[Dependency]: #dependency

### <a name="dependency-default-features"></a> [Dependency][]: `"default-features"`

A boolean indicating that the project depends on the 'on-by-default' features of the dependency. Defaults to `true`.

In most cases, this should be defined to `false` and any needed features should be explicitly depended upon.

### <a name="dependency-features"></a> [Dependency][]: `"features"`

The list of additional features to require. An array of Feature objects. Optional.

A <a name="feature-object"></a>[Feature Object][] is an object with the following fields:
- `name` - The name of the feature. A string. Required.
- `platform` - A [Platform Expression][] that limits the platforms where the feature is required. Optional.

A simple string is also a valid `Feature Object` equivalent to `{ "name": "<feature-name>" }`.

[Feature Object]: #feature-object

For example,

```json
{
  "name": "ffmpeg",
  "default-features": false,
  "features": [
    "mp3lame",
    {
      "name": "avisynthplus",
      "platform": "windows"
    }  
  ]
}
```

Uses the `ffmpeg` library with mp3 encoding support. On Windows only, `avisynthplus` support is also enabled.

### <a name="dependency-host"></a> [Dependency][]: `"host"`

A boolean indicating that the dependency must be built for the [host triplet](../users/host-dependencies.md) instead of the current port's triplet. Defaults to `false`.

Any dependency that provides tools or scripts which should be "executed" during a build (such as buildsystems, code generators, or helpers) should be marked as `"host": true`. This enables correct cross-compilation in cases that the target is not executable -- such as when compiling for `arm64-android`.

See [Host dependencies](../users/host-dependencies.md) for more information.

### <a name="dependency-name"></a> [Dependency][]: `"name"`

The name of the dependency. A string. Required.

This follows the same restrictions as the [`"name"`](#name) property for a project.

### <a name="dependency-platform"></a> [Dependency][]: `"platform"`

An expression that limits the platforms where the dependency is required. A [Platform Expression][]. Optional.

If the expression does not match the current configuration, the dependency will not be used. For example, if a dependency has `"platform": "!windows"`, it only be required when targeting non-Windows systems.

### <a name="dependency-version-gt"></a> [Dependency][]: `"version>="`

A minimum version constraint on the dependency.

This field specifies the minimum version of the dependency, optionally using a
`#N` suffix to also specify a minimum port-version if desired.

For more information on versioning semantics, see [Versioning](../users/versioning.md#version).

## <a name="feature"></a> Feature Fields

Each feature is an object with the following fields:

| Name | Required | Type   | Description |
|------|----------|--------|-------------|
| [description](#feature-description) | Yes      | string | The description of the feature |
| [dependencies](#feature-dependencies) | No | [Dependency][][] | A list of dependencies |
| [supports](#feature-supports) | No | [Platform Expression][] | Qualifier for which platforms and configurations the feature supports |
| [license](#feature-license) | No | string or null | SPDX license expression |

Check out the [Manifest mode](../concepts/features.md) documentation for a conceptual overview of features.

[Feature]: #feature

### Example port with features

```json
{
  "features": {
    "cbor": {
      "description": "The CBOR backend",
      "dependencies": [
        {
          "$explanation": [
            "This is how you tell vcpkg that the cbor feature depends on the json feature of this package"
          ],
          "name": "libdb",
          "default-features": false,
          "features": [ "json" ]
        }
      ]
    },
    "csv": {
      "description": "The CSV backend",
      "dependencies": [
        "fast-cpp-csv-parser"
      ]
    },
    "json": {
      "description": "The JSON backend",
      "dependencies": [
        "jsoncons"
      ]
    }
  }
}
```

### <a id="feature-dependencies"></a> [Feature][]: `"dependencies"`

The list of dependencies required by the [feature](../concepts/features.md). An array of [Dependency objects][Dependency]. Optional.

This field lists any additional dependencies needed to build and use the feature.

### <a id="feature-description"></a> [Feature][]: `"description"`

The description of the [feature](../concepts/features.md). A string or array of strings. Required.

This is used to help users discover the feature as part of a [`search`](../commands/search.md) or `find` command and learn what the feature does.

### <a name="feature-supports"></a> [Feature][]: `"supports"`

The supported platform and build configurations for the [feature](../concepts/features.md). A [Platform Expression][]. Optional.

This field documents the platform configurations where the feature will build and run successfully.

### <a name="feature-license"></a> [Feature][]: `"license"`

The SPDX short license expression of the feature. A string or null. Optional. 
If not provided, the license is the same as specified in the top level [license](#license) field.

>[!NOTE]
> The licensing information provided for each package in the vcpkg registry represents Microsoft's best understanding of the licensing requirements. However, this information may not be definitive. Users are advised to verify the exact licensing requirements for each package they intend to use, as it is ultimately their responsibility to ensure compliance with the applicable licenses.

## <a name="platform-expression"></a> Platform Expression

A Platform Expression is a JSON string which describes when a dependency is required or when a library or feature is expected to build.

Expressions are built from primitive identifiers, logical operators, and grouping:

- `!<expr>`, `not <expr>` - negation
- `<expr>|<expr>`, `<expr>,<expr>` - logical OR (the keyword `or` is reserved but not valid in platform expressions)
- `<expr>&<expr>`, `<expr> and <expr>` - logical AND
- `(<expr>)` - grouping/precedence

The following identifiers are defined based on the [triplet settings](../users/triplets.md) and build configuration:

| Identifier | Triplet Condition |
|------------|-------------------|
| `x64` | `VCPKG_TARGET_ARCHITECTURE` == `"x64"` |
| `x86` | `VCPKG_TARGET_ARCHITECTURE` == `"x86"` |
| `arm` | `VCPKG_TARGET_ARCHITECTURE` == `"arm"` or</br> `VCPKG_TARGET_ARCHITECTURE` == `"arm64"` |
| `arm32` | `VCPKG_TARGET_ARCHITECTURE` == `"arm"` |
| `arm64` | `VCPKG_TARGET_ARCHITECTURE` == `"arm64"` |
| `arm64ec` | `VCPKG_TARGET_ARCHITECTURE` == `"arm64ec"` |
| `wasm32` | `VCPKG_TARGET_ARCHITECTURE` == `"wasm32"` |
| `mips64` | `VCPKG_TARGET_ARCHITECTURE` == `"mips64"` |
| `windows` | `VCPKG_CMAKE_SYSTEM_NAME` == `""` or</br> `VCPKG_CMAKE_SYSTEM_NAME` == `"WindowsStore"` or</br> `VCPKG_CMAKE_SYSTEM_NAME` == `"MinGW"` |
| `mingw` | `VCPKG_CMAKE_SYSTEM_NAME` == `"MinGW"` |
| `uwp` | `VCPKG_CMAKE_SYSTEM_NAME` == `"WindowsStore"` |
| `xbox` | `VCPKG_CMAKE_SYSTEM_NAME` == `""` and</br> `XBOX_CONSOLE_TARGET` is defined. |
| `linux` | `VCPKG_CMAKE_SYSTEM_NAME` == `"Linux"` |
| `osx` | `VCPKG_CMAKE_SYSTEM_NAME` == `"Darwin"` |
| `ios` | `VCPKG_CMAKE_SYSTEM_NAME` == `"iOS"` |
| `freebsd` | `VCPKG_CMAKE_SYSTEM_NAME` == `"FreeBSD"` |
| `openbsd` | `VCPKG_CMAKE_SYSTEM_NAME` == `"OpenBSD"` |
| `android` | `VCPKG_CMAKE_SYSTEM_NAME` == `"Android"` |
| `emscripten` | `VCPKG_CMAKE_SYSTEM_NAME` == `"Emscripten"` |
| `qnx` | `VCPKG_CMAKE_SYSTEM_NAME` == `"QNX"` |
| `vxworks` | `VCPKG_CMAKE_SYSTEM_NAME` == `"VxWorks"` |
| `static` | `VCPKG_LIBRARY_LINKAGE` == `"static"` |
| `staticcrt` | `VCPKG_CRT_LINKAGE` == `"static"` |
| `native` | `TARGET_TRIPLET` == `HOST_TRIPLET` |

### Example platform expression

- **Needs `picosha2` for sha256 on non-Windows, but get it from the OS on Windows (BCrypt)**

```json
{
  "name": "picosha2",
  "platform": "!windows"
}
```

- **Require zlib on arm64 Windows and amd64 Linux**

```json
{
  "name": "zlib",
  "platform": "(windows & arm64) | (linux & x64)"
}
```

[Platform Expression]: #platform-expression
