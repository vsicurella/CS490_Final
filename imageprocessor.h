//----------------------------------------------------------------------
// VMouse - OpenCV Virtual Mouse (HCI)
// Copyright (C) 2014  Kunal Dawn <kunal.dawn@gmail.com>
// Copyright (C) 2014  Medha Devaraj <medha.devaraj@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <opencv2/opencv.hpp>
#include <QDesktopWidget>
#include <QRect>
#define PI 3.14159

using namespace cv;

class ImageProcessor: public QObject
{
    Q_OBJECT

private:

    // eleminates convex defects by its dimention and provides filtered defects
    vector<Vec4i> eleminateDefectsByDimentation(vector<Vec4i>,vector<Point>);
    // eleminates convex defects by its bounding box height region
    vector<Vec4i> eleminateDefectsByregion(vector<Vec4i>,vector<Point>, Rect);
    // calculates distance between two given point
    float distanceP2P(Point, Point);
    // calculates angle between line (p1,p2) and line (p2,p3)
    float getAngle(Point, Point, Point);
    // eleminates redundent defects that are too close to each other
    std::vector<Point> removeRedundantEndPoints(vector<Vec4i>,vector<Point>);
    // find one finger which is not detected by convex defects
    Point findTopPoint(vector<Point> contours, Point);
    // represents the bounding rectangle of the largest contour
    Rect boundingRectect;

public:
    // constructor of this class
    ImageProcessor();

    // contains final points after filter process for current frame
    vector<Point> finalPoints;

    // does preprocessing of the image and then detection/ elemination of points
    vector<Mat> getprocessedImage(Mat);
    // generates the output image based on the processed data
    Mat getOutputImage();
    // get the filtered skin image
    Mat getSkin(Mat);

    vector<Point>* getFinalPoints();
    void printThread();

};

#endif // IMAGEPROCESSOR_H
