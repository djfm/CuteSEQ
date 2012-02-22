#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <deque>

#include "track.h"

class Composition
{

    std::deque<Track*> _tracks;

public:

    Composition();

    void addTrack(Track *track);
    std::deque<Track*> & tracks();


};

#endif // COMPOSITION_H
