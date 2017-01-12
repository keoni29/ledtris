import sys
import pygame
import nescolors
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

	for row in range(0, 20):
		for col in range(0, 10):
			cval = ord(fifo.read(1))
			color = nescolors.color[cval]
			rect = (col * (SIZE + SPACING) + OFFSET_X, 
				row * (SIZE + SPACING) + OFFSET_Y, 
				SIZE, 
				SIZE)

			pygame.draw.rect(window, color, rect)

	pygame.display.update()

