---
title: Binary Caching
description: Reuse binaries built with vcpkg across different projects and machines.
ms.date: 11/30/2022
---

# Binary Caching

Libraries installed with vcpkg can always be built from source. However, this can duplicate work and waste time across multiple projects, developers, or machines.

Binary caching saves copies of library binaries in a shared location that can be accessed by vcpkg for future installs. This means that, as a user, you should only need to build dependencies from source once. If vcpkg is asked to install the same library with the same build configuration in the future, it will copy the built binaries from the cache and finish the operation in seconds.

Binary caching is especially effective when using Continuous Integration, since local developers can reuse the binaries produced during a CI run. It also greatly enhances the performance of ephemeral or "hosted" build agents, since all local changes are otherwise lost between runs. By using binary caching backed by a cloud service, such as GitHub, Azure, or others, you can ensure your CI runs at maximum speed and only rebuilds your dependencies when they've changed.

> [!TIP]
> We recommend creating a writable binary cache for every CI pipeline or workflow. Individual developers should have read-only access to the CI binary cache.

Caches can be hosted in a variety of environments. The most basic examples are a folder on the local machine or a network file share. Caches can also be stored in any NuGet feed (such as GitHub Packages or Azure DevOps Artifacts), Azure Blob Storage, Google Cloud Storage, and many other services.

If your CI provider offers a native "caching" function, we recommend using both vcpkg binary caching and the native method for the most performant results.

