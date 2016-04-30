#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QDebug>

#include "synth.h"


class Oscillator
{
public:
    Oscillator();
    Oscillator(Synth parentIn);

    Synth* synth;
    std::vector<float>* table;

    int* SAMPLE_RATE;
    bool playing;


    float frequency;
    float amplitude;
    float phase;
    int phaseTab;


    void nextSample();
    float* genChunk();


};

#endif // OSCILLATOR_H
