#ifndef MIDI_H
#define MIDI_H

#include <vector>



class Midi
{
public:

    typedef std::vector<char> message;

    static message noteOff(unsigned char note, unsigned char velocity);
    static message noteOn(unsigned char note, unsigned char velocity);
};

#endif // MIDI_H