Binary caching is enabled by default with a [`files` provider](#files) at the first valid location of:

- **Windows**

1. `%VCPKG_DEFAULT_BINARY_CACHE%`
1. `%LOCALAPPDATA%\vcpkg\archives`
1. `%APPDATA%\vcpkg\archives`

- **Non-Windows**

1. `$VCPKG_DEFAULT_BINARY_CACHE`
1. `$XDG_CACHE_HOME/vcpkg/archives`
1. `$HOME/.cache/vcpkg/archives`

Reduced in-tool help is available via `vcpkg help binarycaching`.

Binary Caching only covers binaries you build. To cache source files and prebuilt tools, see [Asset Caching](assetcaching.md).

## Configuration Syntax

Binary caching is configured with the environment variable `VCPKG_BINARY_SOURCES` (set to `<source>;<source>;...`) and the command line option [`--binarysource=<source>`](../commands/common-options.md#binarysource). Options are evaluated first from the environment, then from the command line. Binary caching can be completely disabled by passing `--binarysource=clear` as the last command line option.

| Form | Description |
|---|---|
| `clear` | Disable all previous sources (including the default) |
| `default[,<rw>]` | Adds the default [files](#files) provider |
| [`files,<absolute path>[,<rw>]`](#files) | Adds a file-based location |
| [`nuget,<uri>[,<rw>]`](#nuget) | Adds a NuGet-based source; equivalent to the `-Source` parameter of the NuGet CLI |
| [`nugetconfig,<path>[,<rw>]`](#nuget) | Adds a NuGet-config-file-based source; equivalent to the `-Config` parameter of the NuGet CLI. |
| [`nugettimeout,<seconds>`](#nuget) | Specifies a timeout for NuGet network operations; equivalent to the `-Timeout` parameter of the NuGet CLI. |
| [`http,<url_template>[,<rw>[,<header>]]`](#http) | Adds a custom http-based location. |
| [`x-azblob,<baseuri>,<sas>[,<rw>]`](#azblob)    | **Experimental: will change or be removed without warning**<br>Adds an Azure Blob Storage source using a Shared Access Signature |
| [`x-gcs,<prefix>[,<rw>]`](#gcs)     | **Experimental: will change or be removed without warning**<br>Adds a Google Cloud Storage (GCS) source. |
| [`x-aws,<prefix>[,<rw>]`](#aws)     | **Experimental: will change or be removed without warning**<br>Adds an AWS S3 source. |
| [`x-aws-config,<parameter>`](#aws)  | **Experimental: will change or be removed without warning**<br>Configure all AWS S3 providers. |
| [`x-cos,<prefix>[,<rw>]`](#cos)     | **Experimental: will change or be removed without warning**<br>Adds a Tencent Cloud Object Storage source. |
| [`x-gha,<rw>]`](#gha)               | **Experimental: will change or be removed without warning**<br>Use the GitHub Actions cache as source. |
| `interactive` | Enables interactive credential management for [NuGet](#nuget) (for debugging; requires `--debug` on the command line) |

The `<rw>` optional parameter for certain sources controls whether they will be consulted for
downloading binaries (`read`)(default), whether on-demand builds will be uploaded to that remote (`write`), or both (`readwrite`).

## <a name="aws"></a> AWS S3 provider

[!INCLUDE [experimental](../../includes/experimental.md)]

```
x-aws,<prefix>[,<rw>]
```
Add an AWS S3 source using the AWS CLI. **\<prefix\>** should start with `s3://` and end in a `/`.

```
x-aws-config,no-sign-request
```
Pass `--no-sign-request` to the AWS CLI.

## <a name="azblob"></a> Azure Blob provider

[!INCLUDE [experimental](../../includes/experimental.md)]

```
x-azblob,<baseuri>,<sas>[,<rw>]
```

Adds an Azure Blob Storage provider using Shared Access Signature validation. `<baseuri>` should include the container path.

### <a name="azblob-quickstart"></a> Quickstart

First, you need to create an Azure Storage Account as well as a container. See the [Azure Storage Quick Start Documentation](/azure/storage/blobs/storage-quickstart-blobs-portal) for instructions.

Next, you will need to create a *Shared Access Signature* (SAS), which can be done from the storage account under *Settings* -> *Shared Access Signature*. This SAS will need:

- Allowed services: **Blob**
- Allowed resource types: **Object**
- Allowed permissions: **Read** (if using `read`) or **Read, Create** (if using `write` or `readwrite`)

The blob endpoint plus the container must be passed as the `<baseuri>` and the generated SAS without the `?` prefix must be passed as the `<sas>`.

**Example:**

```
x-azblob,https://<storagename>.blob.core.windows.net/<containername>,sv=2019-12-12&ss=b&srt=o&sp=rcx&se=2020-12-31T06:20:36Z&st=2020-12-30T22:20:36Z&spr=https&sig=abcd,readwrite
```

vcpkg will attempt to avoid revealing the SAS during normal operations, however:

1. It will be printed in full if `--debug` is passed
1. It will be passed as a command line parameter to subprocesses, such as `curl.exe`

Azure Blob Storage includes a feature to remove cache entries that haven't been accessed in a given number of days which can be used to automatically manage the size of your binary cache. See [Data Lifecycle Management on Microsoft Docs](/azure/storage/blobs/lifecycle-management-overview) for more information, or look for *Data management* -> *Lifecycle management* in the Azure Portal for your storage account.

## <a name="cos"></a> Tencent Cloud Object Storage provider

```
x-cos,<prefix>[,<rw>]
```
Adds a COS source. `<prefix>` should start with `cos://` and end with `/`.

## <a name="files"></a> Files provider

```
files,<absolute path>[,<rw>]
```

Stores zip-compressed archives at the path based on the [binary caching ID](#abi-hash).

## <a name="gcs"></a> Google Cloud Storage provider

[!INCLUDE [experimental](../../includes/experimental.md)]

```
x-gcs,<prefix>[,<rw>]
```

Adds a Google Cloud Storage provider. `<prefix>` should start with `gs://` and end with `/`.

### <a name="gcs-quickstart"></a> Quickstart

First, you need to create an Google Cloud Platform Account as well as a storage bucket ([GCS Quick Start](https://cloud.google.com/storage/docs/quickstart-gsutil)].

As part of this quickstart you would have configured the `gsutil` command-line tool to authenticate with Google Cloud. vcpkg will use this command-line tool, so make sure it is in your search path for executables.

Example 1 (using a bucket without a common prefix for the objects):

```
x-gcs,gs://<bucket-name>/,readwrite
```

Example 2 (using a bucket and a prefix for the objects):

```
x-gcs,gs://<bucket-name>/my-vcpkg-cache/maybe/with/many/slashes/,readwrite
x-gcs,gs://<bucket-name>/my-vcpkg-cache/maybe/with`,commas/too!/,readwrite
```

Commas (`,`) are valid as part of a object prefix in GCS. Remember to escape them in the vcpkg configuration, as shown in the previous example. GCS does not have folders (some of the GCS tools simulate folders). It is not necessary to create or otherwise manipulate the prefix used by your vcpkg cache.

## <a name="gha"></a> GitHub Actions cache

[!INCLUDE [experimental](../../includes/experimental.md)]

```
x-gha[,<rw>]
```

Adds the GitHub Actions cache as a provider. This binary caching provider is only valid in the context of a GitHub Actions workflow. This provider requires both of the `ACTIONS_CACHE_URL` and `ACTIONS_RUNTIME_TOKEN` environment variables to be set. Setting these environment variables correctly is covered in the following Quickstart section.

### <a name="gha-quickstart"></a> Quickstart

In order for vcpkg to make use of the GitHub Actions Cache we must provide it the Actions Cache URL and Runtime Token. To do this we export both values as environment variables in an workflow step similar to the following:

```yaml
- uses: actions/github-script@v6
  with:
    script: |
      core.exportVariable('ACTIONS_CACHE_URL', process.env.ACTIONS_CACHE_URL || '');
      core.exportVariable('ACTIONS_RUNTIME_TOKEN', process.env.ACTIONS_RUNTIME_TOKEN || '');
```

Specifying these values as environment variables instead of vcpkg command line arguments is by design as the GitHub Actions Cache binary caching provider can only be used from a GitHub Actions workflow.

Once the environment variables have been exported, vcpkg can be then be run with the GitHub Actions binary caching provider like this:

```yaml
- name: Install dependencies via vcpkg
  run: vcpkg install zlib --binarysource="clear;x-gha,readwrite"
```

## <a name="http"></a> HTTP provider

```
http,<url_template>[,<rw>[,<header>]]
```

Each binary caching operation is mapped to an HTTP verb:

- Download - `GET`
- Upload - `PUT`
- Check Existence - `HEAD`

### URL Template

The template uses curly-brackets for variable expansion. You can use the variables 'name', 'version', 'sha' and 'triplet'. For example:

`https://cache.example.com/{name}/{version}/{sha}`

### Header

> [!WARNING]
> This value may appear on the command line of external process calls, which may have security implications in your environment.

Authentication is supported by specifying an HTTP [Authorization Header](https://developer.mozilla.org/docs/Web/HTTP/Headers/Authorization). For example:

```
http,https://cache.example.com/{name}/{version}/{sha},readwrite,Authorization: Bearer BearerTokenValue
```

## <a name="nuget"></a> NuGet provider

Add a NuGet server with the `-Source` NuGet CLI parameter:
```
nuget,<uri>[,<rw>]
```
Use a NuGet config file with the `-Config` NuGet CLI parameter:
```
nugetconfig,<path>[,<rw>]
```
Configure the timeout for NuGet sources:
```
nugettimeout,<seconds>
```

Config files must define a `defaultPushSource` to support writing packages back to the feed.

### <a name="nuget-credentials"></a> Credentials

Many NuGet servers require additional credentials to access. The most flexible way to supply credentials is via the `nugetconfig` source with a custom `nuget.config` file. See [Consuming packages from authenticated feeds](/nuget/consume-packages/consuming-packages-authenticated-feeds) for more information.

However, it is still possible to authenticate against many servers using NuGet's built-in credential providers or via customizing your environment's default `nuget.config`. The default config can be extended via nuget client calls such as:

```bash
nuget sources add -Name MyRemote -Source https://... -Username $user -Password $pass
```

and then passed to vcpkg via `nuget,MyRemote,readwrite`. You can get a path to the precise copy of NuGet used by vcpkg by running `vcpkg fetch nuget`, which will report something like:

```bash
$ vcpkg fetch nuget
/vcpkg/downloads/tools/nuget-5.5.1-linux/nuget.exe
```

Non-Windows users will need to call this through mono via `mono /path/to/nuget.exe sources add ...`.

### `metadata.repository`

The `nuget` and `nugetconfig` source providers respect certain environment variables while generating nuget packages. The `metadata.repository` field of any packages will be generated as:

```xml
    <repository type="git" url="${VCPKG_NUGET_REPOSITORY}"/>
```

or

```xml
    <repository type="git"
                url="${GITHUB_SERVER_URL}/${GITHUB_REPOSITORY}.git"
                branch="${GITHUB_REF}"
                commit="${GITHUB_SHA}"/>
```

if the appropriate environment variables are defined and non-empty. This is specifically used to associate packages in GitHub Packages with the *building* project and not intended to associate with the original package sources.

### NuGet Cache

NuGet's user-wide cache is not used by default. To use it for every nuget-based source, set the [environment variable](config-environment.md#vcpkg_use_nuget_cache) `VCPKG_USE_NUGET_CACHE` to `true` (case-insensitive) or `1`.

## Provider Examples

If your CI system of choice is not listed, we welcome PRs to add them!

### <a name="quickstart-github"></a> GitHub Packages

To use vcpkg with GitHub Packages, we recommend using the [NuGet provider](#nuget).

> [!NOTE]
> **2020-09-21**: GitHub's hosted agents come with an older, pre-installed copy of vcpkg on the path that does not support the latest binary caching. This means that direct calls to `bootstrap-vcpkg` or `vcpkg` without a path prefix may call an unintended vcpkg instance. We recommend taking the following two steps to avoid issues if you want to use your own copy of vcpkg:
>
> 1. Run the equivalent of `rm -rf "$VCPKG_INSTALLATION_ROOT"` using `shell: 'bash'`.
> 2. Always call `vcpkg` and `bootstrap-vcpkg` with a path prefix, such as `./vcpkg`, `vcpkg/vcpkg`, `.\bootstrap-vcpkg.bat`, etc.

```yaml
# actions.yaml
#
# In this example, vcpkg has been added as a submodule (`git submodule add https://github.com/Microsoft/vcpkg`).
env:
  VCPKG_BINARY_SOURCES: 'clear;nuget,GitHub,readwrite'

matrix:
  os: ['windows-2019', 'ubuntu-20.04']
  include:
    - os: 'windows-2019'
      triplet: 'x86-windows'
      mono: ''
    - os: 'ubuntu-20.04'
      triplet: 'x64-linux'
      # To run `nuget.exe` on non-Windows platforms, we must use `mono`.
      mono: 'mono'

steps:
  # This step assumes `vcpkg` has been bootstrapped (run `./vcpkg/bootstrap-vcpkg`)
  - name: 'Setup NuGet Credentials'
    shell: 'bash'
    # Replace <OWNER> with your organization name
    run: |
      ${{ matrix.mono }} `./vcpkg/vcpkg fetch nuget | tail -n 1` \
        sources add \
        -source "https://nuget.pkg.github.com/<OWNER>/index.json" \
        -storepasswordincleartext \
        -name "GitHub" \
        -username "<OWNER>" \
        -password "${{ secrets.GITHUB_TOKEN }}"
      ${{ matrix.mono }} `./vcpkg/vcpkg fetch nuget | tail -n 1` \
        setapikey "${{ secrets.GITHUB_TOKEN }}" \
        -source "https://nuget.pkg.github.com/<OWNER>/index.json"

  # Omit this step if you're using manifests
  - name: 'vcpkg package restore'
    shell: 'bash'
    run: >
      ./vcpkg/vcpkg install sqlite3 cpprestsdk --triplet ${{ matrix.triplet }}
```

If you're using [manifests](manifests.md), you can omit the `vcpkg package restore` step: it will be run automatically as part of your build.

See the [GitHub Packages' NuGet documentation](https://docs.github.com/packages/using-github-packages-with-your-projects-ecosystem/configuring-dotnet-cli-for-use-with-github-packages) for more information.

### <a name="quickstart-ado"></a> Azure DevOps Artifacts

To use vcpkg with Azure DevOps Artifacts, we recommend using the [NuGet provider](#nuget).

First, ensure Artifacts has been enabled on your DevOps account. An Administrator can enable this through *Project Settings* -> *General* -> *Overview* -> *Azure DevOps Services* > *Artifacts*.

Next, create a feed for your project. Your feed URL will be an `https://` link ending with `/nuget/v3/index.json`. For more information, see the [Azure DevOps Artifacts Documentation](/azure/devops/artifacts/get-started-nuget).

#### Using the feed from a pipeline

```yaml
# azure-pipelines.yaml
variables:
- name: VCPKG_BINARY_SOURCES
  value: 'clear;nuget,<FEED_URL>,readwrite'

steps:
# Remember to add this task to allow vcpkg to upload archives via NuGet
- task: NuGetAuthenticate@0
```

If you are using custom agents with a non-Windows OS, you will need to install Mono to run `nuget.exe` (`apt install mono-complete`, `brew install mono`, etc).

#### Using the feed locally

```powershell
# On Windows Powershell
PS> & $(vcpkg fetch nuget | select -last 1) sources add `
  -name ADO `
  -Source https://pkgs.dev.azure.com/$ORG/_packaging/$FEEDNAME/nuget/v3/index.json `
  -Username $USERNAME `
  -Password $PAT
PS> $env:VCPKG_BINARY_SOURCES="nuget,ADO,readwrite"
```

```bash
# On Linux or OSX
$ mono `vcpkg fetch nuget | tail -n1` sources add \
  -name ADO \
  -Source https://pkgs.dev.azure.com/$ORG/_packaging/$FEEDNAME/nuget/v3/index.json \
  -Username $USERNAME \
  -Password $PAT
$ export VCPKG_BINARY_SOURCES="nuget,ADO,readwrite"
```

We recommend using a Personal Access Token (PAT) as the password for maximum security. You can generate a PAT in *User Settings* -> *Personal Access Tokens* or `https://dev.azure.com/<ORG>/_usersSettings/tokens`.

## ABI Hash

> [!NOTE]
> Information on the ABI Hash is provided as an implementation note and will change without notice.

For every build, vcpkg calculates an _ABI Hash_ to determine equivalence. If two builds have the same ABI Hash, vcpkg will consider them identical and reuse the binaries across projects and machines.

The ABI Hash considers:

- Every file in the port directory
- The triplet file contents and name
- The C++ compiler executable file
- The C compiler executable file
- The set of [features](manifests.md#features) selected
- The ABI Hash of each dependency
- All helper functions referenced by `portfile.cmake` (heuristic)
- The version of CMake used
- The contents of any environment variables listed in [`VCPKG_ENV_PASSTHROUGH`](triplets.md#vcpkg_env_passthrough)
- The toolchain file's textual contents ([`VCPKG_CHAINLOAD_TOOLCHAIN_FILE`](triplets.md#vcpkg_chainload_toolchain_file))

Despite this extensive list, it is possible to defeat the cache and introduce nondeterminism. If you have additional details that you need to track for your environment, you can generate a triplet file with your additional information in a comment. That additional information will be included in the ABI Hash and ensure a unique universe of binaries.

The calculated ABI Hashes are stored in each package and in the current installed directory at `/share/<port>/vcpkg_abi_info.txt` for inspection.
