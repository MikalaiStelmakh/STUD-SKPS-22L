import gpio4
import time

class GPIOwrap(gpio4.sysGPIO):
	def __init__(self, pin, direction):
	super.__init__(pin)
	self.direction = direction
