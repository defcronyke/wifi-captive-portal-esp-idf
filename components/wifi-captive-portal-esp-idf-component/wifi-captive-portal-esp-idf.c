/**	wifi-captive-portal-esp-idf-component

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.
*/
#include "wifi-captive-portal-esp-idf.h"

static struct wifi_captive_portal_esp_idf_wifi_task_action_t wifi_task_action;
static struct wifi_captive_portal_esp_idf_wifi_task_action_value_t wifi_task_action_value;

// static struct wifi_captive_portal_esp_idf_http_task_action_t http_task_action;
// static struct wifi_captive_portal_esp_idf_http_task_action_value_t http_task_action_value;

// static struct wifi_captive_portal_esp_idf_httpd_task_action_t httpd_task_action;
// static struct wifi_captive_portal_esp_idf_httpd_task_action_value_t httpd_task_action_value;

const char* TAG = "wifi-captive-portal-esp-idf";

const char* wifi_captive_portal_esp_idf_wifi_task_name = "wifi_captive_portal_esp_idf_wifi_task";
const uint32_t wifi_captive_portal_esp_idf_wifi_task_stack_depth = 4096;
UBaseType_t wifi_captive_portal_esp_idf_wifi_task_priority = 5;

const char* wifi_captive_portal_esp_idf_httpd_task_name = "wifi_captive_portal_esp_idf_httpd_task";
const uint32_t wifi_captive_portal_esp_idf_httpd_task_stack_depth = 4096;
UBaseType_t wifi_captive_portal_esp_idf_httpd_task_priority = 5;

const char* wifi_captive_portal_esp_idf_task_name = "wifi_captive_portal_esp_idf_task";

// #ifdef CONFIG_EXAMPLE_WIFI_AP_STARTUP_ALWAYS_ON_OPT
// static bool no_deep_sleep_first = true;
// #endif

/** Initialize the task. */
static void wifi_captive_portal_esp_idf_task_init(void) {
	// /** Use the appropriate epaper device. */
	// io = new EpaperIDFSPI();
	// dev = new EpaperIDFDevice(*io);
}

/** Clean up the task. */
static void wifi_captive_portal_esp_idf_task_cleanup(void) {
	// delete dev;
	// delete io;
}

static void wifi_captive_portal_esp_idf_wifi_stopped_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
	ESP_LOGI(TAG, "event received: WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_STOPPED");

	// if (event_data != NULL) {
	// 	wifi_task_action_value = WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_COPY(event_data);
	// 	ESP_LOGI(TAG, "event data received: deep sleep secs: %f", (float)wifi_task_action_value.deep_sleep_usecs / 1000000.f);
	// }

	// wifi_captive_portal_esp_idf_deep_sleep(wifi_task_action_value.deep_sleep_usecs);
}

static void wifi_captive_portal_esp_idf_wifi_finish_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
	ESP_LOGI(TAG, "event received: WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_FINISH");

	xTaskCreate(&wifi_captive_portal_esp_idf_httpd_task, wifi_captive_portal_esp_idf_httpd_task_name, wifi_captive_portal_esp_idf_httpd_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_httpd_task_priority, NULL);
	ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_httpd_task_name);

	// xTaskCreate(&wifi_captive_portal_esp_idf_httpd_task, wifi_captive_portal_esp_idf_httpd_task_name, wifi_captive_portal_esp_idf_httpd_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_httpd_task_priority, NULL);
	// ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_httpd_task_name);

	// // TODO: Start the HTTPD as a task.
	// ESP_ERROR_CHECK(start_httpd(CONFIG_EXAMPLE_WEB_MOUNT_POINT));
}

// static void sta_got_ip_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
// {
// 	ESP_LOGI(TAG, "event received: IP_EVENT_STA_GOT_IP");

// 	// xTaskCreate(&wifi_captive_portal_esp_idf_httpd_task, wifi_captive_portal_esp_idf_httpd_task_name, wifi_captive_portal_esp_idf_httpd_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_httpd_task_priority, NULL);
// 	// ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_httpd_task_name);

// 	// // TODO: Start the HTTPD as a task.
// 	// ESP_ERROR_CHECK(start_httpd(CONFIG_EXAMPLE_WEB_MOUNT_POINT));

