#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CompositionScene;
class Composition;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Composition * _composition;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject *, QEvent *);
    
public slots:

    void on_addTrackButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
