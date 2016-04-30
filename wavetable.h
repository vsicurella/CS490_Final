#ifndef WAVETABLE_H
#define WAVETABLE_H

#include <QDebug>

#include <cmath>
#include <iostream>

#define PI 3.14159265358979323846


class Wavetable
{
public:
    Wavetable();
    Wavetable(int* SR);

    int* SAMPLE_RATE;

//    float* table;
    std::vector<float>* table;

//    float* genWaveTable(int waveCode);
    std::vector<float>* genWaveTable(int waveCode);


    enum WaveType: unsigned int {SINE, TRI, SAW, SQUARE};

    /*
     * 0: Sine
     * 1: Triangle
     * 2: Saw
     * 3: Square
     * default = Sine
     */
};

#endif // WAVETABLE_H
