#ifndef SYNTH_H
#define SYNTH_H

#include <QDebug>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include <cmath>

#include "wavetable.h"
#include "oscillator.h"

class Synth
{

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

    bool playing;

    unsigned int waveShape;
    std::vector<float> wavetable;

    Oscillator* addOsc(int num);
    void removeOsc(int num);
    void setOscNum(int num);
    void setTone(unsigned int waveCode);

    void addToBuffer(float* newBuffer);
    void addToBuffer(int idx, float toBuff);
    void appendToBuffer(float smpl);
    void removeFromBuffer(int numSmpl);
    void clearBuffer();

    void sendFreq(float freq);
    void sendFreq(int oscNum, float freq);
    void sendValues(int oscNum, float freq, float amp);

    void nextSample();
//    float* genChunk();
    std::vector<float>* genChunk();

    void start();
    void pause();
    void resume();
    void close();

    bool isPlaying();
    // Should be private


private:


signals:
    void oscNumChanged();

};

#endif // SYNTH_H
