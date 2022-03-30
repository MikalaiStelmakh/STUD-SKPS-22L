import gpio4
import time
import sys
import math
from pwm import generate_pwm, control_state

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
    t_pwm = generate_pwm([100], range(0, 1, 0.01), 10)
    control_state(t_pwm, pin23)