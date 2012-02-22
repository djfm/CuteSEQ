#include "cell.h"
#include "chunkscene.h"

#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QtGui>

Cell::Cell(ChunkScene *scene, int row, int col, int x, int y, int w, int h, const QString &note_name, QColor bg)
    :QGraphicsRectItem(x,y,w,h), _scene(scene), _row(row), _col(col), _note_name(note_name), _bg(bg)
{
    setPen(QPen(QColor(210,210,210)));
    setBrush(QBrush(_bg));
    setAcceptsHoverEvents(true);
}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)

    setBrush(QBrush(QColor(200,200,200)));
    setToolTip(_note_name);
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)

    setBrush(QBrush(_bg));
    setToolTip("");

}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        _scene->startDrag(QPoint(_row,_col));
    }
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    const QString & n = _scene->noteName(_row);
    if(lod >= 1 or  (n == "C" or (n == "E" and lod >= 0.6) or (n == "G" and lod >= 0.5)))
    {
        QGraphicsRectItem::paint(painter,option,widget);
    }
}
