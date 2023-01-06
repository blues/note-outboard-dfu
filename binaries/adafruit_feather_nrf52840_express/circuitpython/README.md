
## CircuitPython Interpreter for Adafruit Feather nRF52840 Express

> NB: Ensure your device has the [bootloader](../bootloader/README.md) installed before following these steps.

The [Circuit Python distribution](https://circuitpython.org/board/feather_nrf52840_express/) for this board provides a `.uf2` file. This is indended for UF2 bootloaders. To use this with Notecard Outboard Firmware Updates, the `.uf2` file should first be converted to a `.bin` file.

We have done this for the current release at the time of writing (7.3.3) which you can find in this folder. If you wish to create your own, the steps are given below.

### Convering `.uf2` to `.bin`

Python 3 is required. Tested on version 3.9.13.

1. Download the `.uf2` file from the [Circuit Python distribution](https://circuitpython.org/board/feather_nrf52840_express/)

2. download [`uf2conv.py`](https://github.com/microsoft/uf2/blob/19615407727073e36d81bf239c52108ba92e7660/utils/uf2conv.py). (This is the version linked to in the CircuitPython repo. The latest version did not work.)

3. Run the `uf2conv.py` script to convert the uf2 file to binary:

```
    python3 uf2conv.py -c adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.uf2
```

This creates a new file `flash.bin` in the current directory. Rename this to be more specific

```
    mv flash.bin adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.bin
```

### Create a nRF52840 package

1. Install the [`adafruit-nrfutil`](https://github.com/adafruit/Adafruit_nRF52_nrfutil) utility.
2. Create a nRF52 package for the binary
```
    adafruit-nrfutil dfu genpkg --dev-type 0x0052 --application adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.bin adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.zip
```

The resulting `.zip` file can now be used with Notecard CLI to create a binpack.

### Create a binpack

```
    notecard -output adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.zip.binpack -binpack nrf52 adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.zip
```

You can then upload the binpack to Notehub as Host Firmware.
