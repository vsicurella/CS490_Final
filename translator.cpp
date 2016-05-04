#include "translator.h"

Translator::Translator()
{

}

Translator::~Translator()
{
    delete quotientTable;
}

void Translator::translate()
{
    for (int i; i < finalPoints->size(); i++)
    {
        if (quantizing)
        {

        }

        if (interpolating)
        {

        }

//        emit sendDataToSynth(i, freq);
    }

}
