#pragma once

//! @file
//!
//! Copyright (c) 2019-Present Memfault, Inc.
//! See License.txt for details
//!
//! @brief
//! Math helpers

#ifdef __cplusplus
extern "C" {
#endif

#define MEMFAULT_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define MEMFAULT_MIN(a, b)   (((a) < (b)) ? (a) : (b))
#define MEMFAULT_MAX(a , b)  (((a) > (b)) ? (a) : (b))
#define MEMFAULT_FLOOR(a, align) (((a) / (align)) * (align))

#ifdef __cplusplus
}
#endif
