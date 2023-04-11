---
title: vcpkg_user_hint_on_failure
description: Learn how to use vcpkg_user_hint_on_failure.
ms.date: 04/11/2023
---
# vcpkg_user_hint_on_failure

Outputs the message to the console if the building of the port fails.

The message is emitted after the instructions on how to report an issue as an hint how to resolve the build failure.

## Usage

```cmake
vcpkg_user_hint_on_failure(<message>)
```

## Parameters

### `<message>`

The hint message that should be displayed to the user if case of an build failure.

## Source

[scripts/cmake/vcpkg\_user\_hint\_on\_failure.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_user_hint_on_failure.cmake)

