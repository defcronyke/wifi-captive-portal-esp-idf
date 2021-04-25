# wifi-captive-portal-esp-idf-component
#
# Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>
#
# This code is released under the license terms contained in the
# file named LICENSE, which is found in the top-level folder in
# this project. You must agree to follow those license terms,
# otherwise you aren't allowed to copy, distribute, or use any
# part of this project in any way.

idf_component_register(
  SRCS
    "wifi-captive-portal-esp-idf.c"
    "wifi-captive-portal-esp-idf-dns.c"
    "wifi-captive-portal-esp-idf-httpd.c"
    "wifi-captive-portal-esp-idf-wifi.c"

    $ENV{IDF_PATH}/components/fatfs/diskio/diskio.c
    $ENV{IDF_PATH}/components/fatfs/src/ff.c
    $ENV{IDF_PATH}/components/fatfs/vfs/vfs_fat.c
    $ENV{IDF_PATH}/components/lwip/lwip/src/core/ipv4/ip4_addr.c
    $ENV{IDF_PATH}/components/lwip/lwip/src/core/dns.c
    $ENV{IDF_PATH}/components/wear_levelling/wear_levelling.cpp
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_main.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_parse.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_sess.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_txrx.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_uri.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_ws.c
    $ENV{IDF_PATH}/components/esp_http_server/src/util/ctrl_sock.c

  # REQUIRES
  #   ""

  INCLUDE_DIRS
    "include"

    $ENV{IDF_PATH}/components/bootloader_support/include
    $ENV{IDF_PATH}/components/esp_http_server/include
    $ENV{IDF_PATH}/components/esp_http_server/src
    $ENV{IDF_PATH}/components/esp_http_server/src/port/esp32
    $ENV{IDF_PATH}/components/esp_http_server/src/util
    $ENV{IDF_PATH}/components/fatfs/diskio
    $ENV{IDF_PATH}/components/fatfs/src
    $ENV{IDF_PATH}/components/fatfs/vfs
    $ENV{IDF_PATH}/components/lwip/lwip/src/include
    $ENV{IDF_PATH}/components/mbedtls/esp_crt_bundle/include
    $ENV{IDF_PATH}/components/mbedtls/mbedtls/include
    $ENV{IDF_PATH}/components/mbedtls/port/include
    $ENV{IDF_PATH}/components/nghttp/port/include
    $ENV{IDF_PATH}/components/nvs_flash/include
    $ENV{IDF_PATH}/components/spi_flash/include
    $ENV{IDF_PATH}/components/ulp/include
    $ENV{IDF_PATH}/components/wear_levelling/include
    $ENV{IDF_PATH}/components/wear_levelling/private_include
    $ENV{IDF_PATH}/components/esp_wifi/include

    $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/include

  # EMBED_TXTFILES
  #   ""
)

set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)

# add_compile_definitions(
# )

# # TODO: Enable support for reading website files from SPIFFS.
#
# if(CONFIG_EXAMPLE_WEB_DEPLOY_SF)
#
# 	set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/web")
#
#   # TODO: If you use this path to store your website files,
#   # your site can be easily deployed on GitLab Pages
#   # static website hosting. You might like to do that
#   # so your users can see a demo of the site which runs
#   # on the ESP firmware. You can also copy the contents
#   # of this folder to the folder ${CMAKE_CURRENT_SOURCE_DIR}/docs,
#   # and then the site can be easily deployed on GitHub
#   # Pages if you want, but if you do that you don't need
#   # to reference that folder in this file as with the
#   # line below.
#
#   set(WEB_DIST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/public")
#
#   # TODO: Use minified production version of the site if
#   # available (maybe you do it using Webpack).
#
# 	if(EXISTS ${WEB_DIST_DIR})
# 		spiffs_create_partition_image(www ${WEB_DIST_DIR} FLASH_IN_PROJECT)
#
#   else()
# 		message(FATAL_ERROR "${WEB_DIST_DIR} doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
#
# 	endif()
#
# endif()
