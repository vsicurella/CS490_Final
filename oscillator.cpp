#include "oscillator.h"
#include "synth.h"

//Oscillator::Oscillator() {}

Oscillator::Oscillator(Synth* parent)
{
    synth = parent;
    table = synth->table;

    SAMPLE_RATE = synth->SAMPLE_RATE;
    CHUNK_SIZE = synth->CHUNK_SIZE;
}

void Oscillator::nextSample()
{
    phaseTab = (int) round(phaseTab + frequency) % *SAMPLE_RATE;
}

//float* Oscillator::genChunk()
//{
//    for (int i = 0; i < *CHUNK_SIZE; i++)
//    {
//        buffer[i] = table[0][phaseTab] * amplitude;
//        nextSample();
//    }

//    return buffer;
//}
