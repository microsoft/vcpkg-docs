---
title: vcpkg CONTROL files
description: Understand the use and metadata content of deprecated vcpkg CONTROL files.
ms.date: 01/10/2024
ms.topic: reference
---
# CONTROL files

> [!WARNING]
> `CONTROL` files are deprecated and only retained for backwards compatibility with earlier versions of vcpkg. Use [vcpkg.json manifest files](../users/manifests.md) for any newly authored port.
>
> Use `./vcpkg format-manifest path/to/CONTROL` to convert an existing `CONTROL` file to a `vcpkg.json` file.

The `CONTROL` file contains metadata about the port.  The syntax is based on [the Debian `control` format](https://www.debian.org/doc/debian-policy/ch-controlfields.html) although we only support the subset of fields documented here.

Field names are case-sensitive and start the line without leading whitespace.  Paragraphs are separated by one or more empty lines.

## Source Paragraph

The first paragraph in a `CONTROL` file is the Source paragraph.  It must have a `Source`, `Version`, and `Description` field. The full set of fields is documented below.

### Examples

```txt
Source: ace
Version: 6.5.5
Description: The ADAPTIVE Communication Environment
```

```txt
Source: vtk
Version: 8.2.0
Port-Version: 2
Description: Software system for 3D computer graphics, image processing, and visualization
Build-Depends: zlib, libpng, tiff, libxml2, jsoncpp, glew, freetype, expat, hdf5, libjpeg-turbo, proj4, lz4, libtheora, atlmfc (windows), eigen3, double-conversion, pugixml, libharu, sqlite3, netcdf-c
```

### Recognized fields

#### Source

The name of the port.

When adding new ports be aware that the name may conflict with other projects that are not a part of vcpkg.  For example `json` conflicts with too many other projects so you should add a scope to the name such as `taocpp-json` to make it unique.  Verify there are no conflicts on a search engine as well as on other package collections.

Package collections to check for conflicts:

+ [Repology](https://repology.org/projects/)
+ [Debian packages](https://www.debian.org/distrib/packages)
+ [Packages search](https://pkgs.org/)

#### Version

The library version.

This field is an alphanumeric string that may also contain `.`, `_`, or `-`. No attempt at ordering versions is made; all versions are treated as bit strings and are only evaluated for equality.

For tagged-release ports, we follow the following convention:

1. If the port follows a scheme like `va.b.c`, we remove the leading `v`. In this case, it becomes `a.b.c`.
1. If the port includes its own name in the version like `curl-7_65_1`, we remove the leading name: `7_65_1`

For rolling-release ports, we use the date that the _commit was accessed by you_, formatted as `YYYY-MM-DD`. Stated another way: if someone had a time machine and went to that date, they would see this commit as the latest master.

For example, given:

1. The latest commit was made on 2019-04-19
1. The current version string is `2019-02-14-1`
1. Today's date is 2019-06-01.

Then if you update the source version today, you should give it version `2019-06-01`.

#### Port-Version

The version of the port.

This field is a non-negative integer. It allows one to version the port file separately from the version of the underlying library; if you make a change to a port, without changing the underlying version of the library, you should increment this field by one (starting at `0`, which is equivalent to no `Port-Version` field). When the version of the underlying library is upgraded, this field should be set back to `0` (i.e., delete the `Port-Version` field).

##### Examples

```
Version: 1.0.5
Port-Version: 2
```

```
Version: 2019-03-21
```

#### Description

A description of the library.

By convention the first line of the description is a summary of the library.  An optional detailed description follows.  The detailed description can be multiple lines, all starting with whitespace.

##### Examples

```
Description: C++ header-only JSON library
```

```
Description: Mosquitto is an open source message broker that implements the MQ Telemetry Transport protocol versions 3.1 and 3.1.1.
  MQTT provides a lightweight method of carrying out messaging using a publish/subscribe model. This makes it suitable for "machine
  to machine" messaging such as with low power sensors or mobile devices such as phones, embedded computers or microcontrollers like the Arduino.
```

#### Homepage

The URL of the homepage for the library where a user is able to find additional documentation or the original source code.

Example:

```
Homepage: https://github.com/Microsoft/vcpkg
```

#### Build-Depends

Comma separated list of vcpkg ports the library has a dependency on.

vcpkg does not distinguish between build-only dependencies and runtime dependencies. The complete list of dependencies needed to successfully use the library should be specified.

For example: websocketpp is a header only library, and thus does not require any dependencies at install time. However, downstream users need boost and openssl to make use of the library. Therefore, websocketpp lists boost and openssl as dependencies.

If the port is dependent on optional features of another library those can be specified using the `portname[featurelist]` syntax. If the port does not require any features from the dependency, this should be specified as `portname[core]`.

Dependencies can be filtered based on the target triplet to support differing requirements. These filters use the same syntax as the Supports field below and are surrounded in parentheses following the portname and feature list.

##### Example

```
Build-Depends: rapidjson, curl[core,openssl] (!windows), curl[core,winssl] (windows)
```

#### Default-Features

Comma separated list of optional port features to install by default.

This field is optional.

##### Example

```
Default-Features: dynamodb, s3, kinesis
```

#### <a name="Supports"></a> Supports

Expression that evaluates to true when the port is expected to build successfully for a triplet.

Currently, this field is only used in the CI testing to skip ports. In the future, this mechanism is intended to warn users in advance that a given install tree is not expected to succeed. Therefore, this field should be used optimistically; in cases where a port is expected to succeed 10% of the time, it should still be marked "supported".

See [Platform Expressions](../reference/vcpkg-json.md#platform-expression) in the `vcpkg.json` manifest documentation for the list of supported identifiers.

##### Example

```
Supports: !(uwp|arm)
```

## Feature Paragraphs

Multiple optional features can be specified in the `CONTROL` files.  It must have a `Feature` and `Description` field.  It can optionally have a `Build-Depends` field.  It must be separated from other paragraphs by one or more empty lines.

### Example

```
Source: vtk
Version: 8.2.0-2
Description: Software system for 3D computer graphics, image processing, and visualization
Build-Depends: zlib, libpng, tiff, libxml2, jsoncpp, glew, freetype, expat, hdf5, libjpeg-turbo, proj4, lz4, libtheora, atlmfc (windows), eigen3, double-conversion, pugixml, libharu, sqlite3, netcdf-c

Feature: openvr
Description: OpenVR functionality for VTK
Build-Depends: sdl2, openvr

Feature: qt
Description: Qt functionality for VTK
Build-Depends: qt5

Feature: mpi
Description: MPI functionality for VTK
Build-Depends: mpi, hdf5[parallel]

Feature: python
Description: Python functionality for VTK
Build-Depends: python3
```

### Recognized fields

#### Feature

The name of the feature.

#### Description

A description of the feature using the same syntax as the port  `Description` field.

#### Build-Depends

The list of dependencies required to build and use this feature.

On installation the dependencies from all selected features are combined to produce the full dependency list for the build. This field follows the same syntax as `Build-Depends` in the Source Paragraph.
