#include <QDebug>

#include "timingdescriptioneditor.h"
#include "ui_timingdescriptioneditor.h"

#include "solfege.h"

TimingDescriptionEditor::TimingDescriptionEditor(TimingDescription * timing_description, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimingDescriptionEditor),
    _timing_description(timing_description)
{
    ui->setupUi(this);
    ui->beatsPerMeasureEdit->setValidator(new QIntValidator());
    ui->notesPerBeatEdit->setValidator(new QIntValidator());

    ui->beatsPerMeasureEdit->setText(QString::number(_timing_description->beatsPerMeasure()));
    ui->notesPerBeatEdit->setText(QString::number(_timing_description->notesPerBeat()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

}

TimingDescriptionEditor::~TimingDescriptionEditor()
{
    delete ui;
}

void TimingDescriptionEditor::onAccept()
{
    _ok = true;
    _timing_description->setNotesPerBeat(ui->notesPerBeatEdit->text().toInt());
    _timing_description->setBeatsPerMeasure(ui->beatsPerMeasureEdit->text().toInt());
}

bool TimingDescriptionEditor::ok() const
{
    return _ok;
}
