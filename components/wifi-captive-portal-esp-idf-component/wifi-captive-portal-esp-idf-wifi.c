/**	wifi-captive-portal-esp-idf-component

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.

		Contains some modified example code from here:
		https://github.com/espressif/esp-idf/blob/release/v4.2/examples/system/ota/advanced_https_ota/main/advanced_https_ota_example.c

		Original Example Code Header:
		This code is in the Public Domain (or CC0 licensed, at your option.)

		Unless required by applicable law or agreed to in writing, this
		software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
		CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "lwip/dns.h"
#include "protocol_examples_common.h"
#include "wifi-captive-portal-esp-idf-wifi.h"

const char *wifi_captive_portal_esp_idf_wifi_tag = "wifi-captive-portal-esp-idf-wifi";

esp_event_loop_handle_t wifi_captive_portal_esp_idf_wifi_event_loop_handle;
static struct wifi_captive_portal_esp_idf_wifi_task_action_t wifi_task_action;

ESP_EVENT_DEFINE_BASE(WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT);

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static EventGroupHandle_t wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
															 int32_t event_id, void *event_data)
{
	if (event_id == WIFI_EVENT_AP_STACONNECTED)
	{
		wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
		ESP_LOGI(wifi_captive_portal_esp_idf_wifi_tag, "station " MACSTR " join, AID=%d",
						 MAC2STR(event->mac), event->aid);
	}
	else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
	{
		wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
		ESP_LOGI(wifi_captive_portal_esp_idf_wifi_tag, "station " MACSTR " leave, AID=%d",
						 MAC2STR(event->mac), event->aid);
	}
}

static void wifi_captive_portal_esp_idf_wifi_ap_init(void)
{
	wifi_config_t wifi_config_ap = {
			.ap = {
					.ssid = CONFIG_EXAMPLE_WIFI_AP_SSID,
					.ssid_len = strlen(CONFIG_EXAMPLE_WIFI_AP_SSID),
					.channel = CONFIG_EXAMPLE_WIFI_AP_CHANNEL,
					.password = CONFIG_EXAMPLE_WIFI_AP_PASSWORD,
					.max_connection = CONFIG_EXAMPLE_MAX_AP_CONN,
					.authmode = WIFI_AUTH_WPA_WPA2_PSK},
	};
	if (strlen(CONFIG_EXAMPLE_WIFI_AP_PASSWORD) == 0)
	{
		wifi_config_ap.ap.authmode = WIFI_AUTH_OPEN;
	}

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_ap));

	ESP_LOGI(wifi_captive_portal_esp_idf_wifi_tag, "starting WiFi access point: SSID: %s password:%s channel: %d",
					 CONFIG_EXAMPLE_WIFI_AP_SSID, CONFIG_EXAMPLE_WIFI_AP_PASSWORD, CONFIG_EXAMPLE_WIFI_AP_CHANNEL);

	/** Wifi captive portal DNS init. */
	wifi_captive_portal_esp_idf_dns_init();
}

static void wifi_captive_portal_esp_idf_wifi_init(void)
{
	/** Initialize NVS. */
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);

	ESP_ERROR_CHECK(esp_netif_init());

	wifi_event_group = xEventGroupCreate();

	esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
	assert(ap_netif);

	esp_netif_ip_info_t ip_info;

	/** NOTE: This is where you set the access point (AP) IP address 
			and gateway address. It has to be a class A internet address
			otherwise the captive portal sign-in prompt won't show up	on 
			Android when you connect to the access point. */
	IP4_ADDR(&ip_info.ip, 124, 213, 16, 29);
	IP4_ADDR(&ip_info.gw, 124, 213, 16, 29);
	IP4_ADDR(&ip_info.netmask, 255, 0, 0, 0);
	esp_netif_dhcps_stop(ap_netif);
	esp_netif_set_ip_info(ap_netif, &ip_info);
	esp_netif_dhcps_start(ap_netif);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

	ESP_ERROR_CHECK(esp_wifi_init(&cfg));
	wifi_captive_portal_esp_idf_wifi_ap_init();

	esp_event_handler_instance_t instance_any_id;

	ESP_ERROR_CHECK(
			esp_event_handler_instance_register(
					WIFI_EVENT,
					ESP_EVENT_ANY_ID,
					&wifi_event_handler,
					NULL,
					&instance_any_id));

	ESP_ERROR_CHECK(esp_wifi_start());
}

/** The WiFi task. */
void wifi_captive_portal_esp_idf_wifi_task(void *pvParameter)
{
	while (1)
	{
		while (1)
		{
			bool wifi_is_init = false;

			if (pvParameter != NULL)
			{
				wifi_task_action = WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_COPY(pvParameter);
			}

			esp_err_t err = ESP_OK;

			/** Init only once. */
			if (!wifi_is_init)
			{
				ESP_LOGI(wifi_captive_portal_esp_idf_wifi_tag, "wifi initializing...");

				/** Initialize wifi */
				wifi_captive_portal_esp_idf_wifi_init();

				wifi_is_init = true;
			}

			/** Send an event which says "this task is finished". */
			err = esp_event_post_to(wifi_captive_portal_esp_idf_wifi_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_FINISH, NULL, 0, portMAX_DELAY);
			if (err != ESP_OK)
			{
				ESP_LOGE(wifi_captive_portal_esp_idf_wifi_tag, "Sending event failed");
			}

			break;
		}

		vTaskDelete(NULL);
	}
}
