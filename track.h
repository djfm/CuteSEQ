#ifndef TRACK_H
#define TRACK_H

#include <set>
#include <QObject>

#include "solfege.h"

class Chunk;
class ChunkNote;
class CompositionScene;
class Composition;

class Track : public QObject
{
    Q_OBJECT

    friend class Chunk;

    Composition *_composition;
    Chunk * _chunk = 0;
    std::set<int> _marks;

    void setChunk(Chunk * chunk);



public:
    Track(Composition * composition, QObject *parent = 0);

    Chunk * chunk();
    const std::set<int> & marks() const;
    void mark(int measure);
    void unmark(int measure);

    void reFit();

    int msPosition(int mark);
    Composition *composition();

    void emitAdds();
    void emitAdds(int mark);
    void emitRemoves(int mark);

signals:

    void noteAdded(Track * track, const PhysicalNote & note, int startMs, int endMs);
    void noteRemoved(Track * track, const PhysicalNote & note, int startMs, int endMs);

private slots:

    void chunkNoteAdded(Chunk * c, const ChunkNote & note);
    void chunkNoteRemoved(Chunk * c, const ChunkNote & note);

};

#endif // TRACK_H
