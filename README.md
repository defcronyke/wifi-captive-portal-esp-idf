# wifi-captive-portal-esp-idf

An ESP-IDF firmware project for a wifi access point captive portal.

This depends on the `release/v4.2` branch of Espressif's ESP-IDF for
ESP32 microcontrollers, so you'll need to use that version (which was
the current stable version when this project was made, and still might
be).

Note that their `v4.2 git release tag` of ESP-IDF has bugs, so you
need to use their `release/v4.2 git release branch` instead.

## Access Point Login Details for Captive Portal

WiFi AP SSID: `wifi-net-84541`

WiFi AP Password: `DHJSd 86 fUIxdf`

_You can change those details in the Kconfig menu by running `idf.py menuconfig`,
and navigating to the `[<>] Project connection config` section._
