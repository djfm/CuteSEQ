#ifndef SOLFEGE_H
#define SOLFEGE_H

#include <QString>

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

    int columns(int measures = 1) const;


};

#endif // SOLFEGE_H
