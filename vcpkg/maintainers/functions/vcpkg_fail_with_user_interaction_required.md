---
title: vcpkg_fail_with_user_interaction_required
description: Learn how to use vcpkg_fail_with_user_interaction_required.
ms.date: 04/11/2023
---
# vcpkg_fail_with_user_interaction_required

Outputs the message to the console and stops the execution of the `portfile.cmake`. 

In comparison to `message(FATAL_ERROR <message>)` vcpkg don't emit instructions on how to report an issue and the `<message>` is printed instead. 

## Usage

```cmake
vcpkg_fail_with_user_interaction_required(<message>)
```

## Parameters

### `<message>`

The error message that should be displayed to the user.

## Source

[scripts/cmake/vcpkg\_fail\_with\_user\_interaction\_required.cmake](https://github.com/Microsoft/vcpkg/blob/master/scripts/cmake/vcpkg_fail_with_user_interaction_required.cmake)

