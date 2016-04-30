#ifndef SYNTH_H
#define SYNTH_H

#include <QDebug>
#include <QThread>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include <cmath>
#include <iostream>

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

    Wavetable wavetable;
    std::vector<Oscillator*>* oscillators = new std::vector<Oscillator*>;
    Oscillator* tempOsc;

    float* buffer;

    float frequency;
    float phase;
    float amplitude;

    int phaseTab;

    bool playing;

    unsigned int waveShape;
    std::vector<float>* table;

    Oscillator* addOsc(int num);
    void removeOsc(int num);
    void setTone(unsigned int waveCode);

    void addToBuffer(float* newBuffer);
    void addToBuffer(int idx, float toBuff);
    void appendToBuffer(float smpl);
    void removeFromBuffer(int numSmpl);
    void clearBuffer();

    void nextSample();
    float* genChunk();

    void start();
    void pause();
    void resume();
    void close();

    bool isPlaying();
    // Should be private


private:



};

#endif // SYNTH_H
