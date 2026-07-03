# STM32

This folder contains example Arduino sketches for these STM32 boards:

    * MicroMod STM32
    * STM32F405 Feather

These examples implement a "blinky" sketch with Notecard Outboard Firmware Updates enabled in firmware.
The Notecard serial number is set to `stm32-micromod` or `stm32-feather`.

## Enabling Outboard Firmware Update in firmware

Both sketches enable Outboard DFU for an STM32 host on a Notecarrier F with the following requests:

```json
{"req":"card.dfu","name":"stm32","on":true,"mode":"aux"}
{"req":"card.aux","mode":"off"}
{"req":"dfu.status","on":true,"version":"1.0.0"}
```

The Notecard must also be in `continuous` or `periodic` mode (set via `hub.set`). On a Notecarrier F the DFU signals are routed over the Notecard's AUX pins, so `mode` is set to `aux` on `card.dfu` and `card.aux` is set to `off` to free those pins for DFU.

The **MicroMod STM32** uses `"name":"stm32-bi"` instead of `"stm32"` because its boot pin is active LOW (inverted). The **STM32F405 Feather** uses `"name":"stm32"`.