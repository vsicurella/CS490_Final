#ifndef AUDIO_H
#define AUDIO_H

#include <QtCore>
#include <QDebug>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include "synth.h"

class Audio : public QObject
{
    Q_OBJECT

public:

    Audio(int sr);
    ~Audio();

    const char* device;
    float* buffer;

    snd_pcm_t* handle;
    snd_pcm_sframes_t frames;

    Synth synth;

    bool initialized = false;
    int state;

    void init();

    void addToBuffer(float* newBuffer);
    void resetBuffer();
    void playBuffer();

    void pause();
    void resume();

    void setTone(int waveCode);
    void sendFreq(float freq);

signals:

public slots:
    void run();


private:
    int SAMPLE_RATE = 44100;
    int CHUNK_SIZE = 256;

};

#endif // AUDIO_H
