/**	wifi-captive-portal-esp-idf-component

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.

		Contains some modified example code from here:
		https://github.com/espressif/esp-idf/blob/release/v4.2/examples/protocols/http_server/simple/main/main.c
		https://github.com/espressif/esp-idf/blob/release/v4.2/examples/protocols/http_server/restful_server/main/rest_server.c

		Original Example Code Header:
		This example code is in the Public Domain (or CC0 licensed, at your option.)

		Unless required by applicable law or agreed to in writing, this
		software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
		CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <fcntl.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs_semihost.h"
#include "esp_vfs_fat.h"
#include "esp_vfs.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "esp_http_server.h"
#include "wifi-captive-portal-esp-idf-httpd.h"

#define REST_CHECK(a, str, goto_tag, ...)	\
do                                      \
{                                       \
	if (!(a))                             \
	{                                     \
		ESP_LOGE(HTTPD_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__);	\
		goto goto_tag;	\
	}	\
} while (0)

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

static const char* HTTPD_TAG = "wifi-captive-portal-esp-idf-httpd";

esp_event_loop_handle_t wifi_captive_portal_esp_idf_httpd_event_loop_handle;

ESP_EVENT_DEFINE_BASE(WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT);

static const char* base_path = "/www";

typedef struct rest_server_context
{
	char base_path[ESP_VFS_PATH_MAX + 1];
	char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Send HTTP response with the contents of the requested file */
static esp_err_t rest_common_get_handler(httpd_req_t *req)
{
	size_t req_hdr_host_len = httpd_req_get_hdr_value_len(req, "Host");

	char req_hdr_host_val[req_hdr_host_len + 1];

	esp_err_t res = httpd_req_get_hdr_value_str(req, "Host", (char*)&req_hdr_host_val, sizeof(char) * req_hdr_host_len + 1);
	if (res != ESP_OK) {
		ESP_LOGE(HTTPD_TAG, "failed getting HOST header value: %d", res);

		switch (res)
		{
		case ESP_ERR_NOT_FOUND:
			ESP_LOGE(HTTPD_TAG, "failed getting HOST header value: ESP_ERR_NOT_FOUND: Key not found: %d", res);
			break;

		case ESP_ERR_INVALID_ARG:
			ESP_LOGE(HTTPD_TAG, "failed getting HOST header value: ESP_ERR_INVALID_ARG: Null arguments: %d", res);
			break;

		case ESP_ERR_HTTPD_INVALID_REQ:
			ESP_LOGE(HTTPD_TAG, "failed getting HOST header value: ESP_ERR_HTTPD_INVALID_REQ: Invalid HTTP request pointer: %d", res);
			break;

		case ESP_ERR_HTTPD_RESULT_TRUNC:
			ESP_LOGE(HTTPD_TAG, "failed getting HOST header value: ESP_ERR_HTTPD_RESULT_TRUNC: Value string truncated: %d", res);
			break;
		
		default:
			break;
		}
	}

	ESP_LOGI(HTTPD_TAG, "Got HOST header value: %s", req_hdr_host_val);

	const char redir_trigger_host[] = "connectivitycheck.gstatic.com";

	if (strncmp(req_hdr_host_val, redir_trigger_host, strlen(redir_trigger_host)) == 0) {
		const char resp[] = "302 Found";
		
		ESP_LOGI(HTTPD_TAG, "Detected redirect trigger HOST: %s", redir_trigger_host);
		
		httpd_resp_set_status(req, resp);

		/** NOTE: This is where you redirect to whatever DNS address you prefer to open the
				captive portal page. This DNS address will be displayed at the top of the
				page, so maybe you want to choose a nice name to use (it can be any legal
				DNS name that you prefer. */
		httpd_resp_set_hdr(req, "Location", "http://wifi-captive-portal");

		httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

	} else {
		ESP_LOGI(HTTPD_TAG, "No redirect needed for HOST: %s", req_hdr_host_val);

		char*  buf;
    size_t buf_len;

    /*	Get header value string length and allocate memory for length + 1,
    		extra byte for null termination */
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(HTTPD_TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-2") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-2", buf, buf_len) == ESP_OK) {
            ESP_LOGI(HTTPD_TAG, "Found header => Test-Header-2: %s", buf);
        }
        free(buf);
    }

    buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
            ESP_LOGI(HTTPD_TAG, "Found header => Test-Header-1: %s", buf);
        }
        free(buf);
    }

    /*	Read URL query string length and allocate memory for length + 1,
    		extra byte for null termination */
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(HTTPD_TAG, "Found URL query => %s", buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(HTTPD_TAG, "Found URL query parameter => query1=%s", param);
            }
            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(HTTPD_TAG, "Found URL query parameter => query3=%s", param);
            }
            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(HTTPD_TAG, "Found URL query parameter => query2=%s", param);
            }
        }
        free(buf);
    }

    /* Set some custom headers */
    httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    /*	Send response with custom headers and body set as the
    		string passed in user context */
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, strlen(resp_str));

    /*	After sending the HTTP response the old HTTP request
    		headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(HTTPD_TAG, "Request headers lost");
    }
	}

	return ESP_OK;
}

static void start_httpd(void* pvParameter)
{
	/** HTTP server */
	ESP_LOGI(HTTPD_TAG, "Starting HTTP Server...");

	REST_CHECK(base_path, "wrong base path", err);
	rest_server_context_t *rest_context = calloc(1, sizeof(rest_server_context_t));
	REST_CHECK(rest_context, "No memory for rest context", err);
	strlcpy(rest_context->base_path, base_path, sizeof(rest_context->base_path));

	httpd_handle_t server = NULL;
	
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.uri_match_fn = httpd_uri_match_wildcard;
	config.lru_purge_enable = true;

	REST_CHECK(httpd_start(&server, &config) == ESP_OK, "Start HTTP server failed", err_start);
	ESP_LOGI(HTTPD_TAG, "Started HTTP Server.");

	ESP_LOGI(HTTPD_TAG, "Registering HTTP server URI handlers...");

	/** URI handler */
	httpd_uri_t common_get_uri = {
		.uri = "/*",
		.method = HTTP_GET,
		.handler = rest_common_get_handler,
		.user_ctx = "Hello World!"};

	httpd_register_uri_handler(server, &common_get_uri);

	ESP_LOGI(HTTPD_TAG, "Registered HTTP server URI handlers.");

	return;

err_start:
	free(rest_context);

err:
	return;
}

void wifi_captive_portal_esp_idf_httpd_task(void* pvParameter) {
	while (1)
	{

		start_httpd(NULL);

		/** TODO: xEventGroupWaitBits or similar might be much better than vTaskDelay for this section. */
		while (1) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}

		vTaskDelete(NULL);
	}
}
