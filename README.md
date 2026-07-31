# ESP32-C3 HomeKit Smart Plug

A standalone WiFi smart plug built around the ESP32-C3 Super Mini, exposing native Apple HomeKit control via [esp-homekit-sdk](https://github.com/espressif/esp-homekit-sdk).

## Overview

The ESP32-C3 connects directly to the home WiFi network and implements the HomeKit Accessory Protocol (HAP) on-device using Espressif's open-source `esp-homekit-sdk`. It advertises itself as a HomeKit accessory and pairs directly with the Apple Home app. A relay switches mains power to a connected load (tested with a small AC fan), with a local status LED and a manual push-button override.

## Hardware

- **MCU:** ESP32-C3 Super Mini (WiFi + BLE)
- **Relay:** Songle SRD-05VDC-SL-C
- **Relay drive:** 2N3904 NPN, low-side switched, 1k base resistor, 1N4007 flyback diode across the coil
- **Mains supply:** Hi-Link HLK-PM01 (AC → 5V)
- **I/O:** status LED, manual push-button (SW2)
- **Connectors:** 5.08mm pitch screw terminals for mains in/out
- Custom PCB (KiCad), hand-solderable THT components throughout

## Firmware

- **Toolchain:** PlatformIO, `framework = espidf`
- **Application logic:** plain ESP-IDF C — GPIO config, debounced button input, LED/relay control
- **HomeKit layer:** `esp-homekit-sdk` (Espressif's open-source HAP implementation), added as an ESP-IDF component
- WiFi credentials configured via `idf.py menuconfig` (hardcoded); HomeKit pairing done separately after WiFi connects via the HomeKit setup code

## Setup

- `git clone git@github.com:mempb/ESP32-SmartPlug.git`
- Install `PlatformIO` VSCode extension
- WiFi SSID & password: 
    - Option A: `pio run -t menuconfig` → App Wi-Fi → Source of Wi-Fi Credentials → Use Hardcoded → set SSID and password
    - Option B: edit `sdkconfig.esp32-c3-devkitm-1`:
      - `CONFIG_APP_WIFI_SSID="SSID"`
      - `CONFIG_APP_WIFI_PASSWORD="pass"`
- Build/flash: `pio run -t upload -t monitor`

## Roadmap

- [x] Schematic design
- [x] PCB layout and routing, order
- [x] PCB assembly (hand soldering)
- [X] GPIO firmware: button input, debounce, LED toggle
- [X] HomeKit firmware (esp-homekit-sdk integration, relay control, button + LED sync)
- [X] Respin PCB, order (smaller form factor, fix missing GND trace, add capacitor)
- [ ] Enclosure design/print
- [ ] Final writeup: build photos, schematics, wiring diagram, etc

## Repo Structure

```
/pcb        KiCad project
/src        ESP-IDF source
/docs       Schematics, pictures, etc. (TODO)
/housing    3D print housing design (TODO)
```

## References

- https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html
- https://github.com/espressif/esp-homekit-sdk