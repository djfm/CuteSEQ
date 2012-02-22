#include "solfege.h"

Note::Note(const QString &name, int octave) : _octave(octave)
{
    for(int i = 0; i < 12; i++)if(NoteNames[i] == name)_id = i;
}

Note::Note(int id, int octave) : _id(id), _octave(octave)
{
}

int Note::id() const
{
    return _id;
}

int Note::octave() const
{
    return _octave;
}

const QString &Note::name() const
{
    return NoteNames[_id];
}

TimingDescription::TimingDescription(int beats_per_measure, int notes_per_beat)
{
    setBeatsPerMeasure(beats_per_measure);
    setNotesPerBeat(notes_per_beat);
}

void TimingDescription::setBeatsPerMeasure(int beats_per_measure)
{
    _beats_per_measure = beats_per_measure;
}

void TimingDescription::setNotesPerBeat(int notes_per_beat)
{
    _notes_per_beat = notes_per_beat;
}

int TimingDescription::beatsPerMeasure() const
{
    return _beats_per_measure;
}

int TimingDescription::notesPerBeat() const
{
    return _notes_per_beat;
}

int TimingDescription::columns(int measures) const
{
    return _notes_per_beat*_beats_per_measure*measures;
}


