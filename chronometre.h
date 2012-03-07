#ifndef CHRONOMETRE_H
#define CHRONOMETRE_H

#include <QTime>
#include <QTimer>

class Chronometre : public QObject
{
    Q_OBJECT

    enum Status
    {
        Stopped,
        Running,
        Paused
    };

    Status _status = Stopped;

    int _base = 0;
    int _interval;

    QTime   _time;
    QTimer _timer;

public:
    Chronometre(int interval, QObject * parent = 0);

    void start();
    void pause();
    void stop();

    int elapsed() const;

    bool running() const;

private slots:

    void onTimerTimeout();

signals:

    void tick(int elapsed);


};

#endif // CHRONOMETRE_H
