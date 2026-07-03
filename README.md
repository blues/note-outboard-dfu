# ⚠️ This repository is ARCHIVED

**The Notecard Outboard Firmware Update examples have moved into the official Notecard SDK repositories and are maintained there. This repo is read-only and no longer updated.**

| Example | New home |
| --- | --- |
| **Arduino** (STM32 — Swan, Cygnet, STM32F405 Feather, MicroMod STM32) | [note-arduino → `examples/Example10_OutboardFirmwareUpdate`](https://github.com/blues/note-arduino/tree/master/examples/Example10_OutboardFirmwareUpdate) |
| **CircuitPython** (Swan) | [note-python → `examples/outboard-dfu`](https://github.com/blues/note-python/tree/main/examples/outboard-dfu) |
| **Zephyr** (Swan / Cygnet) | [note-zephyr → `examples/outboard-dfu`](https://github.com/blues/note-zephyr/tree/main/examples/outboard-dfu) |
| **Zephyr + MCUboot** (Adafruit Feather nRF52840) | [note-zephyr → `examples/mcuboot-blinky`](https://github.com/blues/note-zephyr/tree/main/examples/mcuboot-blinky) |

For an overview, see [Notecard Outboard Firmware Update](https://dev.blues.io/notehub/host-firmware-updates/notecard-outboard-firmware-update/) on the Blues developer site.

> The content below is retained for historical reference only.

---

# Notecard Outboard Firmware Update Examples

This repo contains resources related to testing, using and demonstrating the [Notecard Outboard Firmware Update](https://dev.blues.io/guides-and-tutorials/notecard-guides/notecard-outboard-firmware-update/) feature.

The examples are organized by board in these subdirectories:

* [STM32](./stm32/): Simple Arduino sketches showing how to enable Outboard Firmware Updates for the STM32F405 Feather and Micromod STM32 module.

* [Swan](./swan/): Examples for the [Swan](https://shop.blues.com/collections/feather-mcu/products/swan) board in Arduino and CircuitPython.

* [Cygnet](./cygnet/): Examples for the [Cygnet](https://shop.blues.com/collections/feather-mcu/products/cygnet) STM32 Feather MCU in Arduino. Cygnet uses the same Outboard Firmware Update configuration as Swan.

> **Zephyr and nRF52840/MCUboot examples now live in [note-zephyr](https://github.com/blues/note-zephyr):** the [outboard-dfu](https://github.com/blues/note-zephyr/tree/main/examples/outboard-dfu) example (Swan/Cygnet) and the [mcuboot-blinky](https://github.com/blues/note-zephyr/tree/main/examples/mcuboot-blinky) example (Adafruit Feather nRF52840).

## Setting ProductUID

 The examples in this repo do *not* set the `ProductUID` on the Notecard from firmware. You will need to configure the Notecard with the ProductUID of your Notehub project before running an example.

> Note: For more info, see [Notehub Walkthrough / Finding a ProductUID](https://bit.ly/product-uid)

## Using `card.dfu` to set the MCU type

The firmware enables Outboard Firmware Updates and sets the MCU type via the `name` property of the [`card.dfu`](https://dev.blues.io/reference/notecard-api/card-requests/#card-dfu) request. Should you start with firmware flashed locally (not using Outboard Firmware Updates), it is not necessary to pre-configure the Notecard with `card.dfu`, although doing so is perfectly fine.

However, if you are using Outboard Firmware Updates with a host in factory condition, then you must configure the Notecard with the host MCU type via the `card.dfu` request's `name` argument. This can be done with the [Notecard CLI](https://dev.blues.io/tools-and-sdks/notecard-cli/) or with the in-browser terminal at [dev.blues.io](https://dev.blues.io).
