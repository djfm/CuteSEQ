#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <deque>
#include <QObject>

#include "track.h"
#include "solfege.h"
#include "events.h"
#include "jackpp.h"
#include "alsapp.h"


extern JackPP jack;

class Composition : public QObject
{

    Q_OBJECT

    friend class Track;
    friend class CompositionScene;

    int _measure_duration = 4000;
    CompositionScene *_composition_scene = 0;

    EventQueue * _queue;

    std::deque<Track*> _tracks;

    void addTrack(Track *track);

public:

    Composition(QObject * parent = 0);

    std::deque<Track*> & tracks();

    int measureDuration() const;
    CompositionScene * compositionScene(bool create = true);
    void recreateScene();

    void play();
    void pause();
    void stop();

    EventQueue * queue();

private slots:

    void TrackNoteAdded(Track * track, const PhysicalNote & note, int startMs, int endMs);
    void TrackNoteRemoved(Track * track, const PhysicalNote & note, int startMs, int endMs);
    void onQueueEventSent(Event * event);
};

#endif // COMPOSITION_H
