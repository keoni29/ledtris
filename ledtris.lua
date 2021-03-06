-- Filename: ledtris.lua
-- Project: NES Ledtris
-- Author: Koen van Vliet <8by8mail@gmail.com>
--
-- ledtris lua script for FCEUX (NES emulator)
-- Stream pixel data from game to LED matrix display trough serial port
--

-- USER CONFIGURATION --
-- Send pixels for the next block display first, then pixels for the main display
next_block_display_first = true
---------------------------------------------


shape = {
 [8]={0, 1, 1, 0, 1, 1, 0, 0}, -- -_
 [11]={0, 1, 1, 0, 0, 0, 1, 1}, -- _-
 [10]={0, 1, 1, 0, 0, 1, 1, 0}, -- {}
 [18]={1, 1, 1, 1, 0, 0, 0, 0}, -- ----
 [14]={1, 1, 1, 0, 0, 0, 0, 1}, -- ,--
 [7]={0, 1, 1, 1, 1, 0, 0, 0}, -- --,
 [2]={0, 1, 1, 1, 0, 1, 0, 0}  -- _|_
}

function setDefault (t, d)
  local mt = {__index = function () return d end}
  setmetatable(t, mt)
end

setDefault(shape, {0, 0, 0, 0, 0, 0, 0, 0})


function memoryBcd(address, count)
	val = 0

	for i = count - 1, 0, -1 do
		b = memory.readbyte(address + i)
		val = val * 100 + math.floor(b / 16) * 10 + b % 16
	end

	return val
end

function showNextBlock(port, nextID)
    local i

    for i = 1, 8, 1 do
        if shape[nextID][i] == 1 then
            r,g,b,palette = emu.getscreenpixel(209,125, true)
        else
            r = 0
            g = 0
            b = 0
        end
		
		r = r/2
		g = g/2
		b = b/2

        port:write(string.char(r,g,b))
    end
end

-- Begin program
emu.message("Script Started")

-- Change serial port name here
local portName = '/dev/ttyACM0'
local port = io.open(portName, 'wb')

-- Try other serial ports
if port == nil then
	portName = '/dev/ttyACM1'
	port = io.open(portName, 'wb')
end

if port == nil then
	portName = '/dev/ttyUSB0'
	port = io.open(portName, 'wb')
end

if port == nil then
	portName = '/dev/ttyUSB1'
	port = io.open(portName, 'wb')
end

os.execute('stty -F'..portName..' 1000000')

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
	frame = (frame + 1) % 2

	local flag = 0 -- go up
	-- Refresh the LED display at 30FPS
	if frame == 0 and port ~= nil then
		-- Send synchronization token
		port:write(string.char(255))

        if next_block_display_first then
			showNextBlock(port, nextID)
		end

		for col = 0, 9, 1 do
			for row = 0, 19, 1 do
				x = topx + col * 8
				
				-- Reverse direction every column because of
				-- the zig-zag pattern in the display.
				if col%2 == 0 then
					y = topy + (19 - row) * 8
				else
					y = topy + row * 8
				end

				-- Read pixels from the screen
				r,g,b,palette = emu.getscreenpixel(x, y, true)

				-- Black pixels converted to dark blue
				-- if b == 0 then
				-- 	b = 20
				-- end

				-- Reduce brightness
				r = r/2
				g = g/2
				b = b/2

				-- Send RGB value of pixel to display
				port:write(string.char(r,g,b))
				--gui.text(x, y, palette)
			end
			-- Flush buffer to serial port
			port:flush()
		end

        if not next_block_display_first then
			showNextBlock(port, nextID)
		end

        -- Send current score in three bytes
        port:write(string.char(bit.band(bit.rshift(score,16), 0xFF)))
        port:write(string.char(bit.band(bit.rshift(score,8), 0xFF)))
        port:write(string.char(bit.band(score, 0xFF)))

        -- Flush buffer to serial port
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

	-- Show some information about the game
	yy = 10
	gui.text(1,yy*2,"nextID="..nextID)
	gui.text(1,yy*3,"level="..level)
	gui.text(1,yy*4,"score="..score)
	gui.text(1,yy*5,"mx="..mouse.xmouse)
	gui.text(1,yy*6,"my="..mouse.ymouse)

	-- Skip to next frame
	emu.frameadvance()
end
