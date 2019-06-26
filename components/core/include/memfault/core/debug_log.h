#pragma once

//! @file
//!
//! @brief
//! Log utility used within the memfault SDK. When enabled, logs will be emitted to help a user
//! undersatand what is happening in the library. The goal is only to emit useful logs rather than
//! be excessively spammy

#include "memfault/core/platform/debug_log.h"

#define MEMFAULT_LOG_DEBUG(...)                                             \
  memfault_platform_log(kMemfaultPlatformLogLevel_Debug, __VA_ARGS__)

#define MEMFAULT_LOG_INFO(...)                                              \
  memfault_platform_log(kMemfaultPlatformLogLevel_Info, __VA_ARGS__)

#define MEMFAULT_LOG_WARN(...)                                              \
  memfault_platform_log(kMemfaultPlatformLogLevel_Warning, __VA_ARGS__)

#define MEMFAULT_LOG_ERROR(...)                                             \
  memfault_platform_log(kMemfaultPlatformLogLevel_Error, __VA_ARGS__)