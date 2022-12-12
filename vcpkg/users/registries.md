---
title: Using Registries
description: Understand the use and content of registries in vcpkg.
ms.date: 11/30/2022
---
# Using Registries

For information on creating your own registries, see [Creating Registries](../users/registries.md).

vcpkg uses _Registries_ to manage ports and versions. By default, vcpkg finds libraries from the public curated registry at https://github.com/microsoft/vcpkg. This default set can be extended by adding additional registry definitions or replaced with your own mirror of the public registry.

Registries are configured in the [`vcpkg-configuration.json`](vcpkg-configuration-json.md).

## Example vcpkg-configuration.json

```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://internal/mirror/of/github.com/Microsoft/vcpkg",
    "baseline": "eefee7408133f3a0fef711ef9c6a3677b7e06fd7"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/northwindtraders/vcpkg-registry",
      "baseline": "dacf4de488094a384ca2c202b923ccc097956e0c",
      "packages": [ "beicode", "beison" ]
    }
  ]
}
```
This example adds a private registry, `https://github.com/northwindtraders/vcpkg-registry`, as the source for the libraries `beicode` and `beison`. All other ports are found from an internal mirror of the Curated Catalog hosted at `https://internal/mirror/of/github.com/Microsoft/vcpkg`.

## Package name resolution

The way package name resolution works in vcpkg is fairly distinct from many
package managers. It is very carefully designed to _never_ implicitly choose
the registry that a package is fetched from. Just from
`vcpkg-configuration.json`, one can tell exactly from which registry a
package definition will be fetched from.

The name resolution algorithm is:

- If the name matches an [overlay](#overlays), use that overlay; otherwise
- If there is a [`"packages"`](vcpkg-configuration-json.md#registry-packages) list that matches the port name, use that registry; otherwise
- If the [default registry](vcpkg-configuration-json.md#default-registry) is not `null`, use that registry; otherwise
- Fail to associate the port with a registry.

## <a name="overlays"></a> Overlays

Overlays are a way to extend vcpkg with additional ports (without creating a full registry) and additional triplets. Overlays are considered before performing any registry lookups or versioning and will replace any builtin triplets or ports.

Overlay settings are evaluated in this order:

1. Overlays from the [command line](../commands/common-options.md#overlay-ports) in the order passed; then
2. Overlays from [`vcpkg-configuration.json`](vcpkg-configuration-json.md#overlay-ports) in order; then
3. Overlays from the `VCPKG_OVERLAY_[PORTS|TRIPLETS]` [environment variables](config-environment.md#vcpkg_overlay_ports) in order

