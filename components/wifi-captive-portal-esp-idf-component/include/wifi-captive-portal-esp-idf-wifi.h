#ifndef __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_H_INCLUDED__
#define __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_H_INCLUDED__
/**	wifi-captive-portal-esp-idf-component

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.
*/
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_base.h"
#include "wifi-captive-portal-esp-idf-dns.h"

/** An event base type for "wifi-captive-portal-esp-idf-wifi". */
ESP_EVENT_DECLARE_BASE(WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT);
enum /**< The events. */
{
	WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_FINISH,	/**< An event for "this task is finished". */
	WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_STOPPED, /**< Do something after the wifi interface has stopped. */
};

/** The event loop handle. */
extern esp_event_loop_handle_t wifi_captive_portal_esp_idf_wifi_event_loop_handle;

/** Task action IDs of actions this task can perform. */
enum wifi_captive_portal_esp_idf_wifi_task_action_id
{
	WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_CONNECT = 1U, /**< Initialize (if necessary), and connect to a wifi network. */
	WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_DISCONNECT,	 /**< Disconnect the wifi interface. */
	WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_STOP,				 /**< Disconnect and stop the wifi interface. */
};

/** Task action values. */
struct wifi_captive_portal_esp_idf_wifi_task_action_value_t
{
	bool no_deep_sleep;				 /**< If true, don't deep sleep after the wifi request. */
	uint64_t deep_sleep_usecs; /**< Deep sleep for this number of microseconds (seconds * 1000000). */
};

typedef void *wifi_captive_portal_esp_idf_wifi_task_action_value_void_p;

#define _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_CAST_VOID_P(x) (wifi_captive_portal_esp_idf_wifi_task_action_value_void_p) & x
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_CAST_VOID_P(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_CAST_VOID_P(x)

#define _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_COPY(x) *(struct wifi_captive_portal_esp_idf_wifi_task_action_value_t *)x
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_COPY(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_COPY(x)

/** Task actions. */
struct wifi_captive_portal_esp_idf_wifi_task_action_t
{
	enum wifi_captive_portal_esp_idf_wifi_task_action_id id;
	wifi_captive_portal_esp_idf_wifi_task_action_value_void_p value;
};

typedef void *wifi_captive_portal_esp_idf_wifi_task_action_void_p;

#define _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_CAST_VOID_P(x) (wifi_captive_portal_esp_idf_wifi_task_action_void_p) & x
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_CAST_VOID_P(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_CAST_VOID_P(x)

#define _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_COPY(x) *(struct wifi_captive_portal_esp_idf_wifi_task_action_t *)x
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_COPY(x) _WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_COPY(x)

#ifdef __cplusplus
extern "C"
{
#endif

	/** The esp-idf task function. */
	void wifi_captive_portal_esp_idf_wifi_task(void *pvParameter);

#ifdef __cplusplus
}
#endif

#endif
