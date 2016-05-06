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
#include "cameracapture.h"
#include <opencv2/opencv.hpp>
#include "systemconfiguration.h"

using namespace cv;

CameraCapture::CameraCapture(VideoCapture capture) : QThread()
{
    captureDevice = capture;
}

void CameraCapture::run()
{
    while(1)
    {
        // create a matrix to store captured image
        Mat originalImageMat(SystemConfiguration::image_size ,SystemConfiguration::image_size,CV_8UC3);
        // check if capture is success
        if(captureDevice.read(originalImageMat))
        {
            // matrix to hold resized image
            Mat resizedImageMat = Mat::zeros(SystemConfiguration::image_size ,SystemConfiguration::image_size,CV_8UC3);
            // resize the captured image
            resize(originalImageMat,resizedImageMat,resizedImageMat.size(),0,0,INTER_NEAREST);
            flip(resizedImageMat, resizedImageMat, 1);
//            flip(resizedImageMat, resizedImageMat, 1);

            // get skin image
            Mat skinImageMat = processor.getSkin(resizedImageMat);
            // get processed images
            vector<Mat> processedImageMat = processor.getprocessedImage(skinImageMat);
            // create display image
            QImage binaryImage = processor.convertMatToQImage(processedImageMat[0]);
            // create display image
            QImage processedImage = processor.convertMatToQImage(processedImageMat[1]);


            emit(capturedNewFrame(binaryImage, processedImage));
        }
    }
}
