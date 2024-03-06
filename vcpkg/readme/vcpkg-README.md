---
title: Microsoft/vcpkg README
description: README for vcpkg registry
author: JavierMatosD
ms.author: javiermat
ms.date: 3/6/2024
ROBOTS: NOINDEX
---

# vcpkg: Overview

vcpkg is a free and open-source C/C++ package manager maintained by Microsoft
and the C++ community. 

Initially launched in 2016 as a tool for assisting developers in migrating their
projects to newer versions of Visual Studio, vcpkg has evolved into a
cross-platform tool used by developers on Windows, macOS, and Linux. vcpkg has a
large catalog of open-source libraries and enterprise-ready features designed to
facilitate your development process with support for any build and project
systems. vcpkg is a C++ tool at heart and is written in C++ with scripts in
CMake. It is designed from the ground up to address the unique pain points C/C++
developers experience.

This tool and ecosystem are constantly evolving, and we always appreciate
contributions! Learn how to start contributing with our [packaging
tutorial](../get_started/get-started-adding-to-registry.md) and [maintainer
guide](../contributing/maintainer-guide.md).

# Using vcpkg

Create a [manifest for your project's dependencies](../consume/manifest-mode.md)
(`vcpkg.json`):

```json
{
  "name": "my-project",
  "version": "0.1.0",
  "dependencies": [
    "fmt"
  ]
}
```

Or [install packages throught the command line](../consume/classic-mode.md):

```Console
./vcpkg install fmt
```

Then use one of our available build system integrations for
[CMake](../concepts/build-system-integration.md#cmake-integration),
[MSBuild](../concepts/build-system-integration.md#msbuild-integration) or 
[other build
systems](../concepts/build-system-integration.md#manual-integration).

For a short description of all available commands, run `vcpkg help`.
Run `vcpkg help [topic]` for details on a specific topic.

# Getting Started

First, follow the [quick start guide](../get_started/get-started.md).

If a library you need is not present in the vcpkg registry, you can [open an
issue on the GitHub repository][contributing:submit-issue] where the vcpkg team and
community can see it, and potentially add the port to vcpkg. Or you can
[contribute the package yourself](../get_started/get-started-adding-to-registry.md).

After you've gotten vcpkg installed and working, you may wish to add
[tab completion](../commands/integrate.md#vcpkg-autocompletion) to your shell.

vcpkg has you covered, it doesn't matter what build system you use:

* [vcpkg with CMake](../get_started/get-started.md)
* [vcpkg with MSBuild](../get_started/get-started-msbuild.md)
* [vcpkg with other build systems](../users/buildsystems/manual-integration.md)

Or what editor:

* [vcpkg with Visual Studio](../get_started/get-started-vs.md)
* [vcpkg with Visual Sudio Code](../get_started/get-started-vscode.md)
* [vcpkg with
  CLion](<https://www.jetbrains.com/help/clion/package-management.html>)
  
# Resources

* Ports: [Microsoft/vcpkg](<https://github.com/microsoft/vcpkg>)
* Source code: [Microsoft/vcpkg-tool](<https://github.com/microsoft/vcpkg-tool>)
* Docs: [Documentation](/vcpkg)
* Website: [vcpkg.io](<https://vcpkg.io>)
* Email: [vcpkg@microsoft.com](<mailto:vcpkg@microsoft.com>)
* Discord: [\#include \<C++\>'s Discord server](<https://www.includecpp.org>), in the #🌏vcpkg channel
* Slack: [C++ Alliance's Slack server](<https://cppalliance.org/slack/>), in the #vcpkg channel

# Contributing

vcpkg is an open source project, and is thus built with your contributions. Here are some ways you can contribute:
]
* [Submit issues][contributing:submit-issue] in vcpkg or existing packages
* [Submit fixes and new Packages][contributing:submit-pr]

Please refer to our [mantainer guide](../contributing/maintainer-guide.md) and
[packaging tutorial](../get_started/get-started-packaging.md) for more details.

This project has adopted the [Microsoft Open Source Code of
Conduct][contributing:coc]. For more information see the [Code of Conduct
FAQ][contributing:coc-faq] or email
[opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional
questions or comments.
 
[contributing:submit-issue]: https://github.com/microsoft/vcpkg/issues/new/choose
[contributing:submit-pr]: https://github.com/microsoft/vcpkg/pulls
[contributing:coc]: https://opensource.microsoft.com/codeofconduct/
[contributing:coc-faq]: https://opensource.microsoft.com/codeofconduct/

# License

The code in this repository is licensed under the MIT License. The libraries
provided by ports are licensed under the terms of their original authors. Where
available, vcpkg places the associated license(s) in the location
`installed/<triplet>/share/<port>/copyright`.

# Security

Most ports in vcpkg build the libraries in question using the original build
system preferred by the original developers of those libraries, and download
source code and build tools from their official distribution locations. For use
behind a firewall, the specific access needed will depend on which ports are
being installed. If you must install it in an "air gapped" environment, consider
instaling once in a non-"air gapped" environment, populating an [asset
cache](../users/assetcaching.md) shared with the otherwise "air gapped"
environment.

# Telemetry

vcpkg collects usage data in order to help us improve your experience. The data
collected by Microsoft is anonymous. You can opt-out of telemetry by:

- running the bootstrap-vcpkg script with `-disableMetrics`
- passing `--disable-metrics` to vcpkg on the command line
- setting the `VCPKG_DISABLE_METRICS` environment variable

Read more about vcpkg telemetry at [https://learn.microsoft.com/vcpkg/about/privacy](../about/privacy.md).
