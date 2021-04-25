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
    # "wifi-captive-portal-esp-idf-http.c"
    "wifi-captive-portal-esp-idf-httpd.c"
    # "wifi-captive-portal-esp-idf-io.cpp"
    # "wifi-captive-portal-esp-idf-ota.c"
    # "wifi-captive-portal-esp-idf-spi.cpp"
    "wifi-captive-portal-esp-idf-wifi.c"

    # "device/Gdew075T8.cpp"

    # "task/http-slideshow.cpp"

    # $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/connect.c
    # $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/addr_from_stdin.c
    # $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/stdin_out.c

    # $ENV{IDF_PATH}/components/esp_https_ota/src/esp_https_ota.c
    $ENV{IDF_PATH}/components/fatfs/diskio/diskio.c
    $ENV{IDF_PATH}/components/fatfs/src/ff.c
    $ENV{IDF_PATH}/components/fatfs/vfs/vfs_fat.c
    # $ENV{IDF_PATH}/components/json/cJSON/cJSON.c
    # $ENV{IDF_PATH}/components/json/cJSON/cJSON_Utils.c
    $ENV{IDF_PATH}/components/lwip/lwip/src/core/ipv4/ip4_addr.c
    $ENV{IDF_PATH}/components/lwip/lwip/src/core/dns.c
    # $ENV{IDF_PATH}/components/openssl/library/ssl_methods.c
    # $ENV{IDF_PATH}/components/openssl/library/ssl_lib.c
    # $ENV{IDF_PATH}/components/openssl/library/ssl_x509.c
    # $ENV{IDF_PATH}/components/openssl/library/ssl_pkey.c
    # $ENV{IDF_PATH}/components/openssl/library/ssl_cert.c
    # $ENV{IDF_PATH}/components/openssl/platform/ssl_pm.c
    # $ENV{IDF_PATH}/components/openssl/platform/ssl_port.c
    # $ENV{IDF_PATH}/components/sdmmc/sdmmc_cmd.c
    # $ENV{IDF_PATH}/components/spiffs/esp_spiffs.c
    # $ENV{IDF_PATH}/components/spiffs/spiffs_api.c
    # $ENV{IDF_PATH}/components/spiffs/spiffs/src/spiffs_cache.c
    # $ENV{IDF_PATH}/components/spiffs/spiffs/src/spiffs_gc.c
    # $ENV{IDF_PATH}/components/spiffs/spiffs/src/spiffs_hydrogen.c
    # $ENV{IDF_PATH}/components/spiffs/spiffs/src/spiffs_nucleus.c
    $ENV{IDF_PATH}/components/wear_levelling/wear_levelling.cpp
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_main.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_parse.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_sess.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_txrx.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_uri.c
    $ENV{IDF_PATH}/components/esp_http_server/src/httpd_ws.c
    $ENV{IDF_PATH}/components/esp_http_server/src/util/ctrl_sock.c
    # $ENV{IDF_PATH}/components/esp_https_server/src/https_server.c

  # REQUIRES
  #   "Adafruit-GFX-Component"

  INCLUDE_DIRS
    "include"

    # $ENV{IDF_PATH}/components/app_update/include
    $ENV{IDF_PATH}/components/bootloader_support/include
    # $ENV{IDF_PATH}/components/esp_http_client/include
    # $ENV{IDF_PATH}/components/openssl/include
    # $ENV{IDF_PATH}/components/openssl/include/openssl
    # $ENV{IDF_PATH}/components/openssl/include/internal
    # $ENV{IDF_PATH}/components/openssl/include/platform
    $ENV{IDF_PATH}/components/esp_http_server/include
    $ENV{IDF_PATH}/components/esp_http_server/src
    $ENV{IDF_PATH}/components/esp_http_server/src/port/esp32
    $ENV{IDF_PATH}/components/esp_http_server/src/util
    # $ENV{IDF_PATH}/components/esp_https_server/include
    # $ENV{IDF_PATH}/components/esp_https_ota/include
    $ENV{IDF_PATH}/components/fatfs/diskio
    $ENV{IDF_PATH}/components/fatfs/src
    $ENV{IDF_PATH}/components/fatfs/vfs
    # $ENV{IDF_PATH}/components/json/cJSON
    $ENV{IDF_PATH}/components/lwip/lwip/src/include
    $ENV{IDF_PATH}/components/mbedtls/esp_crt_bundle/include
    $ENV{IDF_PATH}/components/mbedtls/mbedtls/include
    $ENV{IDF_PATH}/components/mbedtls/port/include
    $ENV{IDF_PATH}/components/nghttp/port/include
    $ENV{IDF_PATH}/components/nvs_flash/include
    # $ENV{IDF_PATH}/components/sdmmc/include
    $ENV{IDF_PATH}/components/spi_flash/include
    # $ENV{IDF_PATH}/components/spiffs
    # $ENV{IDF_PATH}/components/spiffs/include
    # $ENV{IDF_PATH}/components/spiffs/spiffs/src
    $ENV{IDF_PATH}/components/ulp/include
    $ENV{IDF_PATH}/components/wear_levelling/include
    $ENV{IDF_PATH}/components/wear_levelling/private_include
    # $ENV{IDF_PATH}/components/esp-tls
    $ENV{IDF_PATH}/components/esp_wifi/include

    $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/include

  # # Embed the server root certificate into the final binary
  # EMBED_TXTFILES 
  #   ${project_dir}/certs/ca_cert.pem
  #   ${project_dir}/certs/ca_cert_conf.pem
  #   ${project_dir}/certs/ca_key_conf.pem
)

# (Not part of the boilerplate)
# This example uses an extra component for common functions such as Wi-Fi 
# and Ethernet connection.
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)

# add_compile_definitions(
#   # # Set the EpaperIDF class version by setting these environment variables.
#   # # Increase the major or minor version numbers when the class is no longer 
#   # # backwards-compatible. It breaks backwards-compatibility by changing the
#   # # name of the EpaperIDF class.
#   # # Default version if unspecified: v0.1.0
#   # EPAPER_IDF_VERSION_MAJOR=$ENV{EPAPER_IDF_VERSION_MAJOR}
#   # EPAPER_IDF_VERSION_MINOR=$ENV{EPAPER_IDF_VERSION_MINOR}
#   # EPAPER_IDF_VERSION_MICRO=$ENV{EPAPER_IDF_VERSION_MICRO}

#   # # This is an ESP32 IDF project.
#   # ESP32_IDF
# )

# if(CONFIG_EXAMPLE_WEB_DEPLOY_SF)

# 	set(WEB_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/web")
#   set(WEB_DIST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/public")

#   # Use minified production version of the site if available.
# 	if(EXISTS ${WEB_DIST_DIR})
# 		spiffs_create_partition_image(www ${WEB_DIST_DIR} FLASH_IN_PROJECT)
	
#   else()
# 		message(FATAL_ERROR "${WEB_DIST_DIR} doesn't exit. Please run 'npm run build' in ${WEB_SRC_DIR}")
# 	endif()

# endif()
