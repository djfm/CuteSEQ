#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QBrush>

class ChunkScene;

class Cell : public QGraphicsRectItem
{

    ChunkScene *_scene;

    int _row, _col;
    QString _note_name;
    QColor  _bg;


public:
    Cell(ChunkScene *scene, int row, int col, int x, int y, int w, int h, const QString & note_name, QColor bg = QColor(255,255,255));

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // CELL_H
