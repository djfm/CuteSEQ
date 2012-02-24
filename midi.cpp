#include "midi.h"


Midi::message Midi::noteOff(unsigned char note, unsigned char velocity)
{
    return {0x80,note,velocity};
}

Midi::message Midi::noteOn(unsigned char note, unsigned char velocity)
{
    return {0x90,note,velocity};
}
