#include "range.h"
#include "chunkscene.h"
#include "chunk.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
/*
Range::Range(ChunkScene * scene, int row, int col_from, int col_to, const QRectF & rect):
    QGraphicsRectItem(rect),
    _scene(scene),
    _row(row),
    _col_from(col_from),
    _col_to(col_to)
{
    QBrush brush(QColor(0,255,0,127));
    QPen   pen;

    pen.setColor(Qt::blue);
    pen.setWidth(2);

    setBrush(brush);
    setPen(pen);

    setFlags(QGraphicsItem::ItemIsMovable
             | QGraphicsItem::ItemIsSelectable);

    setAcceptsHoverEvents(true);

}*/

Range::Range(ChunkScene *scene, const ChunkNote &note)
    :_scene(scene),_note(note)
{
    qDebug()<<"Created note on view "<<_note;

    QBrush brush(QColor(0,255,0,127));
    QPen   pen;

    pen.setColor(Qt::blue);
    pen.setWidth(2);

    setBrush(brush);
    setPen(pen);

    setFlags(QGraphicsItem::ItemIsMovable
             | QGraphicsItem::ItemIsSelectable);

    setAcceptsHoverEvents(true);

    int the_row = _scene->row(note.id(),note.octave());
    int from_column = note.startPos();
    int to_column   = note.endPos();

    QRectF rect = QRectF(_scene->topLeft(QPoint(the_row, from_column)),
                         _scene->bottomRight(QPoint(the_row,to_column))
                         );

    setRect(rect);


}

void Range::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug()<<"Wanna delete range "<<_note;
        _scene->unmark_model(_note);
    }
    else
    {
        //qDebug()<<"Wanna move range "<<_row<<" "<<_col_from<<" "<<_col_to;
        QGraphicsRectItem::mousePressEvent(event);
    }
}

const ChunkNote &Range::note()
{
    return _note;
}
