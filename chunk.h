#ifndef CHUNK_H
#define CHUNK_H

#include "solfege.h"
#include "chunkscene.h"

#include <set>
#include <QObject>

class Track;

class Chunk : public QObject
{
    Q_OBJECT

public:

    class ChunkNote : public Note
    {
        int _start_pos, _end_pos;
        int _velocity;

    public:

        ChunkNote(const QString & name, int start_pos, int end_pos, int velocity = 100, int octave = 4);
        ChunkNote(int id, int start_pos, int end_pos, int velocity = 100, int octave = 4);

        int startPos() const;
        int endPos() const;
        int velocity() const;

        struct Cmp
        {
            bool operator()(const ChunkNote & a, const ChunkNote & b) const;
        };

    };

    typedef std::set<ChunkNote, ChunkNote::Cmp> ChunkNotes;

    Chunk(Track * track, int measures = 4, const TimingDescription & timing_description = TimingDescription(), QObject * parent = 0);
    virtual ~Chunk();

    void setScene(ChunkScene * scene);

    int measures() const;
    const TimingDescription & timingDescription() const;

    void addNote(const ChunkNote & note, Direction dir = ModelOnly);

    const ChunkNotes & notes() const;

    int lowerOctave() const;
    int upperOctave() const;

    int defaultLowerOctave() const;
    int defaultUpperOctave() const;

    ChunkScene * scene();

    const QString & name() const;
    void  setName(const QString & str);

    static int chunksCount();

    void addHighOctave();
    void addLowOctave();

    void addMeasure();
    void removeMeasure();

    void updateViews();


signals:

    void descriptionChanged();

private:
    Track * _track = 0;
    int _measures;
    TimingDescription _timing_description;
    ChunkNotes _notes;
    int _lower_octave = -1, _upper_octave = -1;
    ChunkScene * _scene = 0;

    QString _name;

    static int _chunks_count;

};

#endif // CHUNK_H