// 	xTaskCreate(&wifi_captive_portal_esp_idf_ota_task, wifi_captive_portal_esp_idf_ota_task_name, wifi_captive_portal_esp_idf_ota_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_ota_task_priority, NULL);
// 	ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_ota_task_name);
// }

// static void wifi_captive_portal_esp_idf_ota_finish_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
// {
// 	ESP_LOGI(TAG, "event received: WIFI_CAPTIVE_PORTAL_ESP_IDF_OTA_EVENT_FINISH");

// 	xTaskCreate(&wifi_captive_portal_esp_idf_httpd_task, wifi_captive_portal_esp_idf_httpd_task_name, wifi_captive_portal_esp_idf_httpd_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_httpd_task_priority, NULL);
// 	ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_httpd_task_name);

// 	// // TODO: Start the HTTPD as a task.
// 	// ESP_ERROR_CHECK(start_httpd(CONFIG_EXAMPLE_WEB_MOUNT_POINT));

// 	xTaskCreate(&wifi_captive_portal_esp_idf_http_task, wifi_captive_portal_esp_idf_http_task_name, wifi_captive_portal_esp_idf_http_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_http_task_priority, NULL);
// 	ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_http_task_name);
// }

// static void wifi_captive_portal_esp_idf_http_finish_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
// {
// 	ESP_LOGI(TAG, "event received: WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_EVENT_FINISH");

// 	if (event_data != NULL) {
// 		http_task_action_value = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_TASK_ACTION_VALUE_COPY(event_data);
// 		ESP_LOGI(TAG, "event data received");
// 	}

// 	wifi_captive_portal_esp_idf_task(WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_TASK_ACTION_VALUE_CAST_VOID_P(http_task_action_value));
// }

static void wifi_captive_portal_esp_idf_httpd_finish_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
	ESP_LOGI(TAG, "event received: WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT_FINISH");

	// if (event_data != NULL) {
	// 	httpd_task_action_value = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_COPY(event_data);
	// 	ESP_LOGI(TAG, "event data received");
	// }
}

// // Enter deep sleep.
// static void wifi_captive_portal_esp_idf_deep_sleep(int32_t sleep_duration_us) {
// 	ESP_LOGI(TAG, "preparing for deep sleep");

// 	ESP_ERROR_CHECK(esp_event_handler_unregister_with(wifi_captive_portal_esp_idf_wifi_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_FINISH, wifi_captive_portal_esp_idf_wifi_finish_event_handler));
// 	// TODO: Can we unregister wifi_captive_portal_esp_idf_wifi_stopped_event_handler, and wifi_captive_portal_esp_idf_wifi_event_loop_handle here also?

// 	ESP_ERROR_CHECK(esp_event_handler_unregister_with(wifi_captive_portal_esp_idf_ota_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_OTA_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_OTA_EVENT_FINISH, wifi_captive_portal_esp_idf_ota_finish_event_handler));
// 	ESP_ERROR_CHECK(esp_event_loop_delete(wifi_captive_portal_esp_idf_ota_event_loop_handle));

// 	ESP_ERROR_CHECK(esp_event_handler_unregister_with(wifi_captive_portal_esp_idf_http_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_EVENT_FINISH, wifi_captive_portal_esp_idf_http_finish_event_handler));
// 	ESP_ERROR_CHECK(esp_event_loop_delete(wifi_captive_portal_esp_idf_http_event_loop_handle));

// 	ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, sta_got_ip_event_handler));
// 	ESP_ERROR_CHECK(esp_event_loop_delete_default());

// 	ESP_LOGI(TAG, "Enabling deep sleep timer wakeup after approx: %f secs", (float)sleep_duration_us / 1000000.f);
// 	ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(sleep_duration_us));

// 	// Isolate GPIO12 pin from external circuits. This is needed for modules
// 	// which have an external pull-up resistor on GPIO12 (such as ESP32-WROVER)
// 	// to minimize current consumption.
// 	ESP_ERROR_CHECK(rtc_gpio_isolate(GPIO_NUM_12));

// 	// Hibernate for lowest power consumption.
// 	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF));
// 	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF));
// 	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF));
// 	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF));

// 	ESP_LOGI(TAG, "entering deep sleep...\n");

// 	esp_deep_sleep_start();
// }

