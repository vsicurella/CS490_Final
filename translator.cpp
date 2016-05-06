#include "translator.h"

Translator::Translator(float lowHz, float unisonInterval, float octaves, bool quant, float numNotes, bool interp, float interpAmt)
{
    startingFreq = lowHz;
    harmonic = unisonInterval;
    range = octaves;

    quantizing = quant;
    divisions = numNotes;

    interpolating = interp;
    interpSteps = interpAmt;

    maxTones = 1;

    quotientTable = new fingerData[maxTones];
}

Translator::~Translator()
{
    delete quotientTable;
}

void Translator::translate()
{
    if (ready)
    {
        if (finalPoints->size() < maxTones)
            iterationBound = finalPoints->size();
        else
            iterationBound = maxTones;

//        qDebug() << finalPoints->size();
        emit sendNumTones(iterationBound);

        for (int i = 0; i < iterationBound; i++)
        {
            currentPoint = &finalPoints[0][i];
            translatedX = currentPoint->x;
            ampOut = 1 - (currentPoint->y / (float) SystemConfiguration::image_size);

            scaleDegree = (range * currentPoint->x/(float) SystemConfiguration::image_size);

            if (quantizing)
            {
                translatedX = quantize(scaleDegree);
            }

            if (interpolating)
            {
                interpolate(i, translatedX);
            }

            freqOut = pointToFrequency(translatedX);

            qDebug() << "------------\nnum: " << i << "\nfreq: " << freqOut << "\namp: " << ampOut << "\n------------";

            emit sendDataToSynth(i, freqOut, ampOut);
        }
    }
}

float Translator::quantize(float degree)
{
    return round(divisions * log(degree)/log(harmonic)) * SystemConfiguration::image_size / range;
}

void Translator::interpolate(int num, float xcoord)
{   
    // Update
    quotientTable[num].previousX = quotientTable[num].nextX;
    quotientTable[num].nextX = translatedX;

    // Get multipication factor
    quotientTable[num].quotient = quotientTable[num].nextX/quotientTable[num].previousX;

    // Only interpolate if the difference is large enough
    if (abs(quotientTable[num].nextX - quotientTable[num].previousX) > 2)
        translatedX *= quotientTable[num].quotient;
}

float Translator::pointToFrequency(float xcoord)
{
    return (startingFreq * pow(harmonic, range * xcoord / SystemConfiguration::image_size));
}

void Translator::makeReady()
{
    ready = true;
}
