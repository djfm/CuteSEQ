#ifndef CHUNKSCENE_H
#define CHUNKSCENE_H

#include <QGraphicsScene>
#include <QList>

#include "util.h"

#include "solfege.h"
//#include "chunk.h"

class Chunk;
class ChunkNote;
class Cell;
class Range;
class PosBar;

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
    PosBar *_pos_bar;

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

    //void mark_view(int row, int from_column, int to_column);
    Range * mark_view(const ChunkNote & note);
    void unmark_view(Range * range);

    void mark_model(int row, int from_column, int to_column);
    void unmark_model(const ChunkNote & note);

    void moveBarToMs(int ms);

    Chunk * chunk();

signals:
    
public slots:
    
};

#endif // SCENE_H
