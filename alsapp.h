#ifndef ALSAPP_H
#define ALSAPP_H

#include <alsa/asoundlib.h>

class AlsaPP
{
    static AlsaPP * _instance;

    snd_seq_t * _seq;
    int _client_id;
    int _midi_out_port;

    AlsaPP();

    void sendEvent(snd_seq_event_t *ev);

public:

    static AlsaPP * server();

    void sendNoteOn(unsigned char channel, unsigned char note, unsigned char velocity);
    void sendNoteOff(unsigned char channel, unsigned char note, unsigned char velocity);



};

#endif // ALSAPP_H
