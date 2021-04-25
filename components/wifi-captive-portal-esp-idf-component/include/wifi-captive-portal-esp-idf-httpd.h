#ifndef __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_H_INCLUDED__
#define __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_H_INCLUDED__
/**	wifi-captive-portal-esp-idf-component

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.

		Contains some modified example code from here:
		https://github.com/espressif/esp-idf/blob/release/v4.2/examples/protocols/openssl_server/main/openssl_server_example.h

		Original Example Code Header:
		This example code is in the Public Domain (or CC0 licensed, at your option.)

		Unless required by applicable law or agreed to in writing, this
		software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
		CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_event_base.h"

// #define OPENSSL_EXAMPLE_TASK_STACK_WORDS 10240
// #define OPENSSL_EXAMPLE_TASK_PRIORITY    8

// #define OPENSSL_EXAMPLE_RECV_BUF_LEN       1024

// #define OPENSSL_EXAMPLE_LOCAL_TCP_PORT     443

extern SemaphoreHandle_t xMutexHTTPD;

/** An event base type for "wifi-captive-portal-esp-idf-httpd". */
ESP_EVENT_DECLARE_BASE(WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT);
enum	/**< The events. */
{
	WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT_FINISH,	/**< An event for "this task is finished". */
};

/** The event loop handle. */
extern esp_event_loop_handle_t wifi_captive_portal_esp_idf_httpd_event_loop_handle;

// /** Task action IDs of actions this task can perform. */
// enum wifi_captive_portal_esp_idf_httpd_task_action_id {

// };

// /** Task action values. */
// struct wifi_captive_portal_esp_idf_httpd_task_action_value_t {

// };

// typedef void* wifi_captive_portal_esp_idf_httpd_task_action_value_void_p;

// #define _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_CAST_VOID_P(x) (wifi_captive_portal_esp_idf_httpd_task_action_value_void_p)&x
// #define WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_CAST_VOID_P(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_CAST_VOID_P(x)

// #define _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_COPY(x) *(struct wifi_captive_portal_esp_idf_httpd_task_action_value_t*)x
// #define WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_COPY(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_COPY(x)

// /** Task actions. */
// struct wifi_captive_portal_esp_idf_httpd_task_action_t {
// 	enum wifi_captive_portal_esp_idf_httpd_task_action_id id;
// 	wifi_captive_portal_esp_idf_httpd_task_action_value_void_p value;
// };

// typedef void* wifi_captive_portal_esp_idf_httpd_task_action_void_p;

// #define _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_CAST_VOID_P(x) (wifi_captive_portal_esp_idf_httpd_task_action_void_p)&x
// #define WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_CAST_VOID_P(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_CAST_VOID_P(x)

// #define _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_COPY(x) *(struct wifi_captive_portal_esp_idf_httpd_task_action_t*)x
// #define WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_COPY(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_COPY(x)

#ifdef __cplusplus
extern "C"
{
#endif

// esp_err_t init_fs(void);

// esp_err_t start_httpd(const char *base_path);

// /** The esp-idf task function. */
void wifi_captive_portal_esp_idf_httpd_task(void *pvParameter);

#ifdef __cplusplus
}
#endif

#endif
