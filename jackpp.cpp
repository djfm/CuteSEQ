#include "jackpp.h"

#include <iostream>

JackPP * JackPP::_server = 0;

JackPP::JackPP()
{
    if ((_client = jack_client_open("CuteSEQ", JackNullOption, NULL)) == 0)
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
            }
        }
    }
}

JackPP::~JackPP()
{
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

int JackPP::process(jack_nframes_t nframes)
{
    return 0;
}
