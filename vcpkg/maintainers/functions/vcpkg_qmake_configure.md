---
title: vcpkg_qmake_configure
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

## Parameters
### SOURCE_PATH
The path to the *.pro qmake project file.

### QMAKE_OPTIONS, QMAKE_OPTIONS_RELEASE, QMAKE_OPTIONS_DEBUG
Options directly passed to qmake with the form QMAKE_X=something or CONFIG=something 

### OPTIONS, OPTIONS_RELEASE, OPTIONS_DEBUG
The options passed after -- to qmake.

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

### VCPKG_QMAKE_OPTIONS_RELEASE
Extra options to pass to `qmake` in release mode.

### VCPKG_QMAKE_OPTIONS_DEBUG
Extra options to pass to `qmake` in debug mode.

## Source
[ports/vcpkg-qmake/vcpkg\_qmake\_configure.cmake](https://github.com/Microsoft/vcpkg/blob/master/ports/vcpkg-qmake/vcpkg_qmake_configure.cmake)

