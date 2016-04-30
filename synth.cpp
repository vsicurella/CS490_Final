#include "synth.h"

Synth::Synth()
{
}

Synth::Synth(int* sr, int* cs)
{
    SAMPLE_RATE = sr;
    CHUNK_SIZE = cs;
    frequency = 1000;
    amplitude = 0.7;
    phase = 0;

    playing = false;

    wavetable = Wavetable(SAMPLE_RATE);
    setTone(Wavetable::SINE);

    buffer = new float(*CHUNK_SIZE);
}

Synth::~Synth()
{
    delete buffer;

    for (int i = 0; i < oscillators->size(); i++)
        delete oscillators->at(oscillators->size() - i);

    delete[] oscillators;
}

bool Synth::isPlaying()
{
    return playing;
}

Oscillator* Synth::addOsc(int num)
{
    Oscillator* newOsc = new Oscillator(this);

    for (int i = 0; i < num; i++)
        oscillators->push_back(newOsc);

    return newOsc;
}

void Synth::removeOsc(int num)
{
    for (int i = 0; i < num; i++)
        oscillators->pop_back();
}

void Synth::setTone(unsigned int waveCode)
{
    waveShape = waveCode;
    table = wavetable.genWaveTable(waveCode);
}

void Synth::addToBuffer(float *newBuffer)
{
    for (int i = 0; i < *CHUNK_SIZE; i++)
    {
        buffer[i] += newBuffer[i];
    }
}

void Synth::addToBuffer(int idx, float toBuff)
{
    buffer[idx] += toBuff;
}

void Synth::clearBuffer()
{
    delete buffer;
    buffer = new float[*CHUNK_SIZE]();
}

void Synth::nextSample()
{
    for (int i = 0; i < oscillators->size(); i++)
    {
        tempOsc = oscillators->at(i);
        tempOsc->phaseTab = (int) round(tempOsc->phaseTab + tempOsc->frequency) % *SAMPLE_RATE;
    }
}

float* Synth::genChunk()
{
    for (int o = 0; o < oscillators->size(); o++)
    {
        tempOsc = oscillators->at(o);

        for (int i = 0; i < *CHUNK_SIZE; i++)
        {
            addToBuffer(i, table[0][tempOsc->phaseTab] * tempOsc->amplitude);
            tempOsc->nextSample();
        }

//        nextSample();
    }

    return buffer;
}
