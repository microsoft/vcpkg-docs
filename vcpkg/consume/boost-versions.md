---
title: "Tutorial: Install a specific version of the Boost libraries using registry baselines"
description: Learn to install specific version of Boost packages using a baseline.
author: vicroms
ms.author: viromer
ms.date: 01/10/2024
ms.topic: tutorial
#CustomerIntent: As a beginner vcpkg user, I want to install a specific version set of the Boost libraries
---
# Tutorial: Install a specific version of the Boost libraries using registry baselines

vcpkg provides the `Boost` set of libraries as individual packages as a
convenience for users that depend only on a limited subset of Boost packages.
However, this approach has a tradeoff. Users who want to lock a specific
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

### 1 - Obtaining the Boost version baseline

Run the following command to see the history of commits that have modified the
version database for the `boost-optional` package:

```Console
git log "--format=%H %cd %s" --date=short --left-only -- versions/b-/boost-optional.json
```

You should see output similar to this:

```Console
caa7579a1c48e2ca770f6ccf98cb03db95642631 2023-09-13 [boost] update to v1.83.0 (#33597)
5d3798ac7388ca66c169773e46103b14077b76a4 2023-06-06 [boost] Remove port version constraints (#31572)
501db0f17ef6df184fcdbfbe0f87cde2313b6ab1 2023-04-15 [boost-build] Fix SHA512 and add MSVC workaround. (#30884)
bedfdb774cfbe47da202169046ca15441a213f3e 2023-04-15 [Boost] Update Boost ports to 1.82#0 (#30856)
9484a57dd560b89f0a583be08af6753611c57fd5 2023-02-24 Update vcpkg-tool to 2023-02-16. (#29664)
6aa38234d08efefc55b70025cf6afc2212e78e15 2023-02-01 [boost] Fix generate ports to match the tree. (#29338)
6d41737729b170cb7d323a4fddd21607c9237636 2022-12-20 [boost] update to 1.81.0 (#28356)
5ba2b95aea2a39aa89444949c7a047af38c401c1 2022-10-18 [boost] Add version constraints (#26817)
8424da584e59e05956913bf96f87654aa3096c7e 2022-08-25 [boost] update to 1.80.0 (#26233)
96ec7fb25da25e0463446e552d59715a47c95e73 2022-04-21 [boost] update to 1.79.0 (#24210)
76d4836f3b1e027758044fdbdde91256b0f0955d 2022-01-10 [boost] update to 1.78.0 (#21928)
cc471dc0f59b7b2066d6172c2893419412327a7a 2021-09-27 [boost] update to 1.77.0 (#19556)
761c81d43335a5d5ccc2ec8ad90bd7e2cbba734e 2021-07-07 [boost] update to 1.76.0 (#17335)
68a74950d0400f5a803026d0860f49853984bf11 2021-01-21 [vcpkg] Rename `port_versions` to `versions` (#15784)
```

As you can see, the commit for version `1.80.0` is listed. In the next step,
you'll use that commit as the baseline for all the Boost packages.

### 2 - Use a baseline to override Boost packages

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
curated registry but at a previous baseline and is limited to packages that
match the `boost*` and `boost-*` patterns.

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
