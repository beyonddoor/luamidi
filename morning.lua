local scheduler = require "scheduler"
local notation = require "notation"

local notes = {"D4q", "E4q", "D4q", "G4q", "Fs4h"}
local function play_song()
    for i = 1, #notes do
        local symbol = notation.parse_note(notes[i])
        notation.play(symbol.note, symbol.duration)
    end
end

local function counter()
    for i = 1, math.huge do
        print("counter: " .. i)
        scheduler.wait(0.01)
    end
end

scheduler.schedule(0.0, coroutine.create(play_song))
scheduler.schedule(0.0, coroutine.create(counter))
scheduler.run()