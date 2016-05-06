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
    bool ready = false;

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
    static float quantize(float baseHz, float octave, float divs, float size, float xcoord);


    // POINT INTERPOLATION
    bool interpolating;
    float interpSteps;
    fingerData* quotientTable;

    void interpolate(int num, float xcoord);

    // Convert x-value to frequency
    float pointToFrequency(float xcoord);
    static float pointToFrequency(float baseHz, float octave, float size, float xcoord);

public slots:

    // main function
    void translate();

    void makeReady();

signals:

    // Int = oscillator number, float = frequency, float = amplitude
    void sendDataToSynth(int, float, float);
    void sendNumTones(int);

};

#endif // TRANSLATOR_H
