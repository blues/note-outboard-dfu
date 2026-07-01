# Adafruit nRF52840 Feather Notecard Outboard Firmware Update

Examples of Notecard Outboard Firmware Update on the Adafruit Feather nRF52840 (Express and Sense).

On the nRF52840, Outboard Firmware Update is performed with the
[MCUboot](https://docs.mcuboot.com/) bootloader (`{"req":"card.dfu","name":"mcuboot"}`),
which was added in Notecard firmware **v5.3.1**. The example lives in
**[`blinky/ncs/`](./blinky/ncs/README.md)** and is built with the Nordic Connect SDK (NCS).

## Hardware

To try out the nRF52840 example, you will need

* an Adafruit Feather nRF52840 [Express](https://www.adafruit.com/product/4062) or [Sense](https://www.adafruit.com/product/4516)
* a [Notecarrier-F](https://shop.blues.io/collections/notecarrier/products/notecarrier-f)
* a [Notecard](https://shop.blues.io/collections/notecard) running firmware **v5.3.1 or newer** (required for MCUboot support)
* a USB cable

## Notecard and Notehub Setup

1. Ensure the DFU DIP switch on the Notecarrier is in the ON position.
2. Insert the Notecard into the Notecarrier as described in our [quickstart guide](https://dev.blues.io/quickstart/notecard-quickstart/notecard-and-notecarrier-f/).
3. Connect a USB cable from your computer to the Notecarrier micro USB port.
4. Continue to follow the quickstart guide to create a Notehub project and set the ProductUID on the Notecard using the in-browser terminal.
5. Configure the Notecard for MCUboot Outboard DFU (see [`blinky/ncs/README.md`](./blinky/ncs/README.md) for details):
```
    {"req":"card.aux","mode":"dfu"}
    {"req":"card.dfu","name":"mcuboot"}
```

## Install Notecard CLI

To upload firmware for Notecard Outboard Firmware Update, install the [Notecard CLI](https://dev.blues.io/tools-and-sdks/notecard-cli/).

## Build the example

Head to **[`blinky/ncs/`](./blinky/ncs/README.md)** to build, flash, and run the MCUboot blinky example, then update it over-the-air with Notecard Outboard Firmware Update.
