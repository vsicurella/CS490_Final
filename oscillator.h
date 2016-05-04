#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QDebug>

#include <cmath>

#define PI 3.14159265359

class Oscillator
{

public:
    Oscillator(int* sr, float freq, float amp);

    int* SAMPLE_RATE;

    float frequency;
    float amplitude;
    float phase;
    unsigned float phaseTab;

    void nextSample();

    void setFreq(float freq);
};

#endif // OSCILLATOR_H
