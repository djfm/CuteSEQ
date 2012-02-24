#ifndef TIMINGDESCRIPTIONEDITOR_H
#define TIMINGDESCRIPTIONEDITOR_H

#include <QDialog>

class TimingDescription;

namespace Ui {
class TimingDescriptionEditor;
}

class TimingDescriptionEditor : public QDialog
{
    Q_OBJECT
    
    TimingDescription *_timing_description;

    bool _ok = false;

public:
    explicit TimingDescriptionEditor(TimingDescription *timing_description, QWidget *parent = 0);
    ~TimingDescriptionEditor();
    bool ok() const;

private slots:

    void onAccept();

private:
    Ui::TimingDescriptionEditor *ui;
};

#endif // TIMINGDESCRIPTIONEDITOR_H
