# Arduino implementation of Blinky

### One time setup - Installing libraries and support for Adafruit nRF52 boards

1. Install the Arduino IDE
2. Follow the steps in the parent [README.md](./README.md)
3. Add the `Blues Wireless Notecard` library via the Arduino IDE's library manager.
4. Using the Board Manager, install "Adafruit nRF52 Boards".
5. Select "Tools" > "Board:" > "Adafruit nRF52 Boards" > "Adafruit Feather nRF52840 Express"
6. Finally, enable verbose compiler output by opening the Preferences dialog and ensuring "verbose output during" [x] "compilation" is checked.

### Compiling a Notecard blinky example

1. Open the `fast/fast.ino` sketch from this folder from the Arduino IDE
2. Select the "Sketch" menu and then "Compile/Verify"
3. The output logs show where the resulting compiled file is. The file you need to use to create a binpack is the `.zip` file (not the `.bin` file as is usually the case.)

## Creating a binpack for Notecard Outboard Firmware Updates

Run this command to create a binpack for the `.zip` file produced above

```
notecard -output arduino-fast.binpack -binpack nrf52 fast.ino.zip -output fast.ino.binpack
```

Which will create the file `arduino-fast.binpack` in the current directory.

You can then continue the remainder of the [tutorial] to upload the binpack to Notehub and have it delivered over the air to your Notecard.

Once you see the LED blinking fairly quickly, rinse and repeat "Compiling a Notecard blinky example" above for the slow blinky app.

