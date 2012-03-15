#include "compositionscene.h"
#include "composition.h"
#include "chunk.h"
#include "track.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QtCore>
#include <QGraphicsView>

#include <algorithm>

class ChunkHandle : public QGraphicsRectItem
{
    Track * _track;
public:

    ChunkHandle(Track * track, int x, int y, int w, int h)
        :QGraphicsRectItem(x,y,w,h), _track(track)
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
            drag->setMimeData(new ChunkMime(_track->chunk()));
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
            _track->mark(_column);
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
            _track->unmark(_measure);
            _comp->removeItem(this);
        }
    }
};

CompositionScene::CompositionScene(Composition *composition, QObject *parent) :
    QGraphicsScene(parent), _composition(composition)
{
    for(unsigned int i = 0; i < _composition->tracks().size(); ++i)
    {
        addTrack(_composition->tracks().at(i));
    }

    _posbar = new PosBar();
    _posbar->setRect(_handle_width,0,5,_edge_size * _composition->tracks().size());
    addItem(_posbar);

    connect(_composition->queue()->chronometre(),SIGNAL(tick(int)),this,SLOT(ticked(int)));

}

void CompositionScene::addTrack(Track *track)
{
    int i = trackRow(track);

    addItem(new ChunkHandle(track,0,i*_edge_size,_handle_width,_edge_size));

    for(int j = 0; j < _columns; ++j)
    {
        addItem(new MarkHandle(this,track,j,_handle_width+j*_edge_size,i*_edge_size,_edge_size,_edge_size));
    }

    for(std::set<int>::const_iterator m = track->marks().begin(); m != track->marks().end(); ++m)
    {
        mark_view(track,*m);
    }

    if(_posbar != 0)
    {
        QRectF r = _posbar->rect();
        r.setHeight(r.height()+_edge_size);
        _posbar->setRect(r);
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

void CompositionScene::mark_view(Track *track, int measure)
{
    int i = trackRow(track);
    addItem(new Mark(this,track,measure,_handle_width+measure*_edge_size,i*_edge_size,_edge_size*track->chunk()->measures(),_edge_size));

}

Composition *CompositionScene::composition()
{
    return _composition;
}

void CompositionScene::ticked(int elapsed)
{
    static int last_m = -1;
    static int last_mark = -1;
    float x = ((float)(elapsed * _edge_size)) / _composition->measureDuration();
    //qDebug()<<"Elapsed : "<<elapsed<<" X : "<<x;
    _posbar->setPos(x,0);
    /*for(QGraphicsView *view : views())
    {
        view->update(view->rect());
    }*/
    update(this->sceneRect());
    //qDebug()<<"Head at "<<elapsed;

    for(Track * track : _composition->tracks())
    {
        if(!track->marks().empty())
        {
            int m = elapsed / (_composition->measureDuration());
            std::set<int>::const_iterator i = track->marks().lower_bound(m);

            if(m != last_m)//measure just changed
            {
                last_m = m;
                qDebug()<<"M is "<<m;
                qDebug()<<"Bound is "<<*i;
            }

            auto inMark = [m, track](int pos)
            {
                return m >= pos and m < pos + track->chunk()->measures();
            };

            int mark = -1;
            if(i != track->marks().end())
            {
                if(inMark(*i))mark=*i;
            }
            if(mark == -1 and i != track->marks().begin())
            {
                if(inMark(*--i))
                {
                    mark=*i;
                }
            }

            if(mark != -1)
            {
                int rel_ms = elapsed - mark * _composition->measureDuration();
                if(track->chunk()->scene() != 0)
                {
                    track->chunk()->scene()->moveBarToMs(rel_ms);
                }
            }
        }
    }

}
