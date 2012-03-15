#include "jackpp.h"

#include <cerrno>

#include <iostream>

JackPP * JackPP::_server = 0;

void errf(const char * msg)
{
    std::cerr<<"Jack error'd : "<<msg<<std::endl;
}

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
            jack_set_error_function(errf);
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
    jack_ringbuffer_write(_midi_ring,&msg[0],msg.size());
}

int JackPP::process(jack_nframes_t nframes)
{
    void * mout = jack_port_get_buffer(_midi_out_port,nframes);
    jack_midi_clear_buffer(mout);

    size_t avail = jack_ringbuffer_read_space(_midi_ring);

    int t = jack_last_frame_time(_client);

    if(avail != 0)
    {
        std::cout<<"Got "<<avail<<" bytes of midi data."<<std::endl;
        char arr[avail];
        jack_ringbuffer_read(_midi_ring,arr,avail);
        int err;
        if((err = jack_midi_event_write(mout,t,(unsigned char*)arr,avail)) != 0)
        {
            std::cout<<"Midi Write Failed! ("<<err<<")"<<std::endl;
            if(err == ENOBUFS)std::cout<<"ENOBUFS"<<std::endl;
        }
    }

    //std::cout<<"Process!"<<std::endl;
    return 0;
}
