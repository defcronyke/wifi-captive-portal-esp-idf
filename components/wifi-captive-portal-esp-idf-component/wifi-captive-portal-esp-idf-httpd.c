/**	wifi-captive-portal-esp-idf-component

		Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

		This code is released under the license terms contained in the
		file named LICENSE, which is found in the top-level folder in
		this project. You must agree to follow those license terms,
		otherwise you aren't allowed to copy, distribute, or use any 
		part of this project in any way.

		Contains some modified example code from here:
		https://github.com/espressif/esp-idf/blob/release/v4.2/examples/protocols/openssl_server/main/openssl_server_example_main.c
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
// #include "esp_spiffs.h"
// #include "sdmmc_cmd.h"
// #if CONFIG_EXAMPLE_WEB_DEPLOY_SD
// #include "driver/sdmmc_host.h"
// #endif
#include "esp_vfs_fat.h"
#include "esp_vfs.h"
// #include "esp_flash_partitions.h"
// #include "esp_partition.h"
// #include "nvs.h"
// #include "nvs_flash.h"
// #if CONFIG_PROJECT_CONNECT_WIFI
#include "esp_wifi.h"
// #endif
// #include "openssl/ssl.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "esp_http_server.h"
// #include "esp_https_server.h"
// #include "cJSON.h"
#include "wifi-captive-portal-esp-idf-httpd.h"

static const char* HTTPD_TAG = "wifi-captive-portal-esp-idf-httpd";

esp_event_loop_handle_t wifi_captive_portal_esp_idf_httpd_event_loop_handle;

ESP_EVENT_DEFINE_BASE(WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_EVENT);

// static bool fs_initialized = false;

static const char* base_path = "/www";
// static const char* base_path = CONFIG_EXAMPLE_WEB_MOUNT_POINT;

/* // #define OPENSSL_EXAMPLE_SERVER_ACK "HTTP/1.1 200 OK\r\n" \
//                                 "Content-Type: text/html\r\n" \
//                                 "Content-Length: 106\r\n\r\n" \
//                                 "<html>\r\n" \
//                                 "<head>\r\n" \
//                                 "<title>OpenSSL example</title></head><body>\r\n" \
//                                 "OpenSSL server example!\r\n" \
//                                 "</body>\r\n" \
//                                 "</html>\r\n" \
//                                 "\r\n" */




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
// // #define SCRATCH_BUFSIZE (20240)
// // #define SCRATCH_BUFSIZE (10240)
#define SCRATCH_BUFSIZE (10240)

