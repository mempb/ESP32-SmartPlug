# ESP32-C3 HomeKit Smart Plug

A standalone WiFi smart plug built around the ESP32-C3 Super Mini, exposing native Apple HomeKit control via [HomeSpan](https://github.com/HomeSpan/HomeSpan).

## Overview

The ESP32-C3 connects directly to the home WiFi network and implements the HomeKit accessory protocol on-device using HomeSpan. It advertises itself over mDNS/Bonjour and pairs directly with the Apple Home app. A relay switches mains power to a connected load (tested with a small AC fan), with local status LED and a manual push-button override.

## Hardware

- **MCU:** ESP32-C3 Super Mini (WiFi + BLE)
- **Relay:** Songle SRD-05VDC-SL-C
- **Relay drive:** 2N3904 NPN, low-side switched, 1k base resistor, 1N4007 flyback diode across the coil
- **Mains supply:** Hi-Link HLK-PM01 (AC → 5V)
- **I/O:** status LED, manual push-button (SW2)
- **Connectors:** 5.08mm pitch screw terminals for mains in/out
- Custom PCB (KiCad), hand-solderable THT components throughout

## Roadmap

- [x] Schematic design
- [x] PCB layout and routing
- [x] PCB ordered
- [ ] PCB assembly (hand soldering)
- [ ] HomeSpan firmware (WiFi provisioning, relay control, button + LED logic, HomeKit accessory definition)
- [ ] Bring-up and pairing test with Apple Home
- [ ] Enclosure design/print
- [ ] Final writeup: build photos, schematics, wiring diagram, etc

## Repo Structure

```
/hardware   KiCad project, schematics, Gerbers
/firmware   HomeSpan (ESP-IDF) source
/docs       Build notes, photos, wiring reference
```