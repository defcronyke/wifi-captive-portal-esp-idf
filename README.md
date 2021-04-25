# Wifi Captive Portal ESP-IDF

## An ESP-IDF firmware project for a wifi access point captive portal

[![pipeline status master branch](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/badges/master/pipeline.svg)](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/-/pipelines) [![sponsor the project](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&link=https://github.com/sponsors/defcronyke)](https://github.com/sponsors/defcronyke)

---

## License

[Copyright © 2021](https://defcronyke.gitlab.io/epaper-idf/jeremy-profile-paint-bw.png) [Jeremy Carter](https://eternalvoid.net) `<`[jeremy@jeremycarter.ca](mailto:Jeremy%20Carter%20<jeremy@jeremycarter.ca>?subject=wifi-captive-portal-esp-idf)`>`

This project is primarily released under the terms of the license contained in the file named [`LICENSE`](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/-/blob/master/LICENSE), which can be found in the top-level folder of this project. It also uses a bit of 3rd-party code, which is in turn primarily licensed under whichever licenses are referenced in each header (.h) or source (.c, .cpp, etc.) file, as per the original authors' preferences. A possibly non-exhaustive set of these other licenses is included in the top-level folder of this project in the files with names beginning with "`LICENSE-`".

---

## Details

This is tested and working on Android at least as recently as `2021-04-25`. It will cause Android devices to prompt the user to "`sign-in to the access point`" as soon as they connect on Wifi when it's working properly, so if you notice it's not doing that, please file a bug report so it can maybe be solved. The author doesn't have other brand devices, so support for others isn't planned.

## Usage

- To install the pre-built version onto your USB/serial-connected ESP32 device, enter the following commands in a Linux terminal:

  ```shell
  # First install "python", "pip", and Espressif's firmware
  # flashing tool "esptool.py" if you don't have those:
  pip install esptool

  # (Optional) If the above command doesn't work, try this
  # one instead:
  python -m pip install esptool

  # Installs directly from GitLab's CI/CD artifacts archive.
  # The arguments are optional and the defaults are as below:
  # (git branch: master, target serial device: /dev/ttyUSB0,
  # flashing baud rate: 115200, firmware size: 4MB)
  bash <(curl -sL https://tinyurl.com/wifi-captive-portal-install) master /dev/ttyUSB0 115200 4MB
  ```

## Access Point Login Details for Captive Portal

WiFi AP SSID: `wifi-net-84541`

WiFi AP Password: `DHJSd 86 fUIxdf`

_You can change those details in the Kconfig menu if you have ESP-IDF installed, by running `idf.py menuconfig` and navigating to the `[<>] Project connection config` section._

_When you're ready to install your modified version, use the command `idf.py flash monitor`_

## Important

This depends on the [`"release/v4.2" branch of Espressif's ESP-IDF`](https://github.com/espressif/esp-idf/tree/release/v4.2) for ESP32 microcontrollers, so you'll need to use that version if you want to customize the firmware (which was [`the current stable version`](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/) when this project was made, and it still might be). Feel free to file a bug report if you notice that Espressif releases a new stable version of IDF, and I'll happily update this so it works properly with it.

Note that their `v4.2 git release tag` of ESP-IDF has bugs, so you need to use their [`release/v4.2 git release branch`](https://github.com/espressif/esp-idf/tree/release/v4.2) instead, which at the time of writing was not mentioned in their stable version documentation.
