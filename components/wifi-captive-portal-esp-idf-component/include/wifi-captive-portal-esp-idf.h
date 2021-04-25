#ifndef __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_H_INCLUDED__
#define __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_H_INCLUDED__
/**	wifi-captive-portal-esp-idf-component - example - http-slideshow

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
// #include "esp_sleep.h"
// #include "driver/rtc_io.h"
// #include "soc/rtc.h"
// #if CONFIG_EXAMPLE_CONNECT_WIFI
#include "esp_wifi.h"
// #endif
#include "protocol_examples_common.h"
// #include "wifi-captive-portal-esp-idf-device.h"
// /** Include the header file for the e-paper display device
//   that you selected in the Kconfig menu settings. It defines 
//   a "EpaperIDFDevice" class which can be used to interact 
//   with your e-paper display device. Choose your e-paper device
//   from the Kconfig menu, and make sure to set the correct pin
//   mappings while you're at it. */
// #include WIFI_CAPTIVE_PORTAL_ESP_IDF_DEVICE_HEADER
// #include "wifi-captive-portal-esp-idf-task.h"
#include "wifi-captive-portal-esp-idf-wifi.h"
// #include "wifi-captive-portal-esp-idf-ota.h"
// #include "wifi-captive-portal-esp-idf-http.h"
#include "wifi-captive-portal-esp-idf-httpd.h"

// #ifndef WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS_POS_MIN
// #define WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS_POS_MIN 15
// #endif

// #ifndef WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS_NEG_MAX
// #define WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS_NEG_MAX -15
// #endif

#ifdef __cplusplus
extern "C"
{
#endif

void wifi_captive_portal_esp_idf_task(void *pvParameter);

void wifi_captive_portal_esp_idf(void);

// #define main_fn wifi_captive_portal_esp_idf

#ifdef __cplusplus
}
#endif

#endif
