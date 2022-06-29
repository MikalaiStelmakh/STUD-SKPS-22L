import gpio4
import time
import sys
import math

class GPIOwrap(gpio4.SysfsGPIO):
    def __init__(self, pin, direction):
        super().__init__(pin)
        self.export = True
        self.direction = direction

    def __del__(self):
        self.export = False


if __name__ == "__main__":                 
	gpio23 = GPIOwrap(23, "out")
	start = time.perf_counter()
	while time.perf_counter() - start < 10:
		t = time.perf_counter() - start
		duty_cycle = abs(math.cos(t))
		gpio23.value = 1
		time.sleep(1/60 * duty_cycle)
		gpio23.value = 0
		time.sleep(1/60 * ( 1 - duty_cycle) )