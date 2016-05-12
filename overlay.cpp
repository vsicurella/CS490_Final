#include "overlay.h"

Overlay::Overlay()
{

}

void Overlay::setOverlay(Mat newOverlay)
{
    overlayBase = newOverlay;
    overlayBase.copyTo(overlayAdd);
}

void Overlay::resetOverlayAdd()
{
    overlayBase.copyTo(overlayAdd);
}

int Overlay::imageYtoOverlayY(int y)
{

    return (y / (float) (size - height)) * height + yPos;
}

Mat Overlay::generate12Tone(int range)
{
    Mat base = Mat(Size(size, size), CV_8UC3, Scalar(255,0,255));

    float totalDivisions = (12.0 * range + 1);
    float width = size / totalDivisions;

    bool pattern[] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0};
    
    for (int i = 0; i < totalDivisions; i++)
    {
        if(pattern[i % 12])
            rectangle(base, Point(i*width, yPos), Point((i+1)*width, size), Scalar(0, 0, 0), CV_FILLED);
        else
            rectangle(base, Point(i*width, yPos), Point((i+1)*width, size), Scalar(255, 255, 255), CV_FILLED);

        line(base, Point(i*width, yPos), Point(i*width, size), Scalar(0, 0, 0));
    }

    line(base, Point(0, yPos), Point(size, yPos), Scalar(0, 0, 0), 3);

    return base;
}

void Overlay::addDot(Point pt)
{
    Point newPt = pt;
    newPt.y = imageYtoOverlayY(pt.y);

    circle(overlayAdd, newPt, 8, Scalar(255, 0, 0), CV_FILLED);
}


Mat Overlay::applyOverlay(Mat dest)
{
    Vec3b overPx, px;
    Vec3b transparency = Vec3b(255, 0, 255);
    Mat newMat = Mat::zeros(dest.rows, dest.cols, dest.type());

    for (int y = 0; y < dest.rows; y++)
        for (int x = 0; x < dest.cols; x++)
        {
            overPx = overlayAdd.at<Vec3b>(y, x);
//            px = dest.at<Vec3b>(y, x) * (1-alpha) + currentOverlay.at<Vec3b>(y, x) * alpha;

            if (overPx != transparency)
                px = overlayAdd.at<Vec3b>(y, x);
            else
                px = dest.at<Vec3b>(y, x);

            newMat.at<Vec3b>(y, x) = px;

        }

    return newMat;
}

void Overlay::update()
{
    height = size * heightFactor;
    yPos = size - height;
}
