#include "audio.h"

Audio::Audio(int sr, int cs)
{
    SAMPLE_RATE = sr;
    CHUNK_SIZE = cs;
}

Audio::~Audio()
{
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    delete device;
    delete handle;
}

void Audio::init()
{
    device = "default";
    buffer = new float[CHUNK_SIZE]();
    synth = Synth(&SAMPLE_RATE, &CHUNK_SIZE);

    prepareDevice();

    qDebug() << "Audio thread: " << QThread::currentThreadId();
}

void Audio::prepareDevice()
{
    snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(handle, SND_PCM_FORMAT_FLOAT, SND_PCM_ACCESS_RW_INTERLEAVED,
                       1, SAMPLE_RATE, 1, 100000);
}

void Audio::run()
{
    if (!initialized)
    {
        init();
        initialized = true;
        qDebug() << "Audio components initialized.";
    }

    if (synth.isPlaying())
    {
        addToBuffer(synth.genChunk());
        playBuffer();
        resetBuffer();
    }

    if (state != snd_pcm_state(handle))
    {
        state = snd_pcm_state(handle);
        qDebug() << "State: " << state;
    }

    if (state == 4)
    {
        snd_pcm_drain(handle);

        prepareDevice();
    }
}

void Audio::addToBuffer(float* newBuffer)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        buffer[i] += newBuffer[i];

        // /* Clip. may not be necessary
        if (buffer[i] > 1.f)
        {
            buffer[i] = 1;
        }
        else if (buffer[i] < -1.f)
        {
            buffer[i] = -1;
        }
        // */
    }
}

void Audio::resetBuffer()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
        buffer[i] = 0;
}

void Audio::playBuffer()
{
    snd_pcm_writei(handle, buffer, CHUNK_SIZE);
}

void Audio::pause()
{
    synth.playing = false;
    snd_pcm_pause(handle, 0);

}

void Audio::resume()
{
    synth.playing = true;
    snd_pcm_pause(handle, 1);
}

void Audio::setTone(int waveCode)
{
    synth.setTone(waveCode);
}

void Audio::sendFreq(float freq)
{
    synth.frequency = freq;
}

