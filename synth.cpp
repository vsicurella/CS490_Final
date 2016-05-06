#include "synth.h"

Synth::Synth()
{
}

Synth::Synth(int* sr, int* cs)
{
    SAMPLE_RATE = sr;
    CHUNK_SIZE = cs;
    frequency = 440;
    amplitude = 0.5;

    playing = false;
    wavetable = (Wavetable::genWaveTable(SAMPLE_RATE, Wavetable::SINE));

    addOsc(1);

//    buffer = new float[*CHUNK_SIZE]();
    buffer.reserve(*CHUNK_SIZE);

    // Initialize buffer
    for (int i = 0; i < *CHUNK_SIZE; i++)
        buffer.push_back(0);
}

Synth::~Synth()
{
}

bool Synth::isPlaying()
{
    return playing;
}

Oscillator* Synth::addOsc(int num)
{
    for (int i = 0; i < num; i++)
        oscillators.push_back(Oscillator(SAMPLE_RATE, frequency, amplitude));

    return &oscillators.back();
}

void Synth::removeOsc(int num)
{
    for (int i = 0; i < num; i++)
        oscillators.pop_back();
}

void Synth::setOscNum(int num)
{
    if (num > 0)
        addOsc(num);

    else if (num < 0)
        removeOsc(num);
}

void Synth::setTone(unsigned int waveCode)
{
    waveShape = waveCode;
    Wavetable::genWaveTable(SAMPLE_RATE, waveCode, wavetable);
}

void Synth::addToBuffer(float *newBuffer)
{
    for (int i = 0; i < *CHUNK_SIZE; i++)
        buffer[i] += newBuffer[i];

}

void Synth::addToBuffer(int idx, float toBuff)
{
    buffer[idx] += toBuff;
}

void Synth::clearBuffer()
{
    for (int i = 0; i < *CHUNK_SIZE; i++)
        buffer[i] = 0;
}

void Synth::sendFreq(float freq)
{
    frequency = freq;

    for (int i = 0; i < oscillators.size(); i++)
    {
//        oscillators.at(i).frequency = freq * (i + 1);
        oscillators.at(i).setFreq(freq * (i+1));
    }
}

void Synth::sendFreq(int oscNum, float freq)
{
    if (oscNum < oscillators.size())
        oscillators[oscNum].setFreq(freq);
}

void Synth::nextSample()
{
    for (int i = 0; i < oscillators.size(); i++)
    {
        tempOsc = &oscillators.at(i);
        tempOsc->phaseTab = (int) round(tempOsc->phaseTab + tempOsc->frequency) % *SAMPLE_RATE;
    }
}
std::vector<float>* Synth::genChunk()
{
    clearBuffer();

    for (int osc = 0; osc < oscillators.size(); osc++)
    {
        tempOsc = &oscillators[osc];
        for (unsigned int i = 0; i < *CHUNK_SIZE; i++)
        {
            addToBuffer(i, wavetable[round(tempOsc->phaseTab)] * tempOsc->amplitude);
            tempOsc->nextSample();
        }
    }

    return &buffer;
}

void Synth::sendData(int oscNum, float freq, float amp)
{ 
    tempOsc = &oscillators[oscNum];
//    qDebug() << "Changing oscillator #" << oscNum << " from " << tempOsc->frequency << "hz to " << freq << "hz.";
    tempOsc->frequency = freq;
    tempOsc->amplitude = amp;

}