/** The main task. */
void wifi_captive_portal_esp_idf_task(void *pvParameter)
{
	struct timeval now;
	gettimeofday(&now, NULL);

	ESP_LOGI(TAG, "task %s is running", wifi_captive_portal_esp_idf_task_name);
	
	// int32_t delay_secs = (int32_t)CONFIG_WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS;

	// http_task_action_value = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_TASK_ACTION_VALUE_COPY(pvParameter);

// 	http_task_action_value.no_deep_sleep = true;
// 	if (delay_secs < 0)
// 	{	
// 		delay_secs = (int32_t)wifi_captive_portal_esp_idf_clamp((float)CONFIG_WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS, (float)INT32_MIN, (float)WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS_NEG_MAX) * -1;
// 	}
// 	else
// 	{
// #ifndef CONFIG_EXAMPLE_WIFI_AP_STARTUP_ALWAYS_ON_OPT
// 		http_task_action_value.no_deep_sleep = false;
// #else
// 		if (no_deep_sleep_first) {
// 			ESP_LOGI(TAG, "disabling deep sleep because \"WiFi Access Point Startup\" is set \"Always On\"");
// 			no_deep_sleep_first = false;
// 		}
// #endif

// 		delay_secs = (int32_t)wifi_captive_portal_esp_idf_clamp((float)CONFIG_WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS, (float)WIFI_CAPTIVE_PORTAL_ESP_IDF_DEEP_SLEEP_SECONDS_POS_MIN, (float)INT32_MAX);
// 	}

	// http_task_action = {
	// 	.id = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_TASK_ACTION_GET_INDEX_JSON,
	// 	.value = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_TASK_ACTION_VALUE_CAST_VOID_P(http_task_action_value),
	// };
		
	ESP_LOGI(TAG, "%s loop", wifi_captive_portal_esp_idf_task_name);
	
	/** Instantiate the io and dev variables. */
	wifi_captive_portal_esp_idf_task_init();

	/** NOTE:	Perform some e-paper device actions here,
						such as drawing to the screen.
	*/

	/** Delete the io and dev variables. */
	wifi_captive_portal_esp_idf_task_cleanup();
	
	// /** No deep sleep. */
	// if (http_task_action_value.no_deep_sleep)
	// {
	// 	ESP_LOGI(TAG, "waiting for %d secs\n", delay_secs);
	// 	vTaskDelay((delay_secs * 1000) / portTICK_PERIOD_MS);

	// 	wifi_captive_portal_esp_idf_http_get(http_task_action_value);
		
	// } else {	/**< Deep sleep. */
	// 	ESP_LOGI(TAG, "stopping the wifi interface");

	// 	wifi_task_action_value.no_deep_sleep = http_task_action_value.no_deep_sleep;
	// 	wifi_task_action_value.deep_sleep_usecs = (uint64_t)delay_secs * 1000000UL;

	// 	// Disable wifi for deep sleep.
	// 	wifi_task_action = {
	// 		.id = WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_STOP,
	// 		.value = WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_VALUE_CAST_VOID_P(wifi_task_action_value),
	// 	};

	// 	xTaskCreate(&wifi_captive_portal_esp_idf_wifi_task, wifi_captive_portal_esp_idf_wifi_task_name, wifi_captive_portal_esp_idf_wifi_task_stack_depth * 8, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_CAST_VOID_P(wifi_task_action), wifi_captive_portal_esp_idf_wifi_task_priority, NULL);
	// 	ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_wifi_task_name);
	// }
}

