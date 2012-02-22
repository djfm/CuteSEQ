#include "chunk.h"
#include "track.h"

#include <QGraphicsView>


int Chunk::_chunks_count = 0;

int Chunk::ChunkNote::startPos() const
{
    return _start_pos;
}

int Chunk::ChunkNote::endPos() const
{
    return _end_pos;
}

int Chunk::ChunkNote::velocity() const
{
    return _velocity;
}

Chunk::Chunk(Track *track, int measures, const TimingDescription &timing_description, QObject * parent)
    :QObject(parent),_track(track),_measures(measures), _timing_description(timing_description)
{
    ++_chunks_count;
    _name = "Chunk_" + QString::number(_chunks_count);
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

void Chunk::addNote(const Chunk::ChunkNote &note, Direction dir)
{
    if(_lower_octave == -1 or note.octave() < _lower_octave) _lower_octave = note.octave();
    if(_upper_octave == -1 or note.octave() > _upper_octave) _upper_octave = note.octave();
    _notes.insert(note);

    if(dir == ModelToView and _scene != 0)
    {
        _scene->mark(_scene->row(note.id(),note.octave()),note.startPos(),note.endPos(),dir);
    }

}

const Chunk::ChunkNotes &Chunk::notes() const
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

Chunk::ChunkNote::ChunkNote(const QString &name, int start_pos, int end_pos, int velocity, int octave)
    :Note(name,octave),
      _start_pos(start_pos),
      _end_pos(end_pos),
      _velocity(velocity)
{
}

Chunk::ChunkNote::ChunkNote(int id, int start_pos, int end_pos, int velocity, int octave)
    :Note(id,octave),
      _start_pos(start_pos),
      _end_pos(end_pos),
      _velocity(velocity)
{
}

bool Chunk::ChunkNote::Cmp::operator ()(const Chunk::ChunkNote &a, const Chunk::ChunkNote &b) const
{
    return a.startPos() < b.startPos();
}




