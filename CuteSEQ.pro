#-------------------------------------------------
#
# Project created by QtCreator 2012-02-22T10:24:00
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11

QT       += core gui opengl

TARGET = qseq2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    solfege.cpp \
    util.cpp \
    range.cpp \
    chunk.cpp \
    composition.cpp \
    track.cpp \
    chunkview.cpp \
    chunkviewcontrol.cpp \
    compositionscene.cpp \
    chunkscene.cpp \
    timingdescriptioneditor.cpp \
    events.cpp \
    jackpp.cpp \
    midi.cpp \
    alsapp.cpp \
    chronometre.cpp \
    posbar.cpp

HEADERS  += mainwindow.h \
    cell.h \
    solfege.h \
    util.h \
    range.h \
    chunk.h \
    composition.h \
    track.h \
    chunkview.h \
    chunkviewcontrol.h \
    chunkscene.h \
    compositionscene.h \
    timingdescriptioneditor.h \
    events.h \
    jackpp.h \
    midi.h \
    alsapp.h \
    chronometre.h \
    posbar.h

FORMS    += mainwindow.ui \
    chunkviewcontrol.ui \
    timingdescriptioneditor.ui

LIBS += -ljack -lasound