void wifi_captive_portal_esp_idf(void)
{
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	
	// ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, sta_got_ip_event_handler, NULL, NULL));

	esp_event_loop_args_t wifi_captive_portal_esp_idf_wifi_event_loop_args = {
			.queue_size = 5,
			.task_name = "wifi_captive_portal_esp_idf_wifi_event_loop_task", // task will be created
			.task_priority = uxTaskPriorityGet(NULL),
			.task_stack_size = wifi_captive_portal_esp_idf_wifi_task_stack_depth,
			.task_core_id = tskNO_AFFINITY};

	ESP_ERROR_CHECK(esp_event_loop_create(&wifi_captive_portal_esp_idf_wifi_event_loop_args, &wifi_captive_portal_esp_idf_wifi_event_loop_handle));
	ESP_ERROR_CHECK(esp_event_handler_instance_register_with(wifi_captive_portal_esp_idf_wifi_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_FINISH, wifi_captive_portal_esp_idf_wifi_finish_event_handler, wifi_captive_portal_esp_idf_wifi_event_loop_handle, NULL));
	ESP_ERROR_CHECK(esp_event_handler_instance_register_with(wifi_captive_portal_esp_idf_wifi_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_EVENT_STOPPED, wifi_captive_portal_esp_idf_wifi_stopped_event_handler, wifi_captive_portal_esp_idf_wifi_event_loop_handle, NULL));

	// esp_event_loop_args_t wifi_captive_portal_esp_idf_ota_event_loop_args = {
	// 		.queue_size = 5,
	// 		.task_name = "wifi_captive_portal_esp_idf_ota_event_loop_task", // task will be created
	// 		.task_priority = uxTaskPriorityGet(NULL),
	// 		.task_stack_size = wifi_captive_portal_esp_idf_ota_task_stack_depth,
	// 		.task_core_id = tskNO_AFFINITY};

	// ESP_ERROR_CHECK(esp_event_loop_create(&wifi_captive_portal_esp_idf_ota_event_loop_args, &wifi_captive_portal_esp_idf_ota_event_loop_handle));
	// ESP_ERROR_CHECK(esp_event_handler_instance_register_with(wifi_captive_portal_esp_idf_ota_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_OTA_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_OTA_EVENT_FINISH, wifi_captive_portal_esp_idf_ota_finish_event_handler, wifi_captive_portal_esp_idf_ota_event_loop_handle, NULL));

	// esp_event_loop_args_t wifi_captive_portal_esp_idf_http_event_loop_args = {
	// 		.queue_size = 5,
	// 		.task_name = "wifi_captive_portal_esp_idf_http_event_loop_task", // task will be created
	// 		.task_priority = uxTaskPriorityGet(NULL),
	// 		.task_stack_size = wifi_captive_portal_esp_idf_http_task_stack_depth,
	// 		.task_core_id = tskNO_AFFINITY};

	// ESP_ERROR_CHECK(esp_event_loop_create(&wifi_captive_portal_esp_idf_http_event_loop_args, &wifi_captive_portal_esp_idf_http_event_loop_handle));
	// ESP_ERROR_CHECK(esp_event_handler_instance_register_with(wifi_captive_portal_esp_idf_http_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTP_EVENT_FINISH, wifi_captive_portal_esp_idf_http_finish_event_handler, wifi_captive_portal_esp_idf_http_event_loop_handle, NULL));

	esp_event_loop_args_t wifi_captive_portal_esp_idf_httpd_event_loop_args = {
		.queue_size = 5,
		.task_name = "wifi_captive_portal_esp_idf_httpd_event_loop_task", // task will be created
		.task_priority = uxTaskPriorityGet(NULL),
		.task_stack_size = wifi_captive_portal_esp_idf_httpd_task_stack_depth,
		.task_core_id = tskNO_AFFINITY};

	ESP_ERROR_CHECK(esp_event_loop_create(&wifi_captive_portal_esp_idf_httpd_event_loop_args, &wifi_captive_portal_esp_idf_httpd_event_loop_handle));
	ESP_ERROR_CHECK(esp_event_handler_instance_register_with(wifi_captive_portal_esp_idf_httpd_event_loop_handle, WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT, WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT_FINISH, wifi_captive_portal_esp_idf_httpd_finish_event_handler, wifi_captive_portal_esp_idf_httpd_event_loop_handle, NULL));

	// // Initialize wifi and connect.
	// static const wifi_captive_portal_esp_idf_wifi_task_action_t wifi_task_action_connect = {
	// 	.id = WIFI_CAPTIVE_PORTAL_ESP_IDF_WIFI_TASK_ACTION_CONNECT,
	// 	.value = NULL,
	// };

	xTaskCreate(&wifi_captive_portal_esp_idf_wifi_task, wifi_captive_portal_esp_idf_wifi_task_name, wifi_captive_portal_esp_idf_wifi_task_stack_depth * 8, NULL, wifi_captive_portal_esp_idf_wifi_task_priority, NULL);
	// xTaskCreate(&wifi_captive_portal_esp_idf_wifi_task, wifi_captive_portal_esp_idf_wifi_task_name, wifi_captive_portal_esp_idf_wifi_task_stack_depth * 8, (void*)&wifi_task_action_connect, wifi_captive_portal_esp_idf_wifi_task_priority, NULL);
	ESP_LOGI(TAG, "Task started: %s", wifi_captive_portal_esp_idf_wifi_task_name);
}
