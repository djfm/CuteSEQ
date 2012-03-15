#include "events.h"

#include <iostream>

Event::Event(int time, Type type) : _time(time), _type(type)
{
}

int Event::time() const
{
    return _time;
}

Event::Type Event::type() const
{
    return _type;
}

Event::~Event()
{
}

bool same(const Event * a, const Event * b)
{
    if(a->time() != b->time() or a->type() != b->type())return false;
    else
    {
        if(a->type() == Event::NoteOn or a->type() == Event::NoteOff)
        {
            return static_cast<const NoteEvent*>(a)->note() == static_cast<const NoteEvent*>(b)->note();
        }
        else return false;
    }
}

NoteEvent::NoteEvent(Event::Type type, const PhysicalNote &note, int time)
    :Event(time,type),_note(note)
{
}

const PhysicalNote &NoteEvent::note() const
{
    return _note;
}


EventQueue::EventQueue(QObject *parent)
    :QObject(parent)
{
    _chronometre =  new Chronometre((int)(1000.0/25),this);
    _timer = new QTimer(this);
    _timer->setSingleShot(true);
    connect(_timer,SIGNAL(timeout()),this,SLOT(play()));
}

void EventQueue::add(Event *event)
{
    _queue.insert(std::make_pair(event->time(),event));
}

void EventQueue::remove(Event *event, bool andDelete)
{
    for(auto i = _queue.find(event->time()); i != _queue.end() and i->first == event->time();)
    {
        decltype(i) k; bool kill = false;
        if(same(i->second,event))
        {
            k = i; kill = true;
        }
        ++i;
        if(kill)
        {
            if(andDelete)delete k->second;
            _queue.erase(k);
        }
    }
}

void EventQueue::show()
{
    std::cout<<"Event queue : ";
    for(const auto & p : _queue)std::cout<<p.first<<" ";
    std::cout<<std::endl;
}

void EventQueue::play()
{
    if(!_chronometre->running())_chronometre->start();

    auto head = _queue.find(_current_pos);
    if(head != _queue.end())
    {
        while(head != _queue.end() and head->first == _current_pos)
        {
            emit eventSent(head->second);
            ++head;
        }

        if(head != _queue.end())
        {
            int dt = head->first - _current_pos;
            _current_pos = head->first;
            _timer->start(dt);
        }

    }
}

void EventQueue::pause()
{
    _chronometre->pause();
}

void EventQueue::stop()
{
    _current_pos = 0;
    _chronometre->stop();
}

int EventQueue::elapsed()
{
    return _chronometre->elapsed();
}

Chronometre *EventQueue::chronometre()
{
    return _chronometre;
}
