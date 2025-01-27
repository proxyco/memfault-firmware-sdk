#pragma once

//! @file
//!
//! Copyright (c) 2019-Present Memfault, Inc.
//! See License.txt for details

#ifdef __cplusplus
extern "C" {
#endif

//! @brief
//! The error codes used by the Memfault SDK
typedef enum MemfaultReturnCode {
  MemfaultReturnCode_Ok = 0,
  MemfaultReturnCode_Error = -1,
  MemfaultReturnCode_InvalidInput = -2,
  MemfaultReturnCode_Empty = -3,
  MemfaultReturnCode_Full = -4,
  MemfaultReturnCode_NoConnection = -5,
  MemfaultReturnCode_DoesNotExist = -6,
  MemfaultReturnCode_NotSupported = -7,
  MemfaultReturnCode_Timeout = -8,
} MemfaultReturnCode;

#ifdef __cplusplus
}
#endif
