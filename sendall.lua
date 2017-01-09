-- script for getting data directly from zero page of tetris game

function memoryBcd(address, count)
	val = 0

	for i = count - 1, 0, -1 do
		b = memory.readbyte(address + i)
		val = val * 100 + math.floor(b / 16) * 10 + b % 16
	end

	return val
end

-- Begin program
emu.message("Script Started")

portName = '/dev/ttyACM0'
port = io.open(portName, 'wb')
os.execute('stty -F'..portName..' 1000000')

frame = 0

-- Main loop begin
while true do
-- Roughly 20 frames per second refresh rate for the LED display
-- Code on arduino could be optimized so 30 frames per second
-- can be achieved. For now this will suffice.
	frame = (frame + 1) % 3

	nextID = memory.readbyte(0x00BF)
	level = memory.readbyte(0x0044)
	score = memoryBcd(0x0053,3)
	-- TODO Hi-score
	mouse = input.get()

	i = 0
	topx = 99
	topy = 51
	if frame == 0 then
		for row = 19, 0, -1 do
			for col = 0, 9, 1 do
				tile = memory.readbyte(i)
				i = i + 1
				x = topx + col * 8
				y = topy + row * 8
				gui.pixel(x, y, {r=255,g=255,b=255,a=100})
				r,g,b = emu.getscreenpixel(x, y, true)
				port:write(string.char(r,g,b))
			end
		end
		port:flush()
	end
	yy = 10
	gui.text(1,yy*2,"nextID="..nextID)
	gui.text(1,yy*3,"level="..level)
	gui.text(1,yy*4,"score="..score)
	gui.text(1,yy*5,"mx="..mouse.xmouse)
	gui.text(1,yy*6,"my="..mouse.ymouse)
	emu.frameadvance()
end
