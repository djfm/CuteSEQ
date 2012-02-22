#ifndef CHUNKVIEW_H
#define CHUNKVIEW_H

#include <QGraphicsView>

class ChunkView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChunkView(QWidget *parent = 0);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *evevt);

signals:
    
public slots:
    
};

#endif // CHUNKVIEW_H
