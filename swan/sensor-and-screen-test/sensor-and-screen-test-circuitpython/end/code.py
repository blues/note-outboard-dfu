import board
import time
import adafruit_bme680
import notecard
import displayio
import terminalio
from adafruit_display_text import label
import adafruit_displayio_ssd1306

productUID = "com.blues.bsatrom:outboard_dfu_demo"
version = "v2.0"

displayio.release_displays()

def clear_screen(screen_group):
  color_bitmap = displayio.Bitmap(128, 32, 1)
  color_palette = displayio.Palette(1)
  color_palette[0] = 0x000000 # Black
  bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)
  screen_group.pop()
  screen_group.insert(2, bg_sprite)

i2c = board.I2C()
sensor = adafruit_bme680.Adafruit_BME680_I2C(i2c)
card = notecard.OpenI2C(i2c, 0, 0, debug = True)

oled_bus = displayio.I2CDisplay(i2c, device_address=0x3c)
display = adafruit_displayio_ssd1306.SSD1306(oled_bus, width=128, height=32)

splash = displayio.Group()
display.show(splash)

color_bitmap = displayio.Bitmap(128, 32, 1)
color_palette = displayio.Palette(1)
color_palette[0] = 0xFFFFFF  # White

bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)
splash.append(bg_sprite)

# Draw a smaller inner rectangle
inner_bitmap = displayio.Bitmap(118, 24, 1)
inner_palette = displayio.Palette(1)
inner_palette[0] = 0x000000  # Black
inner_sprite = displayio.TileGrid(inner_bitmap, pixel_shader=inner_palette, x=5, y=4)
splash.append(inner_sprite)

text = "Hello Swan {}!".format(version)
text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=18, y=15)
splash.append(text_area)

req = {"req": "hub.set"}
req["product"] = productUID
req["mode"] = "continuous"
card.Transaction(req)

req = {"req": "card.dfu"}
req["name"] = "stm32"
req["on"] = True
card.Transaction(req)

time.sleep(5)

while True:
  clear_screen(splash)

  temp = sensor.temperature
  humidity = sensor.humidity
  pressure = sensor.pressure
  gas = sensor.gas
  print('Temperature: {} degrees C'.format(temp))
  print('Humidity: {}%'.format(humidity))
  print('Pressure: {} kPa'.format(pressure))
  print('Gas: {}'.format(gas))


  req = {"req": "note.add"}
  req["file"] = "sensors.qo"
  req["sync"] = True
  req["body"] = { "temp": temp, "humidity": humidity, "pressure": pressure, "gas": gas}
  card.Transaction(req)

  text = "Temp: {} degC\nHumidity: {}%".format(temp, humidity)
  text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=10, y=10)
  splash.append(text_area)

  time.sleep(5)

  clear_screen(splash)

  text = "Press: {} kPa\nGas: {}".format(pressure, gas)
  text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=10, y=10)
  splash.append(text_area)

  time.sleep(15)
