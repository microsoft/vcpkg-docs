# Classic Mode

vcpkg has two primary modes of operation - Classic Mode and [Manifest Mode](manifests.md). For most users, we recommend Manifest Mode.

In Classic Mode, vcpkg maintains a central *installed tree* inside the vcpkg instance built up by individual [`vcpkg install`](../commands/install.md) and [`vcpkg remove`](../commands/remove.md) commands. This central set of packages can then be shared by any number of projects.

Because the set of installed packages is not associated with an individual project, Classic Mode operates similarly to tools like `brew` or `apt`. Note though that the set is still associated with a vcpkg instance and each instance of vcpkg (such as separate `git clone`'s) will have its own set of classic mode packages installed.
