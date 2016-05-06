#ifndef AUDIO_H
#define AUDIO_H

#include <QtCore>
#include <QDebug>
#include <QMainWindow>

#include "opencv2/core/core.hpp"

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include "synth.h"
#include "translator.h"

class Audio : public QObject
{
    Q_OBJECT

public:

    Audio(int sr, int cs);
    ~Audio();

    const char* device;
    float* buffer;
    std::vector<float*> buffertest;

    snd_pcm_t* handle;
    snd_pcm_sframes_t frames;

    Translator* translator;
    Synth* synth;

    std::vector<cv::Point>* finalPoints;

    bool initialized = false;
    int state;

    cv::Point tempPoint;

    void init();
    void prepareDevice();

    void addToBuffer(float* newBuffer);
    void addToBuffer(std::vector<float>* newBuffer);
    void replaceBuffer(std::vector<float>* newBuffer);
    void resetBuffer();
    void playBuffer();

    void pause();
    void resume();

    void setTone(int waveCode);
    void sendFreq(float freq);
    void sendFreq(int oscNum, float freq);


signals:
    void initDone();

public slots:
    void run();
    void checkPlaying();


private:
    int SAMPLE_RATE;
    int CHUNK_SIZE;

};

#endif // AUDIO_H
