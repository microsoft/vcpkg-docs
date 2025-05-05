---
title: vcpkg x-test-features
description: Command line reference for the vcpkg test-features command. Builds all features of a package.
ms.date: 03/07/2025
---

# vcpkg x-test-features

[!INCLUDE [experimental](../../includes/experimental.md)]

## Synopsis

```console
vcpkg x-test-features [options] <package>...
```

## Description

Tests all features of a package by installing them in manifest mode. This command is useful for testing that all features of a package are working correctly.
The following tests are performed:
- The `core` feature gets installed with default featured disabled
- Every feature gets installed separately with default features disabled
- All features combined get installed

Features marked as `unsupported` or `cascade` in [`--ci-feature-baseline`](#ci-feature-baseline) are skipped.

## Example

```console
$ $ ./vcpkg x-test-features curl
Computing 23 install plans...
Computing all ABI hashes...
Detecting compiler hash for triplet x64-linux...
Compiler found: /usr/bin/c++
Checking the binary cache...
Feature Test [1/23] curl[core]:x64-linux
Feature Test [2/23] curl[core,non-http]:x64-linux
Feature Test [3/23] curl[core,tool]:x64-linux
...
```

## Options

All vcpkg commands support a set of [common options](common-options.md).

### `--ci-feature-baseline`

Path to the `ci.feature.baseline.txt` file. Used to skip ports and detect regressions.

Each line in this file is an assignment of a [port spec](install.md#package-syntax) to a state. Lines starting with a `#` (treated as comments) and
blank lines are ignored. A port spec is of the form
`port-name[features]:triplet(supports-expression)`, where the features, triplet, and [`supports`](../reference/vcpkg-json.md#supports)
expression part are optiona. For example, `curl`, `curl[http2]`, `curl:x64-windows`, or
`curl[http2](!windows)`.

States:

* **fail** - The port build is expected to fail in the CI system.  
  This is not necessarily the same as if a port is expected to build on a developers machine
  because it may fail due to the machine configuration.
* **skip** - Do not build this port in the CI system.  
  This is added to ports that may be flaky or conflict with other ports. We recommend adding a
  comment for why a port is skipped so it can be removed when the issue is resolved.
* **cascade** - The port depends on a port that fails or is excluded by a [supports expression](../reference/vcpkg-json.md#platform-expression) of a
  dependency but the port itself states that it can be build. This is added to ports to detect
  "hidden" untested ports. 

Additional states can be specified for specific features:
* **feature-fails** - The listed features do not build. They are therefore excluded from the
  "combined" feature test because it would simply fail too.
* **combination-fails** - The listed combination of features will fail to build.
* **no-separate-feature-test** - The listed features are not tested in the "separate" feature test.
* **options** - The listed features are mutually exclusive and can not be selected together. The
  first feature in the list is added to every test combination. Use the `core` feature as first one
  to add no feature to every test combination. [Note that, in general, features of this form are
  not intended to be accepted for anything new in vcpkg's curated registry.](/vcpkg/contributing/maintainer-guide#do-not-use-features-to-implement-alternatives)

You can select ports by triplet or supports expression:

* `curl[wchar]:x64-osx = feature-fails # the wchar feature does not work on !windows`
* `curl[wchar](!windows) = feature-fails # Using support expressions is often shorter than listing all triplets`

Other examples:

* `qt3d[extras] = options # extras will be always added to the test combination (it is currently a requirement)`
* `curl[core,openssl,wolfssl] = options # openssl and wolfssl are exclusive ssl backends but none must be selected`
* `libgit2[pcre,pcre2] = options # you have to select pcre xor pcre2`
* `libcanberra[gtk3](osx) = combination-fails # The feature works in the "combined" feature test`
* `vlpp[tools](osx) = feature-fails # error: use of undeclared identifier 'PATH_MAX'`
* `entt[experimental] = skip # required head build`
* `realsense2[openni2](!windows) = cascade # cpkg-tool-lessmsi[core] only supports native & windows`
* ```
  # Testing all targets separately would be too time consuming:
  halide[target-amdgpu,target-arm,target-d3d12compute,target-hexagon,target-metal,target-mips,target-nvptx,target-opencl,target-powerpc,target-riscv,target-x86] = no-separate-feature-test
  ```

### `--all`

Test all ports.

### `--no-core`

Skip testing the core feature.

### `--no-separated`

Skip testing each feature separately.

### `--no-combined`

Skip testing all features combined.

### `--failure-logs`

Directory to which failure logs will be copied.

### `--failing-abi-log`

Path to a file to which abis from ports that failed to build will be written.
