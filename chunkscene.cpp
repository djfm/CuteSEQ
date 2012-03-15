#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>

#include "chunkscene.h"
#include "cell.h"
#include "solfege.h"
#include "range.h"
#include "chunk.h"
#include "posbar.h"
#include "track.h"
#include "composition.h"

#include <QtGui>


class BarLine : public QGraphicsLineItem
{
public:

    BarLine(qreal x1, qreal y1, qreal x2, qreal y2)
        : QGraphicsLineItem(x1,y1,x2,y2)
    {
        QPen pen(QColor(150,150,150));
        pen.setWidth(4);
        setPen(pen);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
        if(lod < 0.7)
        {

        }
        QGraphicsLineItem::paint(painter, option, widget);
    }

};

class BeatLine : public QGraphicsLineItem
{
public:

    BeatLine(qreal x1, qreal y1, qreal x2, qreal y2)
        : QGraphicsLineItem(x1,y1,x2,y2)
    {
        QPen pen(QColor(160,160,160));
        setPen(pen);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
        if(lod > 0.7)
        {
            QGraphicsLineItem::paint(painter, option, widget);
        }
    }

};

ChunkScene::ChunkScene(int lower_octave, int upper_octave, int measures, const TimingDescription & timing_description, QObject *parent) :
    QGraphicsScene(parent),
    _lower_octave(lower_octave),
    _upper_octave(upper_octave),
    _measures(measures),
    _timing_description(timing_description),
    _rows(12*(_upper_octave - _lower_octave + 1)),
    _columns(timing_description.columns(_measures)),
    _cell_w(20),
    _cell_h(12),
    _x(0),
    _y(0),
    _w(_columns*_cell_w),
    _h(_rows*_cell_h),
    temp_item(0),
    _chunk(0)

{

    for(int i = 0; i < _rows; i++)
    {
        int y = _y+i*_cell_h;
        for(int j = 0; j < _columns; j++)
        {
            int x = _x+j*_cell_w;
            QColor col;
            if(noteName(i) == "C")col = QColor(220,220,220);
            else if(noteName(i) == "E")col = QColor(235,235,235);
            else if(noteName(i) == "G")col = QColor(245,245,245);
            else col = QColor(255,255,255);
            Cell * cell = new Cell(this, i,j,x,y,_cell_w,_cell_h, noteName(i)+"("+octaveStr(i)+")", col);
            //cells.push_back(cell);
            addItem(cell);
        }
    }

    for(int j = 0; j <= _columns; j++)
    {
        int x = _x+j*_cell_w;
        if(j % timing_description.columns() == 0)
        {
            BarLine * line = new BarLine(x,_y,x,_y+_h);
            addItem(line);
        }
        else if (j % timing_description.notesPerBeat() == 0)
        {
            BeatLine * line = new BeatLine(x,_y,x,_y+_h);
            addItem(line);
        }
    }

    /*setBspTreeDepth(11);
    qDebug()<<"BSP Tree depth is "<<bspTreeDepth();
    qDebug()<<"Index method is "<<itemIndexMethod();*/

}

ChunkScene::ChunkScene(Chunk *chunk, QObject *parent) :
    ChunkScene(chunk->defaultLowerOctave(),chunk->defaultUpperOctave(),chunk->measures(),chunk->timingDescription(), parent)
{
    _chunk = chunk;
    _chunk->setScene(this);
    for(ChunkNotes::iterator n = chunk->notes().begin(); n != chunk->notes().end(); ++n)
    {
        //mark_view(row(n->id(),n->octave()),n->startPos(), n->endPos());
        n->second = mark_view(n->first);
    }
    _pos_bar = new PosBar();
    QRectF r(0,0,_cell_w/2,_cell_h * _rows);
    _pos_bar->setRect(r);
    addItem(_pos_bar);
}

int ChunkScene::id(int row) const
{
    return (_rows - row - 1)%12;
}

