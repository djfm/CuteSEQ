#include "chunkview.h"

#include <QDebug>
#include <QWheelEvent>
#include <QApplication>
#include <QGLWidget>

#include "chunk.h"
#include "compositionscene.h"

ChunkView::ChunkView(QWidget *parent) :
    QGraphicsView(parent)
{
    setupViewport(new QGLWidget());
    qDebug()<<"Created ChunkView!";
    setAcceptDrops(true);

}

void ChunkView::dragEnterEvent(QDragEnterEvent *event)
{
}

void ChunkView::dragMoveEvent(QDragMoveEvent *event)
{
}

void ChunkView::dropEvent(QDropEvent *evevt)
{
}
