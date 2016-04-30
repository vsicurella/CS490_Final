#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QDebug>

class Synth;
//#include "synth.h"

class Oscillator
{

public:
//    Oscillator(){};
    Oscillator(Synth* parent);

    Synth* synth;
    std::vector<float>* table;

    int* SAMPLE_RATE;
    int* CHUNK_SIZE;
    bool playing;


    float frequency;
    float amplitude;
    float phase;
    int phaseTab;


    void nextSample();
//    float* genChunk();


};

#endif // OSCILLATOR_H
