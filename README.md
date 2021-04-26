# Wifi Captive Portal ESP-IDF

## An ESP-IDF firmware project for a wifi access point captive portal

[![pipeline status master branch](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/badges/master/pipeline.svg)](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/-/pipelines) [![sponsor the project](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&link=https://github.com/sponsors/defcronyke)](https://github.com/sponsors/defcronyke)

---

## License

[Copyright © 2021](https://defcronyke.gitlab.io/epaper-idf/jeremy-profile-paint-bw.png) [Jeremy Carter](https://eternalvoid.net) `<`[jeremy@jeremycarter.ca](mailto:Jeremy%20Carter%20<jeremy@jeremycarter.ca>?subject=wifi-captive-portal-esp-idf)`>`

This project is primarily released under the terms of the license contained in the file named [`LICENSE`](https://gitlab.com/defcronyke/wifi-captive-portal-esp-idf/-/blob/master/LICENSE), which can be found in the top-level folder of this project. It also uses a bit of 3rd-party code, which is in turn primarily licensed under whichever licenses are referenced in each header (.h) or source (.c, .cpp, etc.) file, as per the original authors' preferences. A possibly non-exhaustive set of these other licenses is included in the top-level folder of this project in the files with names beginning with "`LICENSE-`".

---

## Details

This is tested and working on Android at least as recently as `2021-04-25`. It will cause Android devices to prompt the user to "`sign-in to the access point`" as soon as they connect on Wifi when it's working properly, so if you notice it's not doing that, please file a bug report so it can maybe be solved.

I don't have other brands of devices, so support for others isn't planned. The goal of this project is to provide a fairly minimal working wifi access point captive portal ESP-IDF firmware and component project for ESP32 microcontroller devices, which you can clone and use in your own projects to add this feature to your own firmware projects easily.

If you find any bugs, please file a bug report so I can try to improve it.

## Usage

- To install the pre-built version onto your USB/serial-connected ESP32 device, enter the following commands in a Linux terminal:

  ```shell
  # First install "python", "pip", and Espressif's firmware
  # flashing tool "esptool.py" if you don't have those:
  pip install esptool

  # (Optional) If the above command doesn't work, try this
  # one instead:
  python -m pip install esptool

  # Install the pre-built firmware of this project directly
  # from GitLab's CI/CD artifacts archive. The arguments are
  # optional and the defaults are as below:
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

## How it works

### (requirements to make the sign-in prompt come up on Android as of 2021-04-26)

1. The ESP32 AP needs its `IP address` set as a `class A public internet address`, which is some address on `subnet 255.0.0.0` (probably any fake IP unless you want to use one you actually own). Its DHCP server will then give out addresses to connected clients starting at +1 from that address.
1. The AP needs to run a `DNS server` which is configured to resolve every requested `hostname` from the client to its own `IP address`.
1. The AP needs to run an `HTTP server` (HTTPS don't work well for this because `HSTS` breaks its usefulness).
1. The `HTTP server` needs to check the "`Host`" header on all client requests, and if `Host` is "`connectivitycheck.gstatic.com`", the server needs to respond with a `redirect "302 Found" status` (which needs a non-empty body, so maybe you put something like "`302 Found`"). The `"Location" header` on that response needs to be an `absolute URL` starting with "`http://`", followed by any `hostname` your prefer. The `hostname` you choose will show at the top of the `captive portal` page, so you might want to use one which would look nice there.
1. The `HTTP server` needs to respond to whatever `hostname` you used in the above `redirect` with a `"200 OK" status response`, and its `body` needs to contain whatever page content you'd like to be displayed to the user inside the page which comes up when they tap the `"sign-in to access point" prompt` on `Android`.
1. The `HTTP server` needs to be responding to all other `requests` that have any different `"Host" header value`, with a `"200 OK" status response`.
1. (Optional) If you want some kind of `auth flow` for the `captive portal`, involving some kind of additional user login or payment or anything like that to be required before the user is allowed to use the `access point`, you can signal to the `Android` device that the user has successfully completed the `login requirement` by switching the `HTTP server's` behaviour so it responds to `any future requests by that user` to "`connectivitycheck.gstatic.com`", with a `"200 OK" status response`.

   If you implement all those things (or use/build on this solution), you should get a `Wifi Captive Portal` which is working for `Android`.

   If anyone wants to tell me `how to modify it so it works on iPhones` as well, I'll happily add the code for it, but I have no way of testing if it works, so you might need to test it for me afterwards to see if I did it correctly.

## Acknowledgements

The DNS-related code in this project was borrowed and improved from this other project by [`github.com/cornelis-61`](https://github.com/cornelis-61), so thanks a lot to the author of that one! - [`https://github.com/cornelis-61/esp32_Captdns`](https://github.com/cornelis-61/esp32_Captdns)
