# Cygnet

This folder contains Notecard Outboard Firmware Update examples for the [Cygnet](https://shop.blues.com/collections/feather-mcu/products/cygnet) STM32 Feather MCU.

Cygnet is an STM32L433CC-based Feather board (256 KB flash, 64 KB RAM). For Outboard Firmware Update it behaves **identically to the [Swan](../swan/)**: it is an STM32 host on a Notecarrier F, so it uses exactly the same Notecard configuration. Only the build target differs (a different board id and a smaller flash region), so these examples are the Swan examples retargeted for Cygnet.

Before programming an app to your Cygnet, set the ProductUID on the Notecard. This needs to be done only once.

## Enabling Outboard Firmware Update in firmware

The examples enable Outboard DFU from firmware for a Cygnet on a Notecarrier F with the following requests (the same as for Swan):

```json
{"req":"card.dfu","name":"stm32","on":true,"mode":"aux"}
{"req":"card.aux","mode":"off"}
{"req":"dfu.status","on":true,"version":"1.0.0"}
```

The Notecard must also be in `continuous` or `periodic` mode (set via `hub.set`). On a Notecarrier F the DFU signals are routed over the Notecard's AUX pins, so `mode` is set to `aux` on `card.dfu` and `card.aux` is set to `off` to free those pins for DFU.

## The example apps

* *button-press* running on Arduino. Sends a note each time the User button is pressed, with the running count.

* *sensor-and-screen-test* running on Arduino. Requires a BME680 environment sensor and an SSD1306 display; reads the BME680 and publishes environmental readings to Notehub.

> CircuitPython examples are not provided for Cygnet. Where CircuitPython examples exist, see the [Swan](../swan/) folder.

## Building

* **Arduino / PlatformIO** — the `platformio.ini` in each `arduino/` folder targets `board = blues_cygnet` (PlatformIO `ststm32` platform). Build with `pio run`.

Because Cygnet has 256 KB of flash (vs 2 MB on Swan), keep an eye on image size; the examples here are small and fit comfortably.

> A Zephyr version of these examples now lives in [note-zephyr](https://github.com/blues/note-zephyr/tree/main/examples/outboard-dfu) (build with `west build -b cygnet`).
