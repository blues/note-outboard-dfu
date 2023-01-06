# Swan

This folder contains  Notecard Outboard Firmware Update examples specifically for the [Swan](https://blues.io/products/swan/) MCU.

Three different applications are presented here implemented across several runtime environments:

* Circuit Python
* Arduino
* Zephyr

The example apps have Outboard firmware updates enabled in the firmware. Before programming an app to your Swan, be sure to set the ProductUID on the Notecard. This needs to be done only once.

These are the example apps

* *blinky* running on Arduino and Circuit Python. Two variants of the app are provided, a slow blink and a fast blink. Start with the slow blink variant and then use Outboard DFU to update to the faster version.

* *button-press* running on Arduino, Circuit Python and Zephyr. This app sends a note each time the User button is pressed, with the number of times the button has been pressed so far and the OS the app is running on. This demonstrates how Outboard Firmware Update can be used to migrate application implementation from one OS to another. The apps are equivalent in functionality, and so the Notecard serial number is set to reflect the running OS.

* *sensor-and-screen-test* running on Arduino and Circuit Python. This is a more complex app, requiring a BME680 environment sensor and a SSD1306 display. Both variants of the app display the version number at startup. The Circuit Pyhon variant goes further and displays environmental data, temperature and humidity in the first version, which is expanded in the second version to include gas and pressure readings. This demonstrates incremental deployment of an app's features, which can be deployed Over the Air with Outboard Firmware Update.

