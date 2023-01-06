# Adafruit nRF52840 Feather Express Notecard Outboard Firmware Update

Examples of Notecard Outboard Firmware Update on the Adafruit Feather nRF52840 Express

## Hardware

To try out any of these nRF52840 examples, you will need

* an [Adafruit Feather nRF52840 Express](https://www.adafruit.com/product/4062?gclid=CjwKCAiAh9qdBhAOEiwAvxIokxLunz0eaSt1G6FXLscbVd6eswxs_La2SP-qHfRYP6VBp_OOnlwOJxoC0WQQAvD_BwE)
* a [Notecarrier-F](https://shop.blues.io/collections/notecarrier/products/notecarrier-f)
* a [Notecard](https://shop.blues.io/collections/notecard)
* a micro USB cable

## Notecard and Notehub Setup

1. Ensure the DFU DIP switch on the Notecarrier is in the ON position.
2. Insert the Notecard into the Notecarrier as described in our [quickstart guide](https://dev.blues.io/quickstart/notecard-quickstart/notecard-and-notecarrier-f/).
3. Connect the micro USB cable from your computer to the Notecarrier micro USB port
4. Continue to follow the quickstart guide to create a Notehub project and setup the ProductUID on the notecard using the in-browser terminal.
5. IMPORTANT: While still connected to the in-browser terminal, enter this request
```
    {"req":"card.dfu","name":"nrf52"}
```
 
6. Disconnect the micro USB cable from the Notecarrier
7. Insert the Adafruit Feather nRF52840 Express into the feather socket on the Notecarrier.
8. Connect the USB cable to the USB port on the Adafruit Feather nRF52840 Express.
9. You should see a new drive appear called `FHR840BOOT`. (If not, double tap the reset button.)

## Update the Bootloader

1. Follow the [Update Bootloader](https://learn.adafruit.com/introducing-the-adafruit-nrf52840-feather/update-bootloader) guide to be sure your current bootloader is at least 0.6.1.
2. Press the reset button the nRF52480
3. Download [this file](../binaries/adafruit_feather_nrf52840_express/bootloader/update-feather_nrf52840_express_bootloader-0.7.0-15-g62b53ae-dirty_nosd.uf2) and copy to the `FHR840BOOT` boot drive.
4. Verify the bootloader update was successful by opening `INFO_UF2.TXT` from the boot drive and verity the version reads `UF2 Bootloader 0.7.0-15-g62b53ae-dirty`

## Install Notecard CLI

To create the binpack used with Notecard Outboard Firmware Update, you'll need to install the [notecard CLI](https://dev.blues.io/tools-and-sdks/notecard-cli/).

## Setup complete

That's the setup required of the hardware and bootloader and binpack tooling. Now you're ready to compile and build [blinky](./blinky) for [Arduino](./blinky/Arduino/) or [Circuit Python](./blinky/circuitpython).

