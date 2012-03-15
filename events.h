#ifndef EVENTS_H
#define EVENTS_H

#include "solfege.h"
#include "chronometre.h"

#include <map>
#include <QObject>
#include <QTimer>

class Composition;

class Event
{
public:
    enum Type
    {
        NoteOn,
        NoteOff
    };

    Event(int time, Type type);

    int  time() const;
    Type type() const;

    virtual ~Event();

private:


    int  _time;
    Type _type;
};

class EventQueue :  public QObject
{
    Q_OBJECT

    std::multimap<int, Event *> _queue;

    int _current_pos = 0;

    QTimer * _timer = 0;

    Chronometre * _chronometre;

public:

    EventQueue(QObject * parent = 0);

    void add(Event * event);
    void remove(Event * event, bool andDelete = false);

    void show();


    void pause();
    void stop();

    int elapsed();

    Chronometre * chronometre();

public slots:

    void play();

signals:

    void eventSent(Event * event);


};

bool same(const Event * a, const Event * b);

class NoteEvent : public Event
{

public:

    NoteEvent(Type type, const PhysicalNote & note, int time);
    const PhysicalNote & note() const;

private:

    PhysicalNote _note;
};

#endif // EVENTS_H
