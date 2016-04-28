#ifndef SYNTH_H
#define SYNTH_H

#include <QDebug>

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

    unsigned int waveShape;
    std::vector<float>* table;

    void setTone(unsigned int waveCode);

    void appendToBuffer(float smpl);
    void removeFromBuffer(int numSmpl);
    void clearBuffer();
    void playBuffer();

    void nextSample();
    void genChunk();

    void start();
    void pause();
    void resume();
    void close();

    // Should be private

    // http://stackoverflow.com/questions/17217829/sending-sine-wave-values-from-array-to-audio-output
    const char* device = "default";                       //soundcard
//    snd_output_t* output = NULL;
    float* buffer = new float[CHUNK_SIZE]();

    snd_pcm_t* handle;
    snd_pcm_sframes_t frames;


private:



};

#endif // SYNTH_H
