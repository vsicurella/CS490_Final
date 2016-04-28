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

    wavetable = Wavetable(SAMPLE_RATE);
}

Synth::~Synth()
{
}

void Synth::setTone(unsigned int waveCode)
{
    waveShape = waveCode;
    table = wavetable.genWaveTable(waveCode);
}

void Synth::clearBuffer()
{
    delete buffer;
    buffer = new float[CHUNK_SIZE]();
}

void Synth::playBuffer()
{
    frames = snd_pcm_writei(handle, buffer, CHUNK_SIZE);

    // Clear buffer

    return;
}

void Synth::nextSample()
{
    phaseTab = (int) round(phaseTab + frequency) % SAMPLE_RATE;
    if(phaseTab >= SAMPLE_RATE)
        abort();
}

void Synth::genChunk()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        buffer[i] = table[0][phaseTab] * amplitude;
        nextSample();
    }
}

void Synth::start()
{
//    qDebug("got to start");
    snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(handle, SND_PCM_FORMAT_FLOAT, SND_PCM_ACCESS_RW_INTERLEAVED,
                                      1, SAMPLE_RATE, 1, 100000);
}

void Synth::pause()
{
    snd_pcm_pause(handle, 1);
}

void Synth::resume()
{
    snd_pcm_pause(handle, 0);
}

void Synth::close()
{
    snd_pcm_close(handle);
}
