import board
import time
import digitalio
from adafruit_debouncer import Debouncer
import notecard

btn = digitalio.DigitalInOut(board.BUTTON_USR)
btn.direction = digitalio.Direction.INPUT
btn.pull = digitalio.Pull.UP
switch = Debouncer(btn)

def send_note(card, count):
    req = {"req": "note.add"}
    req["sync"] = True
    req["body"] = { "button_count": count, "os": "circuit-python"}
    req = card.Transaction(req)

def main():
    button_pressed_count = 0

    i2c = board.I2C()
    card = notecard.OpenI2C(i2c, 0, 0, debug = True)

    req = {"req": "hub.set"}
    req["mode"] = "continuous"
    req["sn"] = "circuitpython-notecard"
    rsp = card.Transaction(req)

    req = {"req": "card.dfu"}
    req["name"] = "stm32"
    req["on"] = True
    rsp = card.Transaction(req)

    while True:
        switch.update()

        if switch.fell:
            button_pressed_count += 1
            send_note(card, button_pressed_count)
        time.sleep(0.1)

main()