int ChunkScene::octave(int row) const
{
    return _lower_octave + (_rows - row - 1)/12;
}

const QString &ChunkScene::noteName(int row) const
{
    return NoteNames[id(row)];
}

QString ChunkScene::octaveStr(int row) const
{
    return QString::number(octave(row));
}

int ChunkScene::row(int id, int octave) const
{
    return _rows - 1 - (12*(octave - _lower_octave) + id);
}

Maybe<QPoint> ChunkScene::cellCoords(const QPoint &p)
{
    QPoint origin(_x,_y);
    QPoint res = (p - origin);

    int x = res.x()/_cell_w;
    int y = res.y()/_cell_h;
    res.setY(x);
    res.setX(y);

    if(res.x() >= 0 && res.y() >= 0 && res.x() < _rows && res.y() < _columns )return Maybe<QPoint>::Just(res);
    else return Maybe<QPoint>::Nothing();

}

void ChunkScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
    QGraphicsScene::mousePressEvent(event);
}

void ChunkScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(from_cell)
    {

        if(temp_item != 0)
        {
            removeItem(temp_item);
            temp_item = 0;
        }

        if(to_cell)
        {


            mark_model(from_cell.value().x(), from_cell.value().y(),to_cell.value().y());

            to_cell.clear();
        }

        from_cell = Maybe<QPoint>::Nothing();
        event->accept();
    }
    event->ignore();
    QGraphicsScene::mouseReleaseEvent(event);
}

void ChunkScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(from_cell)
    {
        if(Maybe<QPoint> other = cellCoords(event->scenePos().toPoint()))
        {
            if((other.value().y() >= from_cell.value().y()) and (other != to_cell))
            {
                //qDebug()<<"Hovering over "<<other.value();
                to_cell = other;
                int row = from_cell.value().x();
                int col_to   = other.value().y();

                QPoint tl = topLeft(from_cell.value());
                QPoint br = bottomRight(QPoint(row,col_to));

                if(temp_item != 0)removeItem(temp_item);
                QGraphicsRectItem * rect = new QGraphicsRectItem(QRectF(tl,br));

                rect->setBrush(QColor(0,0,255,127));

                temp_item = rect;
                addItem(temp_item);

            }
            event->accept();
            return;
        }
    }

    event->ignore();
    QGraphicsScene::mouseMoveEvent(event);
}

void ChunkScene::eraseRange(Range *range)
{
    removeItem(range);
    //ranges.removeAll(range);
    //_chunk->
}

void ChunkScene::startDrag(const QPoint &pos)
{
    if(!from_cell)
    {
        from_cell = pos;
        qDebug()<<"FROM CELL "<<pos;
    }
}

QPoint ChunkScene::topLeft(const QPoint &cell) const
{
    return QPoint(_x + cell.y() * _cell_w, _y + cell.x() * _cell_h);
}

QPoint ChunkScene::bottomRight(const QPoint &cell) const
{
    return topLeft(cell) + QPoint(_cell_w, _cell_h);
}

Range *ChunkScene::mark_view(const ChunkNote &note)
{

    Range * range = new Range(this, note);
    //ranges.push_back(range);
    addItem(range);
    return range;
}

void ChunkScene::unmark_view(Range *range)
{
    removeItem(range);
}

void ChunkScene::mark_model(int row, int from_column, int to_column)
{
    _chunk->addNote(ChunkNote(id(row),from_column,to_column,100,octave(row)));
}

void ChunkScene::unmark_model(const ChunkNote &note)
{
    _chunk->removeNote(note);
}

void ChunkScene::moveBarToMs(int ms)
{
    float percent = ((float)(ms))/((float)(_chunk->measures() * _chunk->track()->composition()->measureDuration()));
    //qDebug()<<"Percent : "<<percent;
    float x = percent * _w;
    _pos_bar->setPos(x,0);
}

Chunk *ChunkScene::chunk()
{
    return _chunk;
}
