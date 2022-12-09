---
title: vcpkg_qmake_configure
description: Learn how to use vcpkg_qmake_configure.
ms.date: 11/30/2022
---
# vcpkg_qmake_configure

Configure a qmake-based project.

## Usage

```cmake
vcpkg_qmake_configure(
    SOURCE_PATH <pro_file_path>
    [QMAKE_OPTIONS arg1 [arg2 ...]]
    [QMAKE_OPTIONS_RELEASE arg1 [arg2 ...]]
    [QMAKE_OPTIONS_DEBUG arg1 [arg2 ...]]
    [OPTIONS arg1 [arg2 ...]]
    [OPTIONS_RELEASE arg1 [arg2 ...]]
    [OPTIONS_DEBUG arg1 [arg2 ...]]
)
```

To use this function, you must depend on the helper port `vcpkg-qmake`:

```json
"dependencies": [
  {
    "name": "vcpkg-qmake",
    "host": true
  }
]
```

## Parameters

### SOURCE_PATH
The path to the *.pro qmake project file.

### QMAKE_OPTIONS, QMAKE_OPTIONS_RELEASE, QMAKE_OPTIONS_DEBUG
Additional options to pass to qmake before `--`.

Typically, these should have the form QMAKE_X=_\<something\>_ or CONFIG=_\<something\>_

### OPTIONS, OPTIONS_RELEASE, OPTIONS_DEBUG
Additional options to pass to qmake after `--`.

## Triplet Parameters

### VCPKG_OSX_DEPLOYMENT_TARGET
Determines `QMAKE_MACOSX_DEPLOYMENT_TARGET`

### VCPKG_QMAKE_COMMAND
Path to qmake.

Defaults to `"${CURRENT_HOST_INSTALLED_DIR}/tools/Qt6/bin/qmake${VCPKG_HOST_EXECUTABLE_SUFFIX}"`.

### VCPKG_QT_CONF_RELEASE
Path to the `qt.config` being used for release mode.

Defaults to `"${CURRENT_INSTALLED_DIR}/tools/Qt6/qt_release.conf"`.

### VCPKG_QT_CONF_DEBUG
Path to the `qt.config` being used for debug mode.

Defaults to `"${CURRENT_INSTALLED_DIR}/tools/Qt6/qt_debug.conf"`

### VCPKG_QT_TARGET_MKSPEC
_Added in `vcpkg-qmake` version `2022-11-16`_.

Qt mkspec to use

### VCPKG_QMAKE_OPTIONS_RELEASE
Extra options to pass to `qmake` in release mode.

### VCPKG_QMAKE_OPTIONS_DEBUG
Extra options to pass to `qmake` in debug mode.

## Source

[ports/vcpkg-qmake/vcpkg\_qmake\_configure.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-qmake/vcpkg_qmake_configure.cmake)

