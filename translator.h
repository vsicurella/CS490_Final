#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QtCore>

#include <opencv2/core/core.hpp>

struct fingerData
{
    int previousX;
    int nextX;
    float quotient;
};

class Translator: public QObject
{
    Q_OBJECT

public:
    Translator();
    ~Translator();

    std::vector<cv::Point>* finalPoints;


    // PITCH QUANTIZATION

    float windowRange;
    float windowOffset;
    bool quantizing;
    float unisonHarmonic;
    float divisions;


    // POINT INTERPOLATION

    bool interpolating;
    int interpSteps;
    fingerData* quotientTable = new fingerData[10];

    void interpolate();

    // Convert x-value to frequency
    float pointToFrequency(int xcoord);

public slots:

    // main function
    void translate();

signals:

    // Int = oscillator number, float = frequency
    void sendDataToSynth(int, float);
};

#endif // TRANSLATOR_H