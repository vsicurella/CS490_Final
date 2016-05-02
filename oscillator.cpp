#include "oscillator.h"

Oscillator::Oscillator(int* sr, float freq, float amp)
{
    SAMPLE_RATE = sr;

    frequency = freq;
    amplitude = amp;
    phase = 0;
    phaseTab = 0;
}

void Oscillator::nextSample()
{
    phaseTab = (int) round(phaseTab + frequency) % *SAMPLE_RATE;
    phase = (phaseTab / (float) *SAMPLE_RATE) * 2 * PI;
}

void Oscillator::setFreq(float freq)
{
    frequency = freq;
//    qDebug() << "freq changed to: " << freq;
}
