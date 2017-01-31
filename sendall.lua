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

local portName = '/dev/ttyACM0'
local port = io.open(portName, 'wb')

os.execute('stty -F'..portName..' 500000')

-- Skip 50 frames
for i=1,50 do
	emu.frameadvance()
end

local frame = 0

-- Main loop begin
while true do
	local nextID = memory.readbyte(0x00BF)
	local level = memory.readbyte(0x0044)
	local score = memoryBcd(0x0053,3)
	-- TODO Hi-score
	local mouse = input.get()
	local topx = 99
	local topy = 51
	frame = (frame + 1) % 4

	local flag = 0 -- go up
	-- Refresh the LED display at 60FPS
	if frame == 0 and port ~= nil then
		for col = 0, 9, 1 do
			for row = 0, 19, 1 do
				x = topx + col * 8

				if col %2 then
					y = topy + (19 - row) * 8
				else
					y = topy + row * 8
				end
				r,g,b,palette = emu.getscreenpixel(x, y, true)
				port:write(string.char(r,g,b))
				--gui.text(x, y, palette)
			end
		end
		port:flush()
	end

	-- Screen overlay for showing which pixels are sampled
	for col = 0, 9, 1 do
		for row = 19, 0, -1 do
			x = topx + col * 8
			y = topy + row * 8
			gui.pixel(x, y, {r=255,g=255,b=255,a=100})
		end
	end

	yy = 10
	gui.text(1,yy*2,"nextID="..nextID)
	gui.text(1,yy*3,"level="..level)
	gui.text(1,yy*4,"score="..score)
	gui.text(1,yy*5,"mx="..mouse.xmouse)
	gui.text(1,yy*6,"my="..mouse.ymouse)
	emu.frameadvance()
end
