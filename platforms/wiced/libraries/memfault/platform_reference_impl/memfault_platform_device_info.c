//! @file
//!
//! Copyright (c) 2019-Present Memfault, Inc.
//! See License.txt for details
//!
//! @brief
//! Reference implementation of platform dependencies for Memfault device info APIs

#include "memfault/core/platform/device_info.h"

#include "wiced_result.h"
#include "wwd_wifi.h"

#include <stdio.h>
#include <string.h>

#ifndef MEMFAULT_WICED_MAIN_FIRMWARE_VERSION
#  define MEMFAULT_WICED_MAIN_FIRMWARE_VERSION "1.0.0"
#endif

#ifndef MEMFAULT_WICED_HW_REVISION
#  define MEMFAULT_WICED_HW_REVISION "wiced-proto"
#endif

static wiced_mac_t s_memfault_platform_device_info_mac = {0};

bool memfault_platform_device_info_boot(void) {
  // NOTE: This API will use an RTOS lock, so we cannot run this at the time of capturing the coredump!
  // Use the WiFi station MAC address as unique device id:
  return (WWD_SUCCESS == wwd_wifi_get_mac_address(&s_memfault_platform_device_info_mac, WWD_STA_INTERFACE));
}

void memfault_platform_get_unique_device_id(char *buf, size_t buf_len) {
  size_t curr_idx = 0;
  for (size_t i = 0; i < sizeof(s_memfault_platform_device_info_mac); i++) {
    size_t space_left = buf_len - curr_idx;
    int bytes_written = snprintf(&buf[curr_idx], space_left, "%02X", (int)s_memfault_platform_device_info_mac.octet[i]);
    if (bytes_written < space_left) {
      curr_idx += bytes_written;
    } else { // we are out of space, return what we got, it's NULL terminated
      return;
    }
  }
}

void memfault_platform_get_device_info(struct MemfaultDeviceInfo *info) {
  static char s_fw_version[32];
  strncpy(s_fw_version, MEMFAULT_WICED_MAIN_FIRMWARE_VERSION, sizeof(s_fw_version));

  static char s_device_serial[MEMFAULT_MAX_SERIAL_NUMBER_LEN];
  memfault_platform_get_unique_device_id(s_device_serial, sizeof(s_device_serial));

  static char s_hw_rev[MEMFAULT_MAX_HW_REVISION_LEN];
  strncpy(s_hw_rev, MEMFAULT_WICED_HW_REVISION, sizeof(s_hw_rev));

  *info = (struct MemfaultDeviceInfo) {
    .device_serial = s_device_serial,
    .hardware_version = s_hw_rev,
    .fw_version = s_fw_version,
  };
}
