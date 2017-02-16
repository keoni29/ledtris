import sys
import pygame
from pygame.locals import *

C_BLACK = (0,0,0)
SIZE = 14 		# size of led segment in pixels
SPACING = 2 		# spacing in between led segments
OFFSET_X = 10
OFFSET_Y = 10

pygame.init()
window = pygame.display.set_mode((640,480))

fifo = open('fifo', 'rb')

while True:
	for event in pygame.event.get():
		if event.type == pygame.locals.QUIT:
			sys.exit()

	window.fill(C_BLACK)

	# Wait for synchronization byte
	while True:
		if ord(fifo.read(1)) == 255:
			break

	for col in range(0, 10):
		for row in range(0, 20):
			r = ord(fifo.read(1))
			g = ord(fifo.read(1))
			b = ord(fifo.read(1))

			color = [r,g,b]

			y = row
			if col%2 == 0:
				y = (19 - y)

			rect = (col * (SIZE + SPACING) + OFFSET_X, 
				y * (SIZE + SPACING) + OFFSET_Y, 
				SIZE, 
				SIZE)

			pygame.draw.rect(window, color, rect)

	pygame.display.update()

