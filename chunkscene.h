#ifndef CHUNKSCENE_H
#define CHUNKSCENE_H

#include <QGraphicsScene>
#include <QList>

#include "util.h"
#include "range.h"
#include "solfege.h"

class Chunk;
class Cell;
class Range;

class ChunkScene : public QGraphicsScene
{
    Q_OBJECT

    /*
    QList<Cell*> cells;
    QList<Range*> ranges;*/

    int _lower_octave, _upper_octave, _measures;

    TimingDescription _timing_description;

    int _rows, _columns;

    int _cell_w, _cell_h;

    int _x, _y, _w, _h;

    Maybe<QPoint> from_cell, to_cell;

    QGraphicsItem * temp_item;
    Chunk *_chunk;

    explicit ChunkScene(int lower_octave, int upper_octave, int measures, const TimingDescription & timing_description, QObject *parent = 0);

public:

    explicit ChunkScene(Chunk * chunk, QObject * parent = 0);

    int id(int row) const;
    int octave(int row) const;
    const QString & noteName(int row) const;
    QString octaveStr(int row) const;
    int row(int id, int octaveStr) const;

    Maybe<QPoint> cellCoords(const QPoint & p);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void eraseRange(Range * range);

    void startDrag(const QPoint & pos);

    QPoint topLeft(const QPoint & cell) const;
    QPoint bottomRight(const QPoint & cell) const;

    void mark(int row, int from_column, int to_column, Direction dir);

signals:
    
public slots:
    
};

#endif // SCENE_H
