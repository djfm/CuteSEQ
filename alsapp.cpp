#include "alsapp.h"
#include <iostream>

AlsaPP * AlsaPP::_instance = 0;

int ACE(int err) //as in AlsaCheckError
{
    if(err < 0)
    {
        std::cerr<<"Error happened in alsa : "<<snd_strerror(err)<<std::endl;
        std::terminate();
    }

    return err;

}

AlsaPP::AlsaPP()
{
    ACE(snd_seq_open(&_seq,"default", SND_SEQ_OPEN_OUTPUT,0));
    ACE(snd_seq_set_client_name(_seq,"CuteSEQ"));
    _midi_out_port = snd_seq_create_simple_port(_seq,"midi_out",SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,SND_SEQ_PORT_TYPE_APPLICATION | SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    ACE(_midi_out_port);
    _client_id = snd_seq_client_id(_seq);
    std::cout<<"ALSA CLIENT ID IS "<<_client_id<<std::endl;
}

AlsaPP *AlsaPP::server()
{
    if(_instance == 0)_instance = new AlsaPP();
    return _instance;
}

void AlsaPP::sendEvent(snd_seq_event_t *ev)
{
    ACE(snd_seq_ev_set_direct(ev));
    ACE(snd_seq_ev_set_source(ev,_midi_out_port));
    ACE(snd_seq_ev_set_subs(ev));
    ACE(snd_seq_event_output_direct(_seq,ev));
}

void AlsaPP::sendNoteOn(unsigned char channel, unsigned char note, unsigned char velocity)
{
    snd_seq_event ev;
    ACE(snd_seq_ev_set_noteon(&ev,channel,note,velocity));
    sendEvent(&ev);
}

void AlsaPP::sendNoteOff(unsigned char channel, unsigned char note, unsigned char velocity)
{
    snd_seq_event ev;
    ACE(snd_seq_ev_set_noteoff(&ev,channel,note,velocity));
    sendEvent(&ev);
}
