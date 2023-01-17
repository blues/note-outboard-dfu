# Notecard Outboard Firmware Update Examples

This repo contains resources related to testing, using and demonstrating the [Notecard Outboard Firmware Update](https://dev.blues.io/guides-and-tutorials/notecard-guides/notecard-outboard-firmware-update/) feature.

The examples are organized by board in these subdirectories:

* [nRF52840](./nrf52840/): Examples for the Adafruit Feather nRF52840 Express, for Arduino and CircuitPython.

* [STM32](./stm32/): Features simple Arduino sketches showing how to enable Outboard Firmware Updates for the STM32F405 Feather and Micromod STM32 module.

* [Swan](./swan/): Here you'll find basic and more advanced examples for the [Swan](https://blues.io/products/swan/) board implemented across Arduino, CircuitPython and Zephyr.

## Setting ProductUID

 The examples in this repo do *not* set the `ProductUID` on the Notecard from firmware. You will need to configure the Notecard with the ProductUID of your Notehub project before running an example.

> Note: For more info, see [Notehub Walkthrough / Finding a ProductUID](https://bit.ly/product-uid)

## Using `card.dfu` to set the MCU type

The firmware enables Outboard Firmware Updates and sets the MCU type via the `name` property of the [`card.dfu`](https://dev.blues.io/reference/notecard-api/card-requests/#card-dfu) request. Should you start with firmware flashed locally (not using Outboard Firmware Updates), it is not necessary to pre-configure the Notecard with `card.dfu`, although doing so is perfectly fine.

However, if you are using Outboard Firmware Updates with a host in factory condition, then you must configure the Notecard with the host MCU type via the `card.dfu` request's `name` argument. This can be done with the [Notecard CLI](https://dev.blues.io/tools-and-sdks/notecard-cli/) or with the in-browser terminal at [dev.blues.io](https://dev.blues.io).
