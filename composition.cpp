#include "composition.h"
#include "compositionscene.h"
#include "QGraphicsView"
#include "events.h"

#include <QDebug>

#include <iostream>

Composition::Composition(QObject *parent)
    :QObject(parent)
{
    _queue = new EventQueue(this);
}

void Composition::addTrack(Track *track)
{
    _tracks.push_back(track);
    if(_composition_scene != 0)_composition_scene->addTrack(track);

    connect(track,SIGNAL(noteAdded(Track*,PhysicalNote,int,int)),this,SLOT(TrackNoteAdded(Track*,PhysicalNote,int,int)));
    connect(track,SIGNAL(noteRemoved(Track*,PhysicalNote,int,int)),this,SLOT(TrackNoteRemoved(Track*,PhysicalNote,int,int)));
    connect(_queue,SIGNAL(eventSent(Event*)),SLOT(onQueueEventSent(Event*)));

    track->emitAdds();

}

std::deque<Track*> &Composition::tracks()
{
    return _tracks;
}

int Composition::measureDuration() const
{
    return _measure_duration;
}

CompositionScene *Composition::compositionScene(bool create)
{
    if(_composition_scene == 0 and create)_composition_scene = new CompositionScene(this);
    return _composition_scene;
}

void Composition::recreateScene()
{
    if(_composition_scene != 0)
    {
        CompositionScene * new_scene = new CompositionScene(this);
        for(QGraphicsView * view : _composition_scene->views())
        {
            view->setScene(new_scene);
        }
        delete _composition_scene;
        _composition_scene = new_scene;
    }
}

void Composition::play()
{
    _queue->play();
}

void Composition::pause()
{
    _queue->pause();
}

void Composition::stop()
{
    _queue->stop();
}

EventQueue *Composition::queue()
{
    return _queue;
}

void Composition::TrackNoteAdded(Track *track, const PhysicalNote &note, int startMs, int endMs)
{
    qDebug()<<"Added note at "<<startMs<<" till "<<endMs;

    NoteEvent * on  = new NoteEvent(Event::NoteOn,note,startMs);
    NoteEvent * off = new NoteEvent(Event::NoteOff,note,endMs);

    _queue->add(on);
    _queue->add(off);

    _queue->show();

}

void Composition::TrackNoteRemoved(Track *track, const PhysicalNote &note, int startMs, int endMs)
{
    qDebug()<<"Removed note at "<<startMs<<" till "<<endMs;

    NoteEvent on(Event::NoteOn,note,startMs);
    NoteEvent off(Event::NoteOff,note,endMs);

    _queue->remove(&on, true);
    _queue->remove(&off, true);

    _queue->show();

}

void Composition::onQueueEventSent(Event *event)
{
    if(event->type() == Event::NoteOn)
    {
        NoteEvent * ne = static_cast<NoteEvent*>(event);
        std::cout<<"Got NoteOn at "<<event->time()<<" vel : "<<ne->note().velocity()<<" note : "<<ne->note().midi()<<std::endl;
        //Midi::message msg = Midi::noteOn(ne->note().midi(),ne->note().velocity());
        //JackPP::server()->sendMidi(msg);
        AlsaPP::server()->sendNoteOn(1,ne->note().midi(),ne->note().velocity());
    }
    else if(event->type() == Event::NoteOff)
    {
        std::cout<<"Got NoteOff at "<<event->time()<<std::endl;
        NoteEvent * ne = static_cast<NoteEvent*>(event);
        //Midi::message msg = Midi::noteOn(ne->note().midi(),ne->note().velocity());
        //JackPP::server()->sendMidi(msg);
        AlsaPP::server()->sendNoteOff(1,ne->note().midi(),ne->note().velocity());
    }

}
