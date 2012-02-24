#ifndef JACKPP_H
#define JACKPP_H

#include <jack/jack.h>

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

private:
    JackPP();

    int process(jack_nframes_t nframes);

    static JackPP * _server;
    jack_client_t * _client;
    jack_port_t   * _midi_out_port;

    bool _ok = false;
};

#endif // JACKPP_H
