#include "track.h"
#include "chunk.h"
#include "compositionscene.h"

#include <QGraphicsView>

Track::Track()
{

}

void Track::setChunk(Chunk *chunk)
{
    _chunk = chunk;
}

void Track::setComp(CompositionScene *comp)
{
    _comp = comp;
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
    _marks.insert(measure);
}

void Track::unmark(int measure)
{
    _marks.erase(measure);
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

    if(_comp != 0)
    {
        CompositionScene * old_comp = _comp;
        _comp = new CompositionScene(_comp->composition());

        for(QGraphicsView * view: old_comp->views())
        {
            view->setScene(_comp);
        }

        delete old_comp;
    }

}
