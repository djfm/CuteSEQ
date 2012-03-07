#include "chronometre.h"

Chronometre::Chronometre(int interval, QObject *parent) :
    QObject(parent),
    _interval(interval)
{

}

void Chronometre::start()
{
    _time.start();
    _status = Running;

    connect(&_timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    _timer.setInterval(_interval);
    _timer.start();

}

void Chronometre::pause()
{
    _base   = elapsed();
    _status = Paused;

    disconnect(&_timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
}

void Chronometre::stop()
{
    _base = 0;
    _status = Stopped;

    disconnect(&_timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    emit tick(0);
}

int Chronometre::elapsed() const
{
    if(_status == Running)
        return _base + _time.elapsed();
    else return _base;
}

bool Chronometre::running() const
{
    return _status == Running;
}

void Chronometre::onTimerTimeout()
{
    emit tick(elapsed());
}
