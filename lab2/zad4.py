import gpio4
import time

class GPIOwrap(gpio4.sysfsGPIO):
	def __init__(self, pin, direction):
		super().__init__(pin)
		self.export = True
		self.direction = direction
	def __del__(self):
		self.export = False

if __name__ == "__main__":
	gpio27 = GPIOwrap(27, "out")
	gpio18 = GPIOwrap(cw1827, "in")
	while True:
	if gpio18.value == 1:
		gpio27.value = 1
	else:	
		gpio27.value = 0
	time.sleep(1)
