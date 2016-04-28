#include "wavetable.h"

Wavetable::Wavetable()
{
    SAMPLE_RATE = 44100;
}

Wavetable::Wavetable(int SR)
{
    SAMPLE_RATE = SR;

    table = new std::vector<float>();
    table->reserve(SAMPLE_RATE);
}


std::vector<float>* Wavetable::genWaveTable(int waveCode)
{
    table->clear();

    switch(waveCode){

    default:
    {
        for (int i = 0; i < SAMPLE_RATE; i++)
            table->push_back(sin(2*PI * i / (float) SAMPLE_RATE));

        break;
    }

    case TRI:
    {
        for (int i = 0; i < SAMPLE_RATE; i++)
            table->push_back(std::abs(fmod(((i / (float) SAMPLE_RATE) * 2), 1.0) - 1) - 1);

        break;
    }

    case SAW:
    {
        for (int i = 0; i < SAMPLE_RATE; i++)
            table->push_back(fmod(((i / (float) SAMPLE_RATE) * 2), 1.0) - 1);

        break;
    }

    case SQUARE:
    {
        for (int i= 0; i < SAMPLE_RATE; i++)
        {
            if (i < SAMPLE_RATE / 2)
                table->push_back(1);
            else
                table->push_back(0);
        }

        break;
    }
    }

    return table;
}
