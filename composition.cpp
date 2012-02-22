#include "composition.h"

Composition::Composition()
{
}

void Composition::addTrack(Track *track)
{
    _tracks.push_back(track);
}

std::deque<Track*> &Composition::tracks()
{
    return _tracks;
}
