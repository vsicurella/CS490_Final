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

    maxTones = 8;

    quotientTable = new fingerData[maxTones];
}

Translator::~Translator()
{
    delete quotientTable;
}

void Translator::translate()
{
    if (finalPoints->size() < maxTones)
        iterationBound = finalPoints->size();
    else
        iterationBound = maxTones;

//    qDebug() << finalPoints->size();
    emit sendNumTones(iterationBound);

    for (int i = 0; i < iterationBound; i++)
    {
        currentPoint = &finalPoints[0][i];
        translatedX = currentPoint->x;
        ampOut = 1 - (currentPoint->y / (float) SystemConfiguration::image_size);

        scaleDegree = (divisions * range * currentPoint->x/(float) SystemConfiguration::image_size);

        if (quantizing)
        {
            translatedX = quantize(scaleDegree);
        }

        // DOES NOT WORK YET
        if (interpolating)
        {
            interpolate(i, translatedX);
        }

        freqOut = pointToFrequency(translatedX);

//        qDebug() << "------------\nnum: " << i << "\nfreq: " << freqOut << "\namp: " << ampOut << "\n------------";

        emit sendDataToSynth(i, freqOut, ampOut);
    }

}

float Translator::quantize(float degree)
{
    float newDeg = round(degree);
    return newDeg * SystemConfiguration::image_size / (divisions * range);
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

float Translator::getDegreeFreq(float base, float degree)
{
    return (base * pow(harmonic, degree));
}

float Translator::getFactor(float interval)
{
    return log(interval)/log(harmonic);
}
