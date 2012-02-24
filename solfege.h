#ifndef SOLFEGE_H
#define SOLFEGE_H

#include <QString>
#include <map>

const QString NoteNames[12] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

class Note
{
    int _id;
    int _octave;

public:

    Note(const QString & name, int octave = 4);
    Note(int id, int octave = 4);

    int id() const;
    int octave() const;
    const QString & name() const;
    int abs() const;

    bool operator==(const Note & other) const;
    bool operator!=(const Note & other) const;

};

class PhysicalNote : public Note
{
protected:

    int _velocity;

public:

    PhysicalNote(const QString & name, int octave = 4);
    PhysicalNote(int id, int octave = 4);

    int velocity() const;
    void setVelocity(int velocity);

    bool operator==(const PhysicalNote & other) const;
    bool operator!=(const PhysicalNote & other) const;

};

class TimingDescription
{
    int _beats_per_measure;
    int _notes_per_beat;

public:

    TimingDescription(int beats_per_measure = 4, int notes_per_beat = 4);

    void setBeatsPerMeasure(int beats_per_measure);
    void setNotesPerBeat(int notes_per_beat);

    int beatsPerMeasure() const;
    int notesPerBeat() const;

    int measure(int pos) const;
    int beat(int pos) const;
    int note(int pos) const;

    int columns(int measures = 1) const;

    int ms (int pos, int measure_duration) const;
    int pos(int ms , int measure_duration) const;


};

#endif // SOLFEGE_H
