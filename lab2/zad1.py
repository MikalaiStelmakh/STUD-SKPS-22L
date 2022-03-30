import gpio4
import time
import sys

class GPIOwrap(gpio4.SysfsGPIO):
    def __init__(self, pin, direction):
        super().__init__(pin)
        self.export = True
        self.direction = direction

    def __del__(self):
        self.export = False

pin = 23
if __name__ == "__main__": 
    pin23 = GPIOwrap(pin, "out")
    for i in range(10):
        pin23.value = 1
        time.sleep(5)
        pin23.value = 0
        time.sleep(5)