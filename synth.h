#ifndef SYNTH_H
#define SYNTH_H

#include <QDebug>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include <cmath>

#include "wavetable.h"
#include "oscillator.h"

class Synth: public QObject
{
    Q_OBJECT

public:
    Synth();
    Synth(int* sr, int* cs);
    ~Synth();

    int* SAMPLE_RATE;
    int* CHUNK_SIZE;

    std::vector<Oscillator> oscillators;
    Oscillator* tempOsc;

//    float* buffer;
    std::vector<float> buffer;

    float frequency;
    float amplitude;

    float masterVolume = 1;

    bool playing;

    unsigned int waveShape;
    std::vector<float> wavetable;

    Oscillator* addOsc(int num);
    void removeOsc(int num);
    void setTone(unsigned int waveCode);

    void addToBuffer(float* newBuffer);
    void addToBuffer(int idx, float toBuff);
    void appendToBuffer(float smpl);
    void removeFromBuffer(int numSmpl);
    void clearBuffer();

    void sendFreq(float freq);
    void sendFreq(int oscNum, float freq);

    void nextSample();
    std::vector<float>* genChunk();

    void start();
    void pause();
    void resume();
    void close();

    bool isPlaying();
    // Should be private

public slots:

    void setOscNum(int num);
    void sendData(int oscNum, float freq, float amp);


private:


signals:

};

#endif // SYNTH_H
