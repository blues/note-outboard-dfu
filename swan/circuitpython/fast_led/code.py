"""LED blinky with a recognizable pattern."""

from time import sleep
from digitalio import DigitalInOut, Direction
import board


def signatureBlinky(pin):
    for i in range(20, -10, -1):
        scale = max(i,0)+4
        pin.value = True
        sleep(0.005 * scale)        
        pin.value = False
        sleep(0.003 * scale)


def main():
    led = DigitalInOut(board.LED)
    led.direction = Direction.OUTPUT
    print("CircuitPython blinky signature. Hit CTRL-C to stop.")
    while True:
        signatureBlinky(led)

main()
