#include "composition.h"
#include "compositionscene.h"
#include "QGraphicsView"

#include <QDebug>

Composition::Composition(QObject *parent)
    :QObject(parent)
{
}

void Composition::addTrack(Track *track)
{
    _tracks.push_back(track);
    if(_composition_scene != 0)_composition_scene->addTrack(track);

    connect(track,SIGNAL(noteAdded(Track*,PhysicalNote,int,int)),this,SLOT(TrackNoteAdded(Track*,PhysicalNote,int,int)));
    connect(track,SIGNAL(noteRemoved(Track*,PhysicalNote,int,int)),this,SLOT(TrackNoteRemoved(Track*,PhysicalNote,int,int)));

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

void Composition::TrackNoteAdded(Track *track, const PhysicalNote &note, int startMs, int endMs)
{
    qDebug()<<"Added note at "<<startMs<<" till "<<endMs;
}

void Composition::TrackNoteRemoved(Track *track, const PhysicalNote &note, int startMs, int endMs)
{
    qDebug()<<"Removed note at "<<startMs<<" till "<<endMs;
}
