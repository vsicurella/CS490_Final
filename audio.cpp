#include "audio.h"

Audio::Audio(int sr, int cs)
{
    SAMPLE_RATE = sr;
    CHUNK_SIZE = cs;

    qDebug() << "Sample Rate: " << SAMPLE_RATE;
    qDebug() << "Chunk Size: " << CHUNK_SIZE;
}

Audio::~Audio()
{
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    delete handle;
    delete buffer;

    delete translator;
}

void Audio::init()
{
    device = "default";
    buffer = new float[CHUNK_SIZE]();
    synth = new Synth(&SAMPLE_RATE, &CHUNK_SIZE);
    // baseHz, octave, range, quantize, divisions, interpolate, steps
    translator = new Translator(110, 2, 3, false, 12, false, 3);
    synth->setTone(Wavetable::SINE);

    prepareDevice();

    qDebug() << "Audio thread: " << QThread::currentThreadId();

    for (int i = 0; i < CHUNK_SIZE; i++)
        buffertest.push_back(&buffer[i]);

    connect(translator, SIGNAL(sendDataToSynth(int,float,float)), synth, SLOT(sendData(int, float, float)));

    emit initDone();
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

    if (synth->masterPlaying && synth->isPlaying())
    {
        replaceBuffer(synth->genChunk());
        playBuffer();
//        resetBuffer();
    }

    if (state != snd_pcm_state(handle))
    {
        state = snd_pcm_state(handle);
        qDebug() << "State: " << state;
    }

    // Buffer underrun (not being written to fast enough)
    if (state == 4)
    {
        snd_pcm_drain(handle);
        prepareDevice();
    }
}

void Audio::addToBuffer(float* newBuffer)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
        buffer[i] += newBuffer[i];
}

void Audio::addToBuffer(std::vector<float>* newBuffer)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
        buffer[i] += newBuffer[0][i];
}

void Audio::replaceBuffer(std::vector<float>* newBuffer)
{
    for (int i = 0; i < CHUNK_SIZE; i++)
        buffer[i] = newBuffer[0][i];
}

void Audio::resetBuffer()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
        buffer[i] = 0;
}

void Audio::playBuffer()
{
    frames = snd_pcm_writei(handle, buffer, CHUNK_SIZE);
}

void Audio::pause()
{
    synth->playing = false;
    snd_pcm_pause(handle, 1);
}

void Audio::resume()
{
    synth->playing = true;
    snd_pcm_pause(handle, 0);
}

void Audio::setTone(int waveCode)
{
    synth->setTone(waveCode);
}

void Audio::sendFreq(float freq)
{
    synth->sendFreq(freq);
}

void Audio::sendFreq(int oscNum, float freq)
{
    synth->sendFreq(oscNum, freq);
}

void Audio::checkPlaying()
{
    if (finalPoints->size())
    {
        synth->playing = true;
        translator->translate();
    }
    else
    {
        resetBuffer();
        synth->playing = false;
    }
}
