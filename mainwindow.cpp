#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cell.h"
#include "chunkscene.h"
#include "compositionscene.h"
#include "chunk.h"
#include "track.h"
#include "composition.h"

#include <QEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //Scene * scene = new Scene(chunk);//(2,7,16,TimingDescription(),this);

    ui->compositionView->installEventFilter(this);
    ui->compositionView->viewport()->installEventFilter(this);

    _composition = new Composition();

    Track * track = new Track(_composition);

    Chunk * chunk = new Chunk(track, 4);
    chunk->addNote(ChunkNote("C",0,15));
    chunk->addNote(ChunkNote("E",16,31));
    chunk->addNote(ChunkNote("G",32,63));
    chunk->addNote(ChunkNote("C",32,63));

    track->mark(0);

    ui->chunkViewControl->setChunk(chunk);
    ui->chunkViewControl2->setChunk(chunk);

    ui->compositionView->setScene(_composition->compositionScene());

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject * object, QEvent * event)
{
    if(event->type() == QEvent::Wheel && (QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        if(object->metaObject()->className() == QString("QGraphicsView"))
        {
            QGraphicsView * view  = qobject_cast<QGraphicsView*>(object);
            QWheelEvent   * wheel = static_cast <QWheelEvent*>(event);

            double zoom_factor;
            if(wheel->delta() > 0)zoom_factor = 0.9;
            else zoom_factor = 1.1;

            view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            view->scale(zoom_factor,zoom_factor);

            return true;
        }
        else
        {
            event->ignore();
            return true;
        }
    }
    return false;
}

void MainWindow::on_addTrackButton_clicked()
{
    Track * track = new Track(_composition);
    Chunk * chunk = new Chunk(track);
}

void MainWindow::on_playButton_clicked()
{
    _composition->play();
}

void MainWindow::on_pauseButton_clicked()
{
    _composition->pause();
}

void MainWindow::on_stopButton_clicked()
{
    _composition->stop();
}
