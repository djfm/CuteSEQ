#ifndef RANGE_H
#define RANGE_H

#include <QGraphicsRectItem>

class ChunkScene;

class Range : public QGraphicsRectItem
{
    ChunkScene * _scene;

    int _row;
    int _col_from, _col_to;

public:
    explicit Range(ChunkScene * scene, int row, int col_from, int col_to, const QRectF & rect);
    
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    
public slots:
    
};

#endif // RANGE_H