typedef struct rest_server_context
{
	char base_path[ESP_VFS_PATH_MAX + 1];
	char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

// #if CONFIG_EXAMPLE_WEB_DEPLOY_SEMIHOST
// esp_err_t init_fs(void)
// {
// 	if (fs_initialized) {
// 		ESP_LOGI(HTTPD_TAG, "fs is already initialized: no-op");
// 		return ESP_OK;
// 	}

// 	esp_err_t ret = esp_vfs_semihost_register(CONFIG_EXAMPLE_WEB_MOUNT_POINT, CONFIG_EXAMPLE_HOST_PATH_TO_MOUNT);
// 	if (ret != ESP_OK)
// 	{
// 		ESP_LOGE(HTTPD_TAG, "Failed to register semihost driver (%s)!", esp_err_to_name(ret));
// 		return ESP_FAIL;
// 	}

// 	fs_initialized = true;

// 	return ESP_OK;
// }
// #endif

// #if CONFIG_EXAMPLE_WEB_DEPLOY_SD
// esp_err_t init_fs(void)
// {
// 	if (fs_initialized) {
// 		ESP_LOGI(HTTPD_TAG, "fs is already initialized: no-op");
// 		return ESP_OK;
// 	}

// 	sdmmc_host_t host = SDMMC_HOST_DEFAULT();
// 	sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

// 	gpio_set_pull_mode(15, GPIO_PULLUP_ONLY); // CMD
// 	gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);  // D0
// 	gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);  // D1
// 	gpio_set_pull_mode(12, GPIO_PULLUP_ONLY); // D2
// 	gpio_set_pull_mode(13, GPIO_PULLUP_ONLY); // D3

// 	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
// 		.format_if_mount_failed = true,
// 		.max_files = 4,
// 		.allocation_unit_size = 16 * 1024};

// 	sdmmc_card_t *card;
// 	esp_err_t ret = esp_vfs_fat_sdmmc_mount(CONFIG_EXAMPLE_WEB_MOUNT_POINT, &host, &slot_config, &mount_config, &card);
// 	if (ret != ESP_OK)
// 	{
// 		if (ret == ESP_FAIL)
// 		{
// 			ESP_LOGE(HTTPD_TAG, "Failed to mount filesystem.");
// 		}
// 		else
// 		{
// 			ESP_LOGE(HTTPD_TAG, "Failed to initialize the card (%s)", esp_err_to_name(ret));
// 		}
// 		return ESP_FAIL;
// 	}
// 	/* print card info if mount successfully */
// 	sdmmc_card_print_info(stdout, card);

// 	fs_initialized = true;

// 	return ESP_OK;
// }
// #endif

// #if CONFIG_EXAMPLE_WEB_DEPLOY_SF
// esp_err_t init_fs(void)
// {
// 	if (fs_initialized) {
// 		ESP_LOGI(HTTPD_TAG, "fs is already initialized: no-op");
// 		return ESP_OK;
// 	}

// 	esp_vfs_spiffs_conf_t conf = {
// 		.base_path = CONFIG_EXAMPLE_WEB_MOUNT_POINT,
// 		.partition_label = NULL,
// 		// .max_files = 7,
// 		.max_files = 5,
// 		.format_if_mount_failed = false};
// 	esp_err_t ret = esp_vfs_spiffs_register(&conf);

// 	if (ret != ESP_OK)
// 	{
// 		if (ret == ESP_FAIL)
// 		{
// 			ESP_LOGE(HTTPD_TAG, "Failed to mount or format filesystem");
// 		}
// 		else if (ret == ESP_ERR_NOT_FOUND)
// 		{
// 			ESP_LOGE(HTTPD_TAG, "Failed to find SPIFFS partition");
// 		}
// 		else
// 		{
// 			ESP_LOGE(HTTPD_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
// 		}
// 		return ESP_FAIL;
// 	}

// 	size_t total = 0, used = 0;
// 	ret = esp_spiffs_info(NULL, &total, &used);
// 	if (ret != ESP_OK)
// 	{
// 		ESP_LOGE(HTTPD_TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
// 	}
// 	else
// 	{
// 		ESP_LOGI(HTTPD_TAG, "Partition size: total: %d, used: %d", total, used);
// 	}

// 	fs_initialized = true;

// 	return ESP_OK;
// }
// #endif

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
	const char *type = "text/plain";
	if (CHECK_FILE_EXTENSION(filepath, ".html"))
	{
		type = "text/html";
	}
	else if (CHECK_FILE_EXTENSION(filepath, ".js"))
	{
		type = "application/javascript";
	}
	else if (CHECK_FILE_EXTENSION(filepath, ".css"))
	{
		type = "text/css";
	}
	else if (CHECK_FILE_EXTENSION(filepath, ".png"))
	{
		type = "image/png";
	}
	else if (CHECK_FILE_EXTENSION(filepath, ".ico"))
	{
		type = "image/x-icon";
	}
	else if (CHECK_FILE_EXTENSION(filepath, ".svg"))
	{
		type = "text/xml";
	}
	return httpd_resp_set_type(req, type);
}

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
		// TODO: There is a value "CONFIG_LWIP_LOCAL_HOSTNAME" in the Kconfig 
		// menu which isn't being used here but it should be.
		httpd_resp_set_hdr(req, "Location", "http://epaper");

		httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

	} else {
		// const char resp[] = HTTPD_200;

		ESP_LOGI(HTTPD_TAG, "No redirect needed for HOST: %s", req_hdr_host_val);
		
		// httpd_resp_set_status(req, HTTPD_200);
		// httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
		
		// httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

		





		rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;

		char filepath[FILE_PATH_MAX];

		// Find the start of any query parameters and remove them.
		uint endpos = strlen(req->uri);

		char *s = NULL;
		s = strstr(req->uri, "?");

		if (s != NULL) {
			endpos = s - req->uri;
		}

		char req_uri[endpos + 1];
		strlcpy(req_uri, req->uri, sizeof(req_uri));

		strlcpy(filepath, rest_context->base_path, sizeof(filepath));
		if (req_uri[strlen(req_uri) - 1] == '/')
		{
			strlcat(filepath, "/index.html", sizeof(filepath));
		}
		else
		{
			strlcat(filepath, req_uri, sizeof(filepath));
		}

		int fd = open(filepath, O_RDONLY, 0);
		if (fd == -1)
		{
			ESP_LOGE(HTTPD_TAG, "Failed to open file : %s", filepath);

			memset((void *)filepath, 0, sizeof(filepath));

			strlcpy(filepath, rest_context->base_path, sizeof(filepath));

			strlcat(filepath, "/index.html", sizeof(filepath));

			fd = open(filepath, O_RDONLY, 0);
			if (fd == -1)
			{
				ESP_LOGE(HTTPD_TAG, "Failed to open file : %s", filepath);

				/* Respond with 500 Internal Server Error */
				httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
				return ESP_FAIL;
			}
		}

		set_content_type_from_file(req, filepath);

		char *chunk = rest_context->scratch;
		ssize_t read_bytes;
		do
		{
			/* Read file in chunks into the scratch buffer */
			read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
			if (read_bytes == -1)
			{
				ESP_LOGE(HTTPD_TAG, "Failed to read file : %s", filepath);
			}
			else if (read_bytes > 0)
			{
				/* Send the buffer contents as HTTP response chunk */
				if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK)
				{
					close(fd);
					ESP_LOGE(HTTPD_TAG, "File sending failed!");
					/* Abort sending file */
					httpd_resp_sendstr_chunk(req, NULL);
					/* Respond with 500 Internal Server Error */
					httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
					return ESP_FAIL;
				}
			}
		} while (read_bytes > 0);
		/* Close file after sending complete */
		close(fd);
		ESP_LOGI(HTTPD_TAG, "File sending complete");
		/* Respond with an empty chunk to signal HTTP response completion */
		httpd_resp_send_chunk(req, NULL, 0);
	}


	// rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;

	// char filepath[FILE_PATH_MAX];

	// // Find the start of any query parameters and remove them.
	// uint endpos = strlen(req->uri);

	// char *s = NULL;
	// s = strstr(req->uri, "?");

	// if (s != NULL) {
	// 	endpos = s - req->uri;
	// }

	// char req_uri[endpos + 1];
	// strlcpy(req_uri, req->uri, sizeof(req_uri));

	// strlcpy(filepath, rest_context->base_path, sizeof(filepath));
	// if (req_uri[strlen(req_uri) - 1] == '/')
	// {
	// 	strlcat(filepath, "/index.html", sizeof(filepath));
	// }
	// else
	// {
	// 	strlcat(filepath, req_uri, sizeof(filepath));
	// }

	// int fd = open(filepath, O_RDONLY, 0);
	// if (fd == -1)
	// {
	// 	ESP_LOGE(HTTPD_TAG, "Failed to open file : %s", filepath);

	// 	memset((void *)filepath, 0, sizeof(filepath));

	// 	strlcpy(filepath, rest_context->base_path, sizeof(filepath));

	// 	strlcat(filepath, "/index.html", sizeof(filepath));

	// 	fd = open(filepath, O_RDONLY, 0);
	// 	if (fd == -1)
	// 	{
	// 		ESP_LOGE(HTTPD_TAG, "Failed to open file : %s", filepath);

	// 		/* Respond with 500 Internal Server Error */
	// 		httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
	// 		return ESP_FAIL;
	// 	}
	// }

	// set_content_type_from_file(req, filepath);

	// char *chunk = rest_context->scratch;
	// ssize_t read_bytes;
	// do
	// {
	// 	/* Read file in chunks into the scratch buffer */
	// 	read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
	// 	if (read_bytes == -1)
	// 	{
	// 		ESP_LOGE(HTTPD_TAG, "Failed to read file : %s", filepath);
	// 	}
	// 	else if (read_bytes > 0)
	// 	{
	// 		/* Send the buffer contents as HTTP response chunk */
	// 		if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK)
	// 		{
	// 			close(fd);
	// 			ESP_LOGE(HTTPD_TAG, "File sending failed!");
	// 			/* Abort sending file */
	// 			httpd_resp_sendstr_chunk(req, NULL);
	// 			/* Respond with 500 Internal Server Error */
	// 			httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
	// 			return ESP_FAIL;
	// 		}
	// 	}
	// } while (read_bytes > 0);
	// /* Close file after sending complete */
	// close(fd);
	// ESP_LOGI(HTTPD_TAG, "File sending complete");
	// /* Respond with an empty chunk to signal HTTP response completion */
	// httpd_resp_send_chunk(req, NULL, 0);
	return ESP_OK;
}

