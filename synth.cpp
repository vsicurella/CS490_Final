#include "synth.h"

Synth::Synth()
{
    SAMPLE_RATE = 44100;
}

Synth::Synth(int SR)
{
    SAMPLE_RATE = SR;
    frequency = 1000;
    amplitude = 0.7;
    phase = 0;

    playing = false;

    wavetable = Wavetable(SAMPLE_RATE);
    setTone(Wavetable::SINE);
}

Synth::~Synth()
{
    for (int i = 0; i < oscillators->size(); i++)
        delete oscillators->at(oscillators->size() - i);

    delete[] oscillators;
}

bool Synth::isPlaying()
{
    return playing;
}

void Synth::addOsc(int num)
{
    for (int i = 0; i < num; i++)
        oscillators->push_back(new Oscillator(this));
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

void Synth::addToBuffer(float *toBuff)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
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
    buffer = new float[CHUNK_SIZE]();
}

void Synth::nextSample()
{
    for (int i = 0; i < oscillators->size(); i++)
    {
        tempOsc = oscillators->at(i);
        tempOsc->phaseTab = (int) round(tempOsc->phaseTab + tempOsc->frequency) % SAMPLE_RATE;
    }
}

float* Synth::genChunk()
{
    for (int o = 0; o < oscillators->size(); o++)
    {
        tempOsc = oscillators->at(o);

        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            addToBuffer(i, table[0][tempOsc->phaseTab] * tempOsc->amplitude);
        }

        nextSample();
    }

    return buffer;
}
