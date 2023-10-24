---
title: "Tutorial: Install a specific version of the Boost libraries using registry baselines"
description: Learn to install specific version of Boost packages using a baseline.
author: vicroms
ms.author: viromer
ms.prod: vcpkg
ms.date: 10/24/2023
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to install dependencies from multiple vcpkg registries
---
# Tutorial: Install a specific version of the Boost libraries using registry baselines"

vcpkg provides the Boost set of libraries as individual packages as a
convenience for users that depend only on a limited subset of Boost packages.
However, this approach has a tradeoff. Users that want to lock a specific
version of their Boost dependencies are required to override the versions of
each Boost package on which they depend.

However, there's a simple solution to this problem using registry baselines and
package patterns. 

## Prerequisites

* A terminal
* A C++ compiler
* vcpkg

## The problem

Let's assume that you have a project that depends on `boost-optional` and you
want to lock down version `1.80.0` of the Boost libraries. So you create an
override for your dependency as show below:

`vcpkg.json`

```json
{
    "dependencies": [ "boost-optional" ],
    "overrides": [
        { "name": "boost-optional", "version": "1.80.0" }
    ]
}
```

`vcpkg-configuration.json`

```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/Microsoft/vcpkg",
    "baseline": "3265c187c74914aa5569b75355badebfdbab7987"
  }
}
```

When you run `vcpkg install`, you'll notice that only the version of
`boost-optional` has been locked to `1.80.0` while the other Boost dependencies
are using the baseline version (`1.83.0`). 

```console
Fetching registry information from https://github.com/Microsoft/vcpkg (HEAD)...
Detecting compiler hash for triplet x64-linux...
The following packages will be built and installed:
  * boost-assert:x64-linux -> 1.83.0
  * boost-config:x64-linux -> 1.83.0
  * boost-core:x64-linux -> 1.83.0
  * boost-detail:x64-linux -> 1.83.0
  * boost-io:x64-linux -> 1.83.0
  * boost-move:x64-linux -> 1.83.0
    boost-optional:x64-linux -> 1.80.0
  * boost-predef:x64-linux -> 1.83.0
  * boost-preprocessor:x64-linux -> 1.83.0
  * boost-static-assert:x64-linux -> 1.83.0
  * boost-throw-exception:x64-linux -> 1.83.0
  * boost-type-traits:x64-linux -> 1.83.0
  * boost-uninstall:x64-linux -> 1.83.0
  * boost-utility:x64-linux -> 1.83.0
  * boost-vcpkg-helpers:x64-linux -> 1.83.0
Additional packages (*) will be modified to complete this operation.
```

### 1 - Use a baseline to override Boost packages

Instead of adding an override for all the related Boost dependencies, you can use
a registry baseline to lock down the version set instead. Modify your
`vcpkg-configuration.json` file as shown below:

`vcpkg-configuration.json`

```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/Microsoft/vcpkg",
    "baseline": "3265c187c74914aa5569b75355badebfdbab7987"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/Microsoft/vcpkg",
      "baseline": "8424da584e59e05956913bf96f87654aa3096c7e",
      "packages": [ "boost*", "boost-*"]
    }
  ]
}
```

The configuration file includes two registry definitions. The `default-registry`
which points to the vcpkg curated registry at
<https://github.com/Microsoft/vcpkg> using the latest commit at the time of
writing this article. And a second registry that also points to the vcpkg
curated registry but at a previous baseline.

If you run the `vcpkg install` command you should notice that this time all the
Boost libraries are using version `1.80.0`.

```Console
Fetching registry information from https://github.com/Microsoft/vcpkg (HEAD)...
Detecting compiler hash for triplet x64-linux...
The following packages will be built and installed:
  * boost-assert:x64-linux -> 1.80.0
  * boost-config:x64-linux -> 1.80.0
  * boost-core:x64-linux -> 1.80.0
  * boost-detail:x64-linux -> 1.80.0
  * boost-io:x64-linux -> 1.80.0
  * boost-move:x64-linux -> 1.80.0
    boost-optional:x64-linux -> 1.80.0
  * boost-predef:x64-linux -> 1.80.0
  * boost-preprocessor:x64-linux -> 1.80.0
  * boost-static-assert:x64-linux -> 1.80.0
  * boost-throw-exception:x64-linux -> 1.80.0
  * boost-type-traits:x64-linux -> 1.80.0
  * boost-uninstall:x64-linux -> 1.80.0
  * boost-utility:x64-linux -> 1.80.0
  * boost-vcpkg-helpers:x64-linux -> 1.80.0
  * boost-winapi:x64-linux -> 1.80.0
Additional packages (*) will be modified to complete this operation.
```

The same approach can be used for other metapackages like `qt`.

## Next steps

Here are some additional tasks to try next:

* Install packages using a [manifest file](manifest-mode.md)
* Install packages for custom platforms using [triplets](../users/triplets.md)
* Lock down your versions for repeatable builds using [versioning](../users/versioning.concepts.md)
* Reuse binaries across Continuous Integration runs using [binary caching](../users/binarycaching.md)
* Manage your private libraries using [custom registries](../maintainers/registries.md)

