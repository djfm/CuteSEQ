#include "compositionscene.h"
#include "composition.h"
#include "chunk.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QtCore>

class ChunkHandle : public QGraphicsRectItem
{
    Chunk * _chunk;
public:

    ChunkHandle(Chunk * chunk, int x, int y, int w, int h)
        :QGraphicsRectItem(x,y,w,h), _chunk(chunk)
    {
        QBrush brush(QColor(127,127,255));
        setBrush(brush);
        /*
        QPen pen;
        pen.setWidth(2);
        setPen(pen);*/

    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        {
            QDrag * drag = new QDrag(event->widget());
            drag->setMimeData(new ChunkMime(_chunk));
            Qt::DropAction drop = drag->exec();
        }
    }

};

class MarkHandle : public QGraphicsRectItem
{
    CompositionScene  * _comp;
    Track * _track;
    int _column;
public:
    MarkHandle(CompositionScene * comp, Track * track, int column, int x, int y, int w, int h)
        :QGraphicsRectItem(x,y,w,h), _comp(comp), _track(track), _column(column)
    {

    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        {
            qDebug()<<"Clicked on "<<_column;
            _comp->mark(_track,_column,ViewToModel);
        }
    }

};

class Mark : public QGraphicsRectItem
{
    CompositionScene  * _comp;
    Track * _track;
    int _measure;
public:
    Mark(CompositionScene * comp, Track * track, int measure, int x, int y, int w, int h)
        :QGraphicsRectItem(x,y,w,h), _comp(comp), _track(track), _measure(measure)
    {
        QBrush brush(QColor(255,127,127));
        setBrush(brush);
        /*
        QPen pen;
        pen.setWidth(2);
        setPen(pen);*/
    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->button() == Qt::RightButton)
        {
            _comp->unmark(_track,_measure,ViewToModel);
            _comp->removeItem(this);
        }
    }
};

CompositionScene::CompositionScene(Composition *composition, QObject *parent) :
    QGraphicsScene(parent), _composition(composition)
{
    for(unsigned int i = 0; i < _composition->tracks().size(); ++i)
    {
        addTrack(_composition->tracks().at(i),ModelToView);
    }
}

void CompositionScene::addTrack(Track *track, Direction dir)
{
    int i;

    if(dir == ModelToView)
    {
        i = trackRow(track);
    }
    else
    {
        _composition->addTrack(track);
        i = _composition->tracks().size() - 1;
    }

    addItem(new ChunkHandle(track->chunk(),0,i*_edge_size,_handle_width,_edge_size));

    for(int j = 0; j < _columns; ++j)
    {
        addItem(new MarkHandle(this,track,j,_handle_width+j*_edge_size,i*_edge_size,_edge_size,_edge_size));
    }

    for(std::set<int>::const_iterator m = track->marks().begin(); m != track->marks().end(); ++m)
    {
        mark(track,*m,ModelToView);
    }

}

int CompositionScene::trackRow(Track *track) const
{
    for(unsigned int i = 0; i < _composition->tracks().size(); ++i)
    {
        if(_composition->tracks().at(i) == track)
        {
            return i;
        }
    }
    return -1;
}

void CompositionScene::mark(Track *track, int measure, Direction dir)
{
    int i = trackRow(track);
    addItem(new Mark(this,track,measure,_handle_width+measure*_edge_size,i*_edge_size,_edge_size*track->chunk()->measures(),_edge_size));
    if(dir == ViewToModel)
    {
        track->mark(measure);
    }
}

void CompositionScene::unmark(Track *track, int measure, Direction dir)
{
    if(dir == ViewToModel)
    {
        track->unmark(measure);
    }
}

Composition *CompositionScene::composition()
{
    return _composition;
}
