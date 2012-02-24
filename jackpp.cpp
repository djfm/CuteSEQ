#include "jackpp.h"

#include <iostream>

JackPP * JackPP::_server = 0;

JackPP::JackPP()
{
    if ((_client = jack_client_open("CuteSEQ",JackNullOption,NULL)) == 0)
    {
        std::cout << "jack server not running?" << std::endl;
    }
    else
    {
        if((_midi_out_port = jack_port_register(_client,"midi_out",JACK_DEFAULT_MIDI_TYPE,JackPortIsOutput,0)) == 0)
        {
            std::cout<<"Well, it's not working. Can't register midi_out."<<std::endl;
        }
        else
        {
            jack_set_process_callback(_client,staticProcess,this);
            if(jack_activate(_client) != 0)
            {
                std::cout<<"Was good till there, but I can't activate the jack client."<<std::endl;
            }
            else
            {
                _ok = true;
                _midi_ring = jack_ringbuffer_create(2048);
            }
        }
    }
}

JackPP::~JackPP()
{
    jack_ringbuffer_free(_midi_ring);
}

int JackPP::staticProcess(jack_nframes_t nframes, void *arg)
{
    return ((JackPP*)(arg))->process(nframes);
}

bool JackPP::ok() const
{
    return _ok;
}

JackPP *JackPP::server()
{
    if(_server == 0)
    {
        _server = new JackPP();
    }
    return _server;
}

void JackPP::sendMidi(const Midi::message &msg)
{
    /*
    unsigned char buf[msg.size()]; for(int i = 0; i < msg.size(); i++)buf[i]=msg[i];
    void * port_buffer = jack_port_get_buffer()
    jack_midi_clear_buffer(_midi_out_port);
    jack_midi_event_write(_midi_out_port,0,buf,msg.size());*/
    jack_ringbuffer_write(_midi_ring,&msg[0],msg.size());
}

int JackPP::process(jack_nframes_t nframes)
{
    std::cout<<"Process!"<<std::endl;
    return 0;
}
