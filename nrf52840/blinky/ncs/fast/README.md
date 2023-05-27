# Adding MCUBoot to an exsiting Nordic Connect SDK Project

> **Note**: Preliminary documentation

This MCUBoot example is based on the `zephyr/samples/basic/blinky` example from the Nordic Connect SDK (NCS).

This example has been configured and tested on Adafruit nRF52840 Feather Express, although it
is expected to work with other Nordic Devices with only minor changes.

These are the steps you take to add MCUBoot support to an existing NCS application:

1. Add `CONFIG_BOOTLOADER_MCUBOOT=y` to your project's `prj.conf` file. This enables MCUBoot for the project.

2. Copy the contents of `child_image` to the `child_image` directory in your project. You may need to create this directory if it is not already present. This folder contains additional configuration for MCUBoot.

3. DFU mode is triggered via an activation pin that MCUBoot checks shortly after device reset. In this example, we used the Feather's D2 pin, which is GPIO 0, Port 10. The activation pin is defined in `child_image/mcuboot.overlay`:

```
    buttonDFU: button_dfu {
        gpios = <&gpio0 10 (GPIO_PULL_UP | GPIO_ACTIVE_LOW)>;
        label = "DFU activation button";
    };
```

If you wish to use a different pin to activate the bootloader, edit `gpio0 10` to correspond with the GPIO peripheral and port of your chosen pin.

4. Use `west build` and `west flash` to rebuild and upload the project to the device.

5. When the device is reset with the activation pin held low, MCUBoot will start in serial recovery mode. This is indicated via the board's LED, which is ON while MCUBoot is active.

6. You can use the `mcumgr` tool inspect and test image updates via MCUBoot. Here's a brief ouline of how to install and use the tool. You can read more about the tool in the Zephyr project [mcumgr page](https://docs.zephyrproject.org/latest/services/device_mgmt/mcumgr.html)

    * If not already installed, install `mcumgr`. This assumes `go` version >= 1.18 is installed.
  ```
    go install github.com/apache/mynewt-mcumgr-cli/mcumgr@latest
    export PATH=$PATH:~/go/bin
  ```
  
    * Add the connection to `mcumgr`
    ```
    export MCUMGR_DEV=<USB-serial-device>
    mcumgr conn add acm0 type="serial" connstring="dev=$MCUMGR_DEV,baud=115200,mtu=512"
    ```

    * Update the application on the device via MCUBoot:
    ```
    mcumgr -c acm0 image upload build/zephyr/app_update.bin
    ```

# Resources

* [NCS - Adding an immutable bootloader](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/app_dev/bootloaders_and_dfu/bootloader_adding.html#id11)
* [`mcumgr`@Zephyr](https://docs.zephyrproject.org/latest/services/device_mgmt/mcumgr.html)
