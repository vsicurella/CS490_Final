#include "oscillator.h"

Oscillator::Oscillator() {}

Oscillator::Oscillator(Synth* parent)
{
    synth = parent;
    table = synth->table;

    SAMPLE_RATE = &(synth->SAMPLE_RATE);
}

void Synth::nextSample()
{
    phaseTab = (int) round(phaseTab + frequency) % SAMPLE_RATE;
}

float* Synth::genChunk()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        buffer[i] = table[0][phaseTab] * amplitude;
        nextSample();
    }

    return buffer;
}
