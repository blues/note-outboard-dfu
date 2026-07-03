# Swan

This folder contains Notecard Outboard Firmware Update examples specifically for the [Swan](https://shop.blues.com/collections/feather-mcu/products/swan) MCU.

Three different applications are presented here implemented across these runtime environments:

* Circuit Python
* Arduino

The *button-press* and *sensor-and-screen-test* apps enable Outboard Firmware Update in firmware (see below). Before programming an app to your Swan, be sure to set the ProductUID on the Notecard. This needs to be done only once.

## Enabling Outboard Firmware Update in firmware

The *button-press* and *sensor-and-screen-test* examples enable Outboard DFU from firmware for a Swan on a Notecarrier F with the following requests:

```json
{"req":"card.dfu","name":"stm32","on":true,"mode":"aux"}
{"req":"card.aux","mode":"off"}
{"req":"dfu.status","on":true,"version":"1.0.0"}
```

The Notecard must also be in `continuous` or `periodic` mode (set via `hub.set`). On a Notecarrier F the DFU signals are routed over the Notecard's AUX pins, so `mode` is set to `aux` on `card.dfu` and `card.aux` is set to `off` to free those pins for DFU. `dfu.status` enables the host firmware download and reports the running version to Notehub.

The *blinky* example is a minimal update payload and does **not** configure Outboard DFU itself, so configure the Notecard for Outboard DFU with the [Notecard CLI](https://dev.blues.io/tools-and-sdks/notecard-cli/) or the [in-browser terminal](https://dev.blues.io/terminal/) before flashing it.

## The example apps

* *blinky* running on CircuitPython. Two variants of the app are provided, a slow blink and a fast blink. Start with the slow blink variant and then use Outboard DFU to update to the faster version.

* *button-press* running on Arduino and CircuitPython. This app sends a note each time the User button is pressed, with the number of times the button has been pressed so far and the OS the app is running on. This demonstrates how Outboard Firmware Update can be used to migrate application implementation from one OS to another. The apps are equivalent in functionality, and so the Notecard serial number is set to reflect the running OS.

> A Zephyr version of this example now lives in [note-zephyr](https://github.com/blues/note-zephyr/tree/main/examples/outboard-dfu).

* *sensor-and-screen-test* running on Arduino and CircuitPython. This is a more complex app, requiring a BME680 environment sensor and a SSD1306 display. Each variant displays the version number at startup, reads the BME680, and publishes environmental readings to Notehub. The CircuitPython variant is provided in two versions to demonstrate incremental deployment of an app's features Over the Air with Outboard Firmware Update: the first version reports temperature and humidity, expanded in the second version to add gas and pressure readings.

