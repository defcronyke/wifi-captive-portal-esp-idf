# Wifi Captive Portal ESP-IDF

## An ESP-IDF firmware project for a wifi access point captive portal

[![pipeline status master branch](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/badges/master/pipeline.svg)](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/-/pipelines) [![sponsor the project](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&link=https://github.com/sponsors/defcronyke)](https://github.com/sponsors/defcronyke)

---

## License

[Copyright © 2021](https://defcronyke.gitlab.io/epaper-idf/jeremy-profile-paint-bw.png) [Jeremy Carter](https://eternalvoid.net) `<`[jeremy@jeremycarter.ca](mailto:Jeremy%20Carter%20<jeremy@jeremycarter.ca>?subject=wifi-captive-portal-esp-idf)`>`

This project is primarily released under the terms of the license contained in the file named [`LICENSE`](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/-/blob/master/LICENSE), which can be found in the top-level folder of this project. It also uses a bit of 3rd-party code, which is in turn primarily licensed under whichever licenses are referenced in each header (.h) or source (.c, .cpp, etc.) file, as per the original authors' preferences. A possibly non-exhaustive set of these other licenses is included in the top-level folder of this project in the files with names beginning with "`LICENSE-`".

---

## Details

This depends on the [`"release/v4.2" branch of Espressif's ESP-IDF`](https://github.com/espressif/esp-idf/tree/release/v4.2) for ESP32 microcontrollers, so you'll need to use that version (which was [`the current stable version`](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/) when this project was made, and it still might be). Feel free to file a bug report if you notice that Espressif releases a new stable version of IDF, and I'll happily update this so it works properly with it.

Note that their `v4.2 git release tag` of ESP-IDF has bugs, so you need to use their [`release/v4.2 git release branch`](https://github.com/espressif/esp-idf/tree/release/v4.2) instead, which at the time of writing, was not mentioned in their stable version documentation.

## Access Point Login Details for Captive Portal

WiFi AP SSID: `wifi-net-84541`

WiFi AP Password: `DHJSd 86 fUIxdf`

_You can change those details in the Kconfig menu by running `idf.py menuconfig`,
and navigating to the `[<>] Project connection config` section._
