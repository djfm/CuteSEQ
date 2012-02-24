#include "chunkviewcontrol.h"


#include "chunk.h"
#include "compositionscene.h"
#include "timingdescriptioneditor.h"
#include "solfege.h"

#include <QDrag>
#include <QWheelEvent>
#include <QInputDialog>

ChunkViewControl::ChunkViewControl(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setAcceptDrops(true);
}

ChunkViewControl::~ChunkViewControl()
{

}

void ChunkViewControl::setChunk(Chunk *chunk)
{
    _chunk = chunk;
    graphicsView->setScene(chunk->scene());
    connect(_chunk,SIGNAL(descriptionChanged()),this,SLOT(updateFromChunk()));
    updateFromChunk();
}

void ChunkViewControl::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->text() == "Chunk")
    {
        qDebug("Drag Enters!");
        event->accept();
    }

}

void ChunkViewControl::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->text() == "Chunk")
    {
        event->accept();
    }
}

void ChunkViewControl::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->text() == "Chunk")
    {
        qDebug("Drop!!");

        const ChunkMime * chunk = static_cast<const ChunkMime*>(event->mimeData());

        setChunk(chunk->chunk());

        event->accept();
    }
}

void ChunkViewControl::wheelEvent(QWheelEvent *wheel)
{
    if((QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        double zoom_factor;
        if(wheel->delta() > 0)zoom_factor = 0.9;
        else zoom_factor = 1.1;

       graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
       graphicsView->scale(zoom_factor,zoom_factor);
    }
}

void ChunkViewControl::updateFromChunk()
{
    nameLabel->setText(_chunk->name());
}

void ChunkViewControl::on_nameButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         "Please enter a new name for " + _chunk->name(),
                                         "New name:",
                                         QLineEdit::Normal,
                                         "",
                                         &ok);
    if(ok)_chunk->setName(text);
}

void ChunkViewControl::on_addHighOctaveButton_clicked()
{
    _chunk->addHighOctave();
}

void ChunkViewControl::on_addLowOctaveButton_clicked()
{
    _chunk->addLowOctave();
}

void ChunkViewControl::on_addMeasureButton_clicked()
{
    _chunk->addMeasure();
}

void ChunkViewControl::on_removeMeasureButton_clicked()
{
    _chunk->removeMeasure();
}

void ChunkViewControl::on_timingButton_clicked()
{
    TimingDescription desc = _chunk->timingDescription();
    TimingDescriptionEditor editor(&desc);
    editor.exec();
    if(editor.ok())
    {
        _chunk->setTimingDescription(desc);
    }

}
