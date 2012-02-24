#ifndef JACKPP_H
#define JACKPP_H

#include <jack/jack.h>
#include <jack/midiport.h>
#include <jack/ringbuffer.h>

#include "midi.h"

//LICENSE
//Free stuff blah blah.
//Inspired by https://github.com/harryhaaren/JACK-MIDI-Examples#readme , thx!

class JackPP
{
public:

    ~JackPP();

    static int staticProcess(jack_nframes_t nframes, void *arg);

    bool ok() const;

    static JackPP * server();

    void sendMidi(const Midi::message & msg);

private:
    JackPP();

    int process(jack_nframes_t nframes);

    static JackPP * _server;
    jack_client_t * _client;
    jack_port_t   * _midi_out_port;

    jack_ringbuffer_t * _midi_ring = 0;

    bool _ok = false;
};

#endif // JACKPP_H
