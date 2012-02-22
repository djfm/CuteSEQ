#ifndef TRACK_H
#define TRACK_H

#include <set>

class Chunk;
class CompositionScene;

class Track
{
    CompositionScene * _comp = 0;
    Chunk * _chunk = 0;
    std::set<int> _marks;
public:
    Track();
    void setChunk(Chunk * chunk);
    void setComp(CompositionScene * comp);
    Chunk * chunk();
    const std::set<int> & marks() const;
    void mark(int measure);
    void unmark(int measure);

    void reFit();

};

#endif // TRACK_H
