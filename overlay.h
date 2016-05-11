#ifndef OVERLAY_H
#define OVERLAY_H

#include <QDebug>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "systemconfiguration.h"

using namespace cv;

class Overlay
{
public:
    Overlay();

    bool showing = true;

    Mat overlayBase, overlayAdd;

    int size = SystemConfiguration::image_size;
    int height = size / 6;
    int yPos = size - height;


    void setOverlay(Mat newOverlay);
    void resetOverlayAdd();
    void addDot(Point pt);
    int imageYtoOverlayY(int y);
    Mat generatePitchMap(int divisions);
    Mat generate12Tone(int range);
    Mat applyOverlay(Mat dest);
};

#endif // OVERLAY_H
