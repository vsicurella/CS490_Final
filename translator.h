#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QtCore>

#include <opencv2/core/core.hpp>

#include "systemconfiguration.h"

struct fingerData
{
    float previousX;
    float nextX;
    float quotient;
};

class Translator: public QObject
{
    Q_OBJECT

public:
    Translator(float lowHz, float unisonInterval, float octaves, bool quant, float numNotes, bool interp, float interpAmt);
    ~Translator();

    std::vector<cv::Point>* finalPoints;
    int maxTones;
    int iterationBound;

    // TEMP VARIABLES
    cv::Point* currentPoint;
    float scaleDegree;
    float translatedX;
    float freqOut;
    float ampOut;


    // POINTS TO FREQ
    float startingFreq;
    float harmonic;
    float range;


    // PITCH QUANTIZATION
    bool quantizing;
    float divisions;

    float quantize(float degree);


    // POINT INTERPOLATION
    bool interpolating;
    float interpSteps;
    fingerData* quotientTable;

    void interpolate(int num, float xcoord);

    // Convert x-value to frequency
    float pointToFrequency(float xcoord);

public slots:

    // main function
    void translate();

signals:

    // Int = oscillator number, float = frequency, float = amplitude
    void sendDataToSynth(int, float, float);
    void sendNumTones(int);

    void connectFP();

};

#endif // TRANSLATOR_H
