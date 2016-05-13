#include "wavetable.h"

void Wavetable::genWaveTable(int* SAMPLE_RATE, int waveCode, std::vector<float>& table)
{
    table.clear();

    switch(waveCode){

    default:
    {
        for (int i = 0; i < *SAMPLE_RATE; i++)
            table.push_back(sin(2*PI * i / (float) *SAMPLE_RATE));

        break;
    }

    case TRI:
    {
        // can be done in one loop
        for (int i = 0; i < *SAMPLE_RATE / 4.0; i++)
        {
            table.push_back((i * 4.0) / (float) *SAMPLE_RATE);
        }

        for (int i = 0; i < *SAMPLE_RATE / 4.0; i++)
        {
            table.push_back((*SAMPLE_RATE - i * 4.0) / (float) *SAMPLE_RATE);
        }

        for (int i = 0; i < *SAMPLE_RATE / 4.0; i++)
        {
            table.push_back((i * -4.0) / (float) *SAMPLE_RATE);
        }

        for (int i = 0; i < *SAMPLE_RATE / 4.0; i++)
        {
            table.push_back((-1 * *SAMPLE_RATE + i * 4.0) / (float) *SAMPLE_RATE);
        }

        break;
    }

    case SAW:
    {
        for (int i = 0; i < *SAMPLE_RATE; i++)
            table.push_back(fmod(((i / (float) *SAMPLE_RATE + 0.25) * 2), 1.0) - 1);

        break;
    }

    case SQUARE:
    {
        for (int i= 0; i < *SAMPLE_RATE; i++)
        {
            if (i < *SAMPLE_RATE / 2)
                table.push_back(1);
            else
                table.push_back(0);
        }

        break;
    }
    }

    qDebug() << "Wavetable created";
}

std::vector<float> Wavetable::genWaveTable(int* SAMPLE_RATE, int waveCode)
{
    std::vector<float> table;

    switch(waveCode){

    default:
    {
        for (int i = 0; i < *SAMPLE_RATE; i++)
            table.push_back(sin(2*PI * i / (float) *SAMPLE_RATE));

        break;
    }

    case TRI:
    {
        for (int i = 0; i < *SAMPLE_RATE; i++)
            table.push_back(std::abs(fmod(((i / (float) *SAMPLE_RATE) * 2), 1.0) - 1) - 1);

        break;
    }

    case SAW:
    {
        for (int i = 0; i < *SAMPLE_RATE; i++)
            table.push_back(fmod(((i / (float) *SAMPLE_RATE) * 2), 1.0) - 1);

        break;
    }

    case SQUARE:
    {
        for (int i= 0; i < *SAMPLE_RATE; i++)
        {
            if (i < *SAMPLE_RATE / 2)
                table.push_back(1);
            else
                table.push_back(0);
        }

        break;
    }
    }

    qDebug() << "Wavetable created";
    return table;
}
