#include "chunk.h"
#include "track.h"
#include "composition.h"

#include <QGraphicsView>
#include <QDebug>
#include <QApplication>


int Chunk::_chunks_count = 0;

int ChunkNote::startPos() const
{
    return _start_pos;
}

int ChunkNote::endPos() const
{
    return _end_pos;
}

void ChunkNote::setPos(int start, int end)
{
    qDebug()<<"Pos : "<<start<<" "<<end;
    _start_pos = start;
    _end_pos   = end;
}

Chunk::Chunk(Track *track, int measures, const TimingDescription &timing_description, QObject * parent)
    :QObject(parent),_track(track),_measures(measures), _timing_description(timing_description)
{
    ++_chunks_count;
    _name = "Chunk_" + QString::number(_chunks_count);
    _track->setChunk(this);
}

Chunk::~Chunk()
{
    if(_scene != 0)
    {
        delete _scene;
    }
}

void Chunk::setScene(ChunkScene *scene)
{
    _scene = scene;
}

int Chunk::measures() const
{
    return _measures;
}

const TimingDescription &Chunk::timingDescription() const
{
    return _timing_description;
}

void Chunk::addNote(const ChunkNote &note)
{
    if(_lower_octave == -1 or note.octave() < _lower_octave) _lower_octave = note.octave();
    if(_upper_octave == -1 or note.octave() > _upper_octave) _upper_octave = note.octave();

    Range * range = 0;

    if(_scene != 0)
    {
        range = _scene->mark_view(note);
    }

    _notes.insert(std::make_pair(note,range));

    emit(noteAdded(this,note));

}

void Chunk::removeNote(const ChunkNote &note)
{

    qDebug()<<"Looking for note "<<note;
    qDebug()<<"In :";
    for(auto & n : _notes)
    {
        qDebug()<<n.first;
    }

    if(_scene != 0)
    {
        ChunkNotes::iterator i = _notes.find(note);
        if(i != _notes.end())
        {
            _scene->unmark_view(i->second);
        }
        else
        {
            qDebug("CANNOT FIND NOTE");
        }
    }

    _notes.erase(note);

    emit(noteRemoved(this, note));

}

const ChunkNotes &Chunk::notes() const
{
    return _notes;
}

ChunkNotes & Chunk::notes()
{
    return _notes;
}

int Chunk::lowerOctave() const
{
    return _lower_octave;
}

int Chunk::upperOctave() const
{
    return _upper_octave;
}

int Chunk::defaultLowerOctave() const
{
    if(_notes.empty())return 3;
    else return(_lower_octave);
}

int Chunk::defaultUpperOctave() const
{
    if(_notes.empty())return 5;
    else return(_upper_octave);
}

ChunkScene *Chunk::scene()
{
    if(_scene == 0)_scene = new ChunkScene(this);
    return _scene;
}

const QString &Chunk::name() const
{
    return _name;
}

void Chunk::setName(const QString &str)
{
    _name = str;
    emit descriptionChanged();
}

int Chunk::chunksCount()
{
    return _chunks_count;
}

void Chunk::addHighOctave()
{
    if(_upper_octave == -1)_upper_octave = defaultUpperOctave()+1;
    else ++_upper_octave;

    updateViews();
}

void Chunk::addLowOctave()
{
    if(_lower_octave == -1)_lower_octave = defaultLowerOctave()-1;
    else --_lower_octave;

    updateViews();

}

void Chunk::addMeasure()
{
    ++_measures;
    updateViews();
    _track->reFit();
}

void Chunk::removeMeasure()
{
}

void Chunk::updateViews()
{
    if(_scene != 0)
    {
        ChunkScene * oldScene = _scene;
        ChunkScene * newScene = new ChunkScene(this);

        QList <QGraphicsView*> views = oldScene->views();
        for(QGraphicsView * view : views)
        {
            view->setScene(newScene);
        }

        delete oldScene;
    }
}

int Chunk::msPosition(int mark, int pos) const
{
    return _timing_description.ms(mark*_measures+pos,_track->composition()->measureDuration());
}

void Chunk::setTimingDescription(const TimingDescription &timing_description)
{
    ChunkNotes old_notes = _notes;
    _notes.clear();
    for(const auto & n : old_notes)
    {
        int m_start  = _timing_description.measure(n.first.startPos());
        int b_start  = _timing_description.beat(n.first.startPos());
        int sb_start = _timing_description.note(n.first.startPos());

        int m_dur    = _timing_description.measure(n.first.endPos() - n.first.startPos() + 1);
        int b_dur    = _timing_description.beat(n.first.endPos() - n.first.startPos() + 1);
        int sb_dur   = _timing_description.note(n.first.endPos() - n.first.startPos() + 1);

        ChunkNote nprime = n.first;

        int new_start = m_start*timing_description.columns() + b_start*timingDescription().beatsPerMeasure() + sb_start;
        int new_end   = new_start + timing_description.columns(m_dur) + b_dur * timingDescription().notesPerBeat() + sb_dur - 1;

        nprime.setPos(new_start, new_end);

        removeNote(n.first);
        addNote(nprime);

    }

    _timing_description = timing_description;

    updateViews();
}

void Chunk::emitAdds()
{
    for(const auto & n : _notes)
    {
        emit noteAdded(this,n.first);
    }
}

ChunkNote::ChunkNote(const QString &name, int start_pos, int end_pos, int velocity, int octave)
    :PhysicalNote(name,octave),
      _start_pos(start_pos),
      _end_pos(end_pos)

{
    setVelocity(velocity);
}

ChunkNote::ChunkNote(int id, int start_pos, int end_pos, int velocity, int octave)
    :PhysicalNote(id,octave),
      _start_pos(start_pos),
      _end_pos(end_pos)
{
    setVelocity(velocity);
}

bool ChunkNote::Cmp::operator ()(const ChunkNote &a, const ChunkNote &b) const
{
    if(a.startPos() == b.startPos())return a.abs() < b.abs();
    else return a.startPos() < b.startPos();
}

QDebug operator<<(QDebug dbg, const ChunkNote & note)
{
    return dbg<<"("<<note.startPos()<<"<->"<<note.endPos()<<")";
}



