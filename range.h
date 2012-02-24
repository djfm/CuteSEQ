#ifndef RANGE_H
#define RANGE_H

#include <QGraphicsRectItem>

#include "chunk.h"

class ChunkScene;


class Range : public QGraphicsRectItem
{
    ChunkScene * _scene;
    ChunkNote     _note;

    /*
    int _row;
    int _col_from, _col_to;*/

public:
    //explicit Range(ChunkScene * scene, int row, int col_from, int col_to, const QRectF & rect);
    
    explicit Range(ChunkScene * scene, const ChunkNote & note);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    const ChunkNote & note();

signals:
    
public slots:
    
};

#endif // RANGE_H
