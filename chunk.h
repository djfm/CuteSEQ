#ifndef CHUNK_H
#define CHUNK_H

#include "solfege.h"
#include "chunkscene.h"

#include <map>
#include <QObject>

class Track;
class Range;


class ChunkNote : public PhysicalNote
{
    int _start_pos, _end_pos;

public:

    ChunkNote(const QString & name, int start_pos, int end_pos, int velocity = 100, int octave = 4);
    ChunkNote(int id, int start_pos, int end_pos, int velocity = 100, int octave = 4);

    int startPos() const;
    int endPos() const;

    struct Cmp
    {
        bool operator()(const ChunkNote & a, const ChunkNote & b) const;
    };

    void setPos(int start, int end);

};

QDebug operator<<(QDebug dbg, const ChunkNote & note);

typedef std::multimap<ChunkNote, Range*, ChunkNote::Cmp> ChunkNotes;

class Chunk : public QObject
{
    Q_OBJECT

    friend class ChunkScene;

public:


    Chunk(Track * track, int measures = 4, const TimingDescription & timing_description = TimingDescription(), QObject * parent = 0);
    virtual ~Chunk();

    void setScene(ChunkScene * scene);

    int measures() const;
    const TimingDescription & timingDescription() const;

    void addNote(const ChunkNote &note);
    void removeNote(const ChunkNote & note);

    const ChunkNotes & notes() const;
    ChunkNotes & notes();

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

    int msPosition(int mark, int pos) const;

    void setTimingDescription(const TimingDescription & timing_description);

    void emitAdds();

    Track * track();

signals:

    void descriptionChanged();
    void noteAdded(Chunk * chunk, const ChunkNote & note);
    void noteRemoved(Chunk * chunk, const ChunkNote & note);

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