// /* System info GET handler */
// static esp_err_t system_info_get_handler(httpd_req_t *req)
// {
// 	httpd_resp_set_type(req, "application/json");
// 	cJSON *root = cJSON_CreateObject();
// 	esp_chip_info_t chip_info;
// 	esp_chip_info(&chip_info);
// 	cJSON_AddStringToObject(root, "version", IDF_VER);
// 	cJSON_AddNumberToObject(root, "cores", chip_info.cores);
// 	const char *sys_info = cJSON_Print(root);

// 	// httpd_resp_send(req, sys_info, -1);
// 	httpd_resp_sendstr(req, sys_info);

// 	free((void *)sys_info);
// 	cJSON_Delete(root);
// 	return ESP_OK;
// }

// static esp_err_t util_restart_post_handler(httpd_req_t *req) {
// 	httpd_resp_set_type(req, "application/json");

// 	cJSON *root = cJSON_CreateObject();

// 	cJSON_AddStringToObject(root, "msg", "success");
// 	cJSON_AddNumberToObject(root, "statusCode", 200);

// 	const char *sys_info = cJSON_Print(root);
	
// 	// httpd_resp_send(req, sys_info, -1);
// 	httpd_resp_sendstr(req, sys_info);

// 	free((void *)sys_info);
// 	cJSON_Delete(root);

