#ifndef CHUNKVIEWCONTROL_H
#define CHUNKVIEWCONTROL_H

#include <QWidget>

#include "chunkview.h"
#include "ui_chunkviewcontrol.h"

class Chunk;

class ChunkViewControl : public QWidget, private Ui::ChunkViewControl
{
    Q_OBJECT

public:
    explicit ChunkViewControl(QWidget *parent = 0);
    virtual ~ChunkViewControl();
    
    void setChunk(Chunk *chunk);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void wheelEvent(QWheelEvent *wheel);



signals:
    
public slots:

    void updateFromChunk();

private slots:

    void on_nameButton_clicked();
    void on_addHighOctaveButton_clicked();
    void on_addLowOctaveButton_clicked();
    void on_addMeasureButton_clicked();
    void on_removeMeasureButton_clicked();
    void on_timingButton_clicked();

private:
    //Ui::ChunkViewControl *ui;
    Chunk *_chunk;
    
};

#endif // CHUNKVIEWCONTROL_H
