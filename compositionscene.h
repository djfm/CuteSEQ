#ifndef COMPOSITIONSCENE_H
#define COMPOSITIONSCENE_H

#include <QGraphicsScene>
#include <QMimeData>

#include "util.h"
#include "posbar.h"

class Composition;

class Track;
class Chunk;

class ChunkMime : public QMimeData
{
    Chunk * _chunk;
public:
    inline ChunkMime(Chunk * chunk)
        :QMimeData(),_chunk(chunk)
    {
        setText("Chunk");
    }
    inline Chunk *chunk() const
    {
        return _chunk;
    }
};

class CompositionScene : public QGraphicsScene
{
    Q_OBJECT

    Composition *_composition;

    int _edge_size = 20;
    int _handle_width = 50;
    int _columns = 100;

    PosBar * _posbar = 0;

public:
    explicit CompositionScene(Composition * composition, QObject *parent = 0);

    void addTrack(Track *track);
    int trackRow(Track *track) const;

    void mark_view(Track * track, int measure);

    Composition *composition();

signals:
    
public slots:

    void ticked(int elapsed);
    
};

#endif // COMP_H
