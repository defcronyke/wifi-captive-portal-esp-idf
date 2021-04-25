#ifndef __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_H_INCLUDED__
#define __WIFI_CAPTIVE_PORTAL_ESP_IDF_COMPONENT_WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_H_INCLUDED__
/**	wifi-captive-portal-esp-idf-component

  Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>

  This code is released under the license terms contained in the
  file named LICENSE, which is found in the top-level folder in
  this project. You must agree to follow those license terms,
  otherwise you aren't allowed to copy, distribute, or use any
  part of this project in any way.

  Contains some modified example code from here:
  https://github.com/cornelis-61/esp32_Captdns/blob/master/main/captdns.c

  Original Example Code Header:
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain
* this notice you can do whatever you want with this stuff. If we meet some day,
* and you think this stuff is worth it, you can buy me a beer in return.
*
* modified for ESP32 by Cornelis
*
* ----------------------------------------------------------------------------
*/
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_system.h"

#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_LEN 512

#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_FLAG_QR (1 << 7)
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_FLAG_AA (1 << 2)
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_FLAG_TC (1 << 1)
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_FLAG_RD (1 << 0)

#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_A 1
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_NS 2
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_CNAME 5
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_SOA 6
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_WKS 11
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_PTR 12
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_HINFO 13
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_MINFO 14
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_MX 15
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_TXT 16
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QTYPE_URI 256

#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QCLASS_IN 1
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QCLASS_ANY 255
#define WIFI_CAPTIVE_PORTAL_ESP_IDF_DNS_QCLASS_URI 256

typedef struct __attribute__((packed))
{
  uint16_t id;
  uint8_t flags;
  uint8_t rcode;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} DnsHeader;

typedef struct __attribute__((packed))
{
  uint8_t len;
  uint8_t data;
} DnsLabel;

typedef struct __attribute__((packed))
{
  uint16_t type;
  uint16_t cl;
} DnsQuestionFooter;

typedef struct __attribute__((packed))
{
  uint16_t type;
  uint16_t cl;
  uint32_t ttl;
  uint16_t rdlength;
} DnsResourceFooter;

typedef struct __attribute__((packed))
{
  uint16_t prio;
  uint16_t weight;
} DnsUriHdr;

#ifdef __cplusplus
extern "C"
{
#endif

  void wifi_captive_portal_esp_idf_dns_init(void);

#ifdef __cplusplus
}
#endif

#endif
