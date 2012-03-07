#include "posbar.h"

#include <QPen>
#include <QBrush>

PosBar::PosBar() :
    QGraphicsRectItem()
{
    QBrush brush(QColor(0,0,255,127));
    setBrush(brush);
}
