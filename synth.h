#ifndef SYNTH_H
#define SYNTH_H

#include <QDebug>
#include <QThread>

#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include <cmath>
#include <iostream>

#include "wavetable.h"

class Synth
{

public:
    Synth();
    Synth(int SR);
    ~Synth();

    int SAMPLE_RATE;
    int CHUNK_SIZE = 256;

    Wavetable wavetable;

    float frequency;
    float phase;
    float amplitude;

    int phaseTab;

    bool playing;

    unsigned int waveShape;
    std::vector<float>* table;

    void setTone(unsigned int waveCode);

    void appendToBuffer(float smpl);
    void removeFromBuffer(int numSmpl);
    void clearBuffer();
    void playBuffer();

    void nextSample();
    float* genChunk();

    void start();
    void pause();
    void resume();
    void close();

    bool isPlaying();
    // Should be private

    const char* device = "default";
    float* buffer = new float[CHUNK_SIZE]();

    snd_pcm_t* handle;
    snd_pcm_sframes_t frames;


private:



};

#endif // SYNTH_H
