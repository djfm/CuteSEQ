#include "solfege.h"

Note::Note(const QString &name, int octave) : _octave(octave)
{
    for(int i = 0; i < 12; i++)if(NoteNames[i] == name)_id = i;
}

Note::Note(int id, int octave) : _id(id), _octave(octave)
{
}

int Note::midi() const
{
    return abs()+12;
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

int Note::abs() const
{
    return 12*_octave + _id;
}

bool Note::operator ==(const Note &other) const
{
    return other.id() == id() and other.octave() == octave();
}

bool Note::operator !=(const Note &other) const
{
    return not operator ==(other);
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

int TimingDescription::ms(int mark, int pos, int measure_duration) const
{
    return (mark + (float)pos / columns()) * measure_duration;
}


int TimingDescription::measure(int pos) const
{
    return pos / columns(1);
}

int TimingDescription::beat(int pos) const
{
    return (pos % columns(1)) / notesPerBeat();
}

int TimingDescription::note(int pos) const
{
    return (pos % columns(1)) % notesPerBeat();
}

int PhysicalNote::velocity() const
{
    return _velocity;
}

void PhysicalNote::setVelocity(int velocity)
{
    _velocity = velocity;
}

bool PhysicalNote::operator ==(const PhysicalNote &other) const
{
    return Note::operator ==((const Note &)other) and velocity() == other.velocity();
}

bool PhysicalNote::operator !=(const PhysicalNote &other) const
{
    return not operator==(other);
}


PhysicalNote::PhysicalNote(const QString &name, int octave)
    :Note(name,octave)
{
}

PhysicalNote::PhysicalNote(int id, int octave)
    :Note(id, octave)
{
}



