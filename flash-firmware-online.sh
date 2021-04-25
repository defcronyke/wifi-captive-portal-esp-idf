#!/bin/bash
#
# wifi-captive-portal-esp-idf
#
# Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>
#
# This code is released under the license terms contained in the
# file named LICENSE, which is found in the top-level folder in
# this project. You must agree to follow those license terms,
# otherwise you aren't allowed to copy, distribute, or use any 
# part of this project in any way.

wifi_captive_portal_esp_idf_flash_firmware_online() {
  GITLAB_USER=${GITLAB_USER:-"defcronyke"}
  GITLAB_REPO=${GITLAB_REPO:-"wifi-captive-portal-esp-idf"}
  SERIAL_PORT=${SERIAL_PORT:-"/dev/ttyUSB0"}
  BAUD_RATE=${BAUD_RATE:-"115200"}
  FLASH_SIZE=${FLASH_SIZE:-"4MB"}

  if [ $# -ge 1 ]; then
    SERIAL_PORT="$1"
  fi

  if [ $# -ge 2 ]; then
    BAUD_RATE="$2"
  fi

  if [ $# -ge 3 ]; then
    FLASH_SIZE="$3"
  fi

  URL_BASE="https://gitlab.com/$GITLAB_USER/$GITLAB_REPO/builds/artifacts/$GIT_REPO_BRANCH/raw/"; \
  FILENAME="wifi-captive-portal-esp-idf-$GIT_REPO_BRANCH.bin"; \
  PARTITION_FILENAME="wifi-captive-portal-esp-idf-partition-table-$GIT_REPO_BRANCH.bin"; \
  BOOTLOADER_FILENAME="wifi-captive-portal-esp-idf-bootloader-$GIT_REPO_BRANCH.bin"; \
  curl -sL ${URL_BASE}partition-table.bin?job=build-job \
  > "$PARTITION_FILENAME" && \
  curl -sL ${URL_BASE}bootloader.bin?job=build-job \
  > "$BOOTLOADER_FILENAME" && \
  curl -sL ${URL_BASE}wifi-captive-portal-esp-idf.bin?job=build-job \
  > "$FILENAME" && \
  esptool.py -p "$SERIAL_PORT" -b "$BAUD_RATE" \
  write_flash --flash_size "$FLASH_SIZE" \
  0x1000 "$BOOTLOADER_FILENAME" \
  0x8000 "$PARTITION_FILENAME" \
  0x10000 "$FILENAME"; \
  rm "$PARTITION_FILENAME"; \
  rm "$BOOTLOADER_FILENAME"; \
  rm "$FILENAME"
}

wifi_captive_portal_esp_idf_flash_firmware_online $@