// 	esp_restart();

// 	return ESP_OK;
// }

// static esp_err_t start_httpd(const char *base_path)
static void start_httpd(void* pvParameter)
{
	// ESP_ERROR_CHECK(init_fs());



// 	int ret;

//     SSL_CTX *ctx;
//     SSL *ssl;

//     int sockfd, new_sockfd;
//     socklen_t addr_len;
//     struct sockaddr_in sock_addr;

//     char recv_buf[OPENSSL_EXAMPLE_RECV_BUF_LEN];

//     const char send_data[] = OPENSSL_EXAMPLE_SERVER_ACK;
//     const int send_bytes = sizeof(send_data);

//     // extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
//     // extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");

// 		extern const unsigned char cacert_pem_start[] asm("_binary_ca_cert_conf_pem_start");
//     extern const unsigned char cacert_pem_end[]   asm("_binary_ca_cert_conf_pem_end");
//     const unsigned int cacert_pem_bytes = cacert_pem_end - cacert_pem_start;

//     // extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
//     // extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");

// 		extern const unsigned char prvtkey_pem_start[] asm("_binary_ca_key_conf_pem_start");
//     extern const unsigned char prvtkey_pem_end[]   asm("_binary_ca_key_conf_pem_end");
//     const unsigned int prvtkey_pem_bytes = prvtkey_pem_end - prvtkey_pem_start;   

//     ESP_LOGI(HTTPD_TAG, "SSL server context create ......");
//     /* For security reasons, it is best if you can use
//        TLSv1_2_server_method() here instead of TLS_server_method().
//        However some old browsers may not support TLS v1.2.
//     */
//     ctx = SSL_CTX_new(TLS_server_method());
//     if (!ctx) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed1;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server context set own certification......");
//     ret = SSL_CTX_use_certificate_ASN1(ctx, cacert_pem_bytes, cacert_pem_start);
//     if (!ret) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed2;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server context set private key......");
//     ret = SSL_CTX_use_PrivateKey_ASN1(0, ctx, prvtkey_pem_start, prvtkey_pem_bytes);
//     if (!ret) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed2;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server create socket ......");
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed2;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server socket bind ......");
//     memset(&sock_addr, 0, sizeof(sock_addr));
//     sock_addr.sin_family = AF_INET;
//     sock_addr.sin_addr.s_addr = 0;
//     sock_addr.sin_port = htons(OPENSSL_EXAMPLE_LOCAL_TCP_PORT);
//     ret = bind(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
//     if (ret) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed3;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server socket listen ......");
//     ret = listen(sockfd, 32);
//     if (ret) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed3;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

// reconnect:
//     ESP_LOGI(HTTPD_TAG, "SSL server create ......");
//     ssl = SSL_new(ctx);
//     if (!ssl) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed3;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server socket accept client ......");
//     new_sockfd = accept(sockfd, (struct sockaddr *)&sock_addr, &addr_len);
//     if (new_sockfd < 0) {
//         ESP_LOGI(HTTPD_TAG, "failed" );
//         goto failed4;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     SSL_set_fd(ssl, new_sockfd);

//     ESP_LOGI(HTTPD_TAG, "SSL server accept client ......");
//     ret = SSL_accept(ssl);
//     if (!ret) {
//         ESP_LOGI(HTTPD_TAG, "failed");
//         goto failed5;
//     }
//     ESP_LOGI(HTTPD_TAG, "OK");

//     ESP_LOGI(HTTPD_TAG, "SSL server read message ......");
//     do {
//         memset(recv_buf, 0, OPENSSL_EXAMPLE_RECV_BUF_LEN);
//         ret = SSL_read(ssl, recv_buf, OPENSSL_EXAMPLE_RECV_BUF_LEN - 1);
//         if (ret <= 0) {
//             break;
//         }
//         ESP_LOGI(HTTPD_TAG, "SSL read: %s", recv_buf);
//         if (strstr(recv_buf, "GET ") &&
//             strstr(recv_buf, " HTTP/1.1")) {
//             ESP_LOGI(HTTPD_TAG, "SSL get matched message");
//             ESP_LOGI(HTTPD_TAG, "SSL write message");
//             ret = SSL_write(ssl, send_data, send_bytes);
//             if (ret > 0) {
//                 ESP_LOGI(HTTPD_TAG, "OK");
//             } else {
//                 ESP_LOGI(HTTPD_TAG, "error");
//             }
//             break;
//         }
//     } while (1);
    
//     SSL_shutdown(ssl);
// failed5:
//     close(new_sockfd);
//     new_sockfd = -1;
// failed4:
//     SSL_free(ssl);
//     ssl = NULL;
//     goto reconnect;
// failed3:
//     close(sockfd);
//     sockfd = -1;
// failed2:
//     SSL_CTX_free(ctx);
//     ctx = NULL;
// failed1:
//     vTaskDelete(NULL);
//     return ;



	// HTTP
	ESP_LOGI(HTTPD_TAG, "Starting HTTP Server...");

	// const char base_path[] = CONFIG_EXAMPLE_WEB_MOUNT_POINT;
	// const char base_path[] = "/www";

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

	// /* URI handler for getting system info */
	// httpd_uri_t system_info_get_uri = {
	// 	.uri = "/api/v1/system/info",
	// 	.method = HTTP_GET,
	// 	.handler = system_info_get_handler,
	// 	.user_ctx = rest_context};
	// httpd_register_uri_handler(server, &system_info_get_uri);

	// /* URI handler for restarting the device */
	// httpd_uri_t util_restart_post_uri = {
	// 	.uri = "/api/util/restart",
	// 	.method = HTTP_POST,
	// 	.handler = util_restart_post_handler,
	// 	.user_ctx = rest_context};
	// httpd_register_uri_handler(server, &util_restart_post_uri);	

	/* URI handler for getting web server files */
	httpd_uri_t common_get_uri = {
		.uri = "/*",
		.method = HTTP_GET,
		.handler = rest_common_get_handler,
		.user_ctx = rest_context};
	httpd_register_uri_handler(server, &common_get_uri);

	ESP_LOGI(HTTPD_TAG, "Registered HTTP server URI handlers.");


	// ESP_LOGI(HTTPD_TAG, "Registering HTTP URI handlers.");
	
	// httpd_register_uri_handler(server, &system_info_get_uri);
	// httpd_register_uri_handler(server, &util_restart_post_uri);
	// httpd_register_uri_handler(server, &common_get_uri);


	// HTTPS
	// ESP_LOGI(HTTPD_TAG, "Starting HTTPS Server...");

	// httpd_handle_t server_s = NULL;
	// httpd_ssl_config_t config_s = HTTPD_SSL_CONFIG_DEFAULT();
	// config_s.httpd.uri_match_fn = httpd_uri_match_wildcard;
	// // config_s.httpd.max_open_sockets = CONFIG_LWIP_MAX_SOCKETS - 3;
	// // config_s.httpd.max_open_sockets = 1;
	// // config_s.transport_mode = HTTPD_SSL_TRANSPORT_INSECURE;

	// extern const unsigned char ca_cert_conf_pem_start[] asm("_binary_ca_cert_conf_pem_start");
	// extern const unsigned char ca_cert_conf_pem_end[]   asm("_binary_ca_cert_conf_pem_end");
	// config_s.cacert_pem = ca_cert_conf_pem_start;
	// config_s.cacert_len = ca_cert_conf_pem_end - ca_cert_conf_pem_start;

	// // config_s.client_verify_cert_pem = config_s.cacert_pem;
	// // config_s.client_verify_cert_len = config_s.cacert_len;
	
	// // extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
	// // extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
	// // config_s.cacert_pem = cacert_pem_start;
	// // config_s.cacert_len = cacert_pem_end - cacert_pem_start;

	// extern const unsigned char ca_key_conf_pem_start[] asm("_binary_ca_key_conf_pem_start");
	// extern const unsigned char ca_key_conf_pem_end[]   asm("_binary_ca_key_conf_pem_end");
	// config_s.prvtkey_pem = ca_key_conf_pem_start;
	// config_s.prvtkey_len = ca_key_conf_pem_end - ca_key_conf_pem_start;

	// // extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
	// // extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
	// // config_s.prvtkey_pem = prvtkey_pem_start;
	// // config_s.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

	// // REST_CHECK(httpd_ssl_start(&server_s, &config_s) == ESP_OK, "Start HTTPS server failed", err_start);

	// // esp_err_t res = httpd_ssl_start(&server_s, &config_s);
	// // if (res != ESP_OK) {
	// // 		ESP_LOGE(HTTPD_TAG, "Error starting HTTPS server: %d", res);
	// // 		return ESP_OK;
	// // }

// 	ESP_LOGI(HTTPD_TAG, "Registering HTTPS URI handlers.");

// 	/* URI handler for getting system info */
// 	httpd_uri_t system_info_get_uri = {
// 		.uri = "/api/v1/system/info",
// 		.method = HTTP_GET,
// 		.handler = system_info_get_handler,
// 		.user_ctx = rest_context};
// 	// httpd_register_uri_handler(server_s, &system_info_get_uri);

// 	/* URI handler for restarting the device */
// 	httpd_uri_t util_restart_post_uri = {
// 		.uri = "/api/util/restart",
// 		.method = HTTP_POST,
// 		.handler = util_restart_post_handler,
// 		.user_ctx = rest_context};
// 	// httpd_register_uri_handler(server_s, &util_restart_post_uri);	

// 	/* URI handler for getting web server files */
// 	httpd_uri_t common_get_uri = {
// 		.uri = "/*",
// 		.method = HTTP_GET,
// 		.handler = rest_common_get_handler,
// 		.user_ctx = rest_context};
// 	// httpd_register_uri_handler(server_s, &common_get_uri);

// 	// REST_CHECK(httpd_ssl_start(&server_s, &config_s) == ESP_OK, "Start HTTPS server failed", err_start);

// 	ESP_LOGI(HTTPD_TAG, "Started HTTPS Server.");

	// return ESP_OK;
	return;
err_start:
	free(rest_context);
err:
	return;
// 	return ESP_FAIL;
}

void wifi_captive_portal_esp_idf_httpd_task(void* pvParameter) {
	while (1)
	{
		/** Inner scope to ensure all destructors are called before vTaskDelete. */
		{
			// if (pvParameter != NULL) {
			// 	httpd_task_action = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_COPY(pvParameter);
			// }

			// if (httpd_task_action.value != NULL) {
			// 	httpd_task_action_value = WIFI_CAPTIVE_PORTAL_ESP_IDF_HTTPD_TASK_ACTION_VALUE_COPY(httpd_task_action.value);
			// }

			// start_httpd(CONFIG_EXAMPLE_WEB_MOUNT_POINT);

			start_httpd(NULL);

			while (1) {
				vTaskDelay(1000 / portTICK_PERIOD_MS);
			}

			// uint32_t val = 0;

			// xTaskNotifyWait(0, ULONG_MAX, &val, portMAX_DELAY);
		}

		vTaskDelete(NULL);
	}
}
