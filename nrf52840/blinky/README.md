# Blinky

The blinky app demonstrates Notecard Outboard Firmware Update with a visual indication (the LED blink speed) showing that the application has been updated over-the-air.

It is implemented with the [Nordic Connect SDK using MCUboot](./ncs/README.md), the supported path for Outboard Firmware Update on the nRF52840 (`{"req":"card.dfu","name":"mcuboot"}`).