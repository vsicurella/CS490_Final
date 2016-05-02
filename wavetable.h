#ifndef WAVETABLE_H
#define WAVETABLE_H

#include <QDebug>

#include <cmath>
#include <iostream>

#define PI 3.14159265359


class Wavetable
{
public:
    static void genWaveTable(int* SAMPLE_RATE, int waveCode, std::vector<float>& table);
    static std::vector<float> genWaveTable(int* SAMPLE_RATE, int waveCode);

    enum WaveType: unsigned int {SINE, TRI, SAW, SQUARE};

    /*
     * 0: Sine
     * 1: Triangle
     * 2: Saw
     * 3: Square
     * default = Sine
     */

private:
    Wavetable(){}
};

#endif // WAVETABLE_H
