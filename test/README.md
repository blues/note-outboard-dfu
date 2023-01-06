# Test

This folder contains app that stress test Outboard DFU against a host app that is sending a high volume of commands over the I2C bus to the Notecard.

The apps are Arduino sketches for STM32 and ESP32 boards.

To stress test Notecard Outboard Firmware Updates with a chatty host, flash one of these apps to your host, and then use Outboard Firmware Update to flash a regular app. When the update succeeds, the stress test is passed.