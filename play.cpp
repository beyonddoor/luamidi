extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#include "rtmidi/RtMidi.h"
#include <iostream>

static RtMidiOut midi;

int midi_send(lua_State* L){ 
    double status = lua_tonumber(L, -3); 
    double data1 = lua_tonumber(L, -2); 
    double data2 = lua_tonumber(L, -1);

    std::cout << "status=" << status << " data1=" << data1 << " data2=" << data2 << std::endl;

    std::vector<unsigned char> message(3);
    message[0] = static_cast<unsigned char>(status); 
    message[1] = static_cast<unsigned char>(data1); 
    message[2] = static_cast<unsigned char>(data2);
    midi.sendMessage(&message);
    return 0;
}

void testMidi(RtMidiOut& midi) {
    std::vector<unsigned char> message(3);
    message[0] = 0x90;
    message[1] = 0x90;
    message[2] = 0x90;
    midi.sendMessage(&message);
}

int main(int argc, char *argv[])
{
    unsigned int ports = midi.getPortCount();
    if (ports < 1) { 
        std::cout << "No MIDI ports found" << std::endl;
        return -1; 
    }
    std::cout << "midi count=" << ports << std::endl;
    midi.openPort(0);

    testMidi(midi);

    lua_State* L = luaL_newstate(); 
    luaL_openlibs(L); 

    lua_pushcfunction(L, midi_send);
    lua_setglobal(L, "midi_send");

    luaL_dofile(L, argv[1]);

    luaL_dostring(L, "print('Hello world!')"); 
    lua_close(L); 
    return 0;
}
