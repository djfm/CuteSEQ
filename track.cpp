#include "track.h"
#include "chunk.h"

#include "composition.h"
#include "compositionscene.h"

#include <QGraphicsView>
#include <QDebug>

Track::Track(Composition *composition, QObject *parent)
    :QObject(parent), _composition(composition)
{
    _composition->addTrack(this);
}

void Track::setChunk(Chunk *chunk)
{
    _chunk = chunk;
    connect(_chunk,SIGNAL(noteAdded(Chunk*,ChunkNote)),this, SLOT(chunkNoteAdded(Chunk*,ChunkNote)));
    connect(_chunk,SIGNAL(noteRemoved(Chunk*,ChunkNote)),this, SLOT(chunkNoteRemoved(Chunk*,ChunkNote)));
    _chunk->emitAdds();
}

void Track::chunkNoteAdded(Chunk * c, const ChunkNote &note)
{
    for(int m : _marks)
    {
        int startMs = c->msPosition(m,note.startPos());
        int endMs   = c->msPosition(m,note.endPos()+1);
        emit noteAdded(this,note,startMs,endMs);
    }
}

void Track::chunkNoteRemoved(Chunk *c, const ChunkNote &note)
{
    for(int m : _marks)
    {
        int startMs = c->msPosition(m,note.startPos());
        int endMs   = c->msPosition(m,note.endPos()+1);
        emit noteRemoved(this,note,startMs,endMs);
    }
}

Chunk *Track::chunk()
{
    return _chunk;
}

const std::set<int> &Track::marks() const
{
    return _marks;
}

void Track::mark(int measure)
{
    qDebug()<<"Marked measure "<<measure;
    _marks.insert(measure);
    if(_composition->compositionScene(false))
    {
        _composition->compositionScene()->mark_view(this,measure);
    }

    emitAdds(measure);
}

void Track::unmark(int measure)
{
    _marks.erase(measure);
    //FIXME : will not update view.
    emitRemoves(measure);
}

void Track::reFit()
{
    std::set<int> new_marks;

    for(int m : _marks)
    {
        if(new_marks.empty())new_marks.insert(m);
        else
        {
            int maxPos = *(new_marks.rbegin())+_chunk->measures();
            if( m >= maxPos)new_marks.insert(m);
            else new_marks.insert(maxPos);
        }
    }

    _marks = new_marks;
    _composition->recreateScene();


}

int Track::msPosition(int mark)
{
    return _composition->measureDuration()*_chunk->measures()*mark;
}

Composition *Track::composition()
{
    return _composition;
}

void Track::emitAdds()
{
    if(_chunk != 0)
    {
        _chunk->emitAdds();
    }
}

void Track::emitAdds(int mark)
{
    qDebug()<<"Emitting adds for mark "<<mark;
    for(const auto & p : _chunk->notes())
    {
        int startMs = _chunk->msPosition(mark/**_chunk->measures()*/,p.first.startPos());
        int endMs   = _chunk->msPosition(mark/**_chunk->measures()*/,p.first.endPos()+1);
        emit noteAdded(this,p.first,startMs,endMs);
    }
}

void Track::emitRemoves(int mark)
{
    qDebug()<<"Emitting removes for mark "<<mark;
    for(const auto & p : _chunk->notes())
    {
        int startMs = _chunk->msPosition(mark/**_chunk->measures()*/,p.first.startPos());
        int endMs   = _chunk->msPosition(mark/**_chunk->measures()*/,p.first.endPos()+1);
        emit noteRemoved(this,p.first,startMs,endMs);
    }
}
