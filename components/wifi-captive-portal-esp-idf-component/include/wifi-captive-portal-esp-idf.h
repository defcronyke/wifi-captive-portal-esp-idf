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
#include "esp_wifi.h"
#include "protocol_examples_common.h"
#include "wifi-captive-portal-esp-idf-wifi.h"
#include "wifi-captive-portal-esp-idf-httpd.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wifi_captive_portal_esp_idf_task(void *pvParameter);

void wifi_captive_portal_esp_idf(void);

#ifdef __cplusplus
}
#endif

#endif
