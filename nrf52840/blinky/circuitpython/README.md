# Circuit Python implementation of Blinky


## Prerequisites

* [Adafruit Feather nRF52840 Express CircuitPython interupter](../../../binaries/circuitpython/adafruit_feather_nRF52840_express/)


## Creating a binpack for CircuitPython scripts

Creaing binpack file for CircuitPython scripts is done in 2 stages:

1. run the [CircuitPython filesystem builder](https://github.com/blues/circuitpython-fs-builder) to create a `.cpy` file containing the scripts. For example,

```
    python3 main.py fast_led fast_led.cpy
```

2. invoke the notecard CLI to create a binpack for nRF52 containing the scripts

```
    notecard -output fast_led.cpy.binpack -binpack nrf52 fast_led.cpy
```

Making a separate binpack for the script means you can update just the scripts without having to update the CircuitPython interpreter, which is a much larger binary.

However, for initial deployment, or when the scripts and interpreter must be updated together for compatibility reasons, then these two can be combined into a single binpack, like this

```
    notecard -output fast_led_v2.cpy.binpack -binpack nrf52 adafruit-circuitpython-feather_nrf52840_express-en_US-7.3.3.zip fast_led.cpy
```