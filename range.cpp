#include "range.h"
#include "chunkscene.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

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

}

void Range::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        qDebug()<<"Wanna delete range "<<_row<<" "<<_col_from<<" "<<_col_to;
        _scene->eraseRange(this);
    }
    else
    {
        qDebug()<<"Wanna move range "<<_row<<" "<<_col_from<<" "<<_col_to;
        QGraphicsRectItem::mousePressEvent(event);
    }
}
