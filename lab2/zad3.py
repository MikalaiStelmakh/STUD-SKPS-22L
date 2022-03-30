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

pin = 18
if __name__ == "__main__": 
	pin18 = GPIOwrap(pin, "out")
	sounds = [261.6, 293.7 , 329.6, 349.2, 391.9, 440, 493.9, 523.2, 587.3, 659.2, 698.4, 784, 880, 987]
	for sound in sounds:
		start = time.perf_counter()
		while time.perf_counter() - start < 1:
			time.sleep(1/sound/2)
			pin18.value = 1
			time.sleep(1/sound/2)
			pin18.value = 0
		time.sleep(2)
