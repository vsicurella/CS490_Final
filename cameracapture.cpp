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

void CameraCapture::addPointsToOverlay()
{
    for (int i = 0; i < processor.finalPoints.size(); i++)
    {
        overlay.addDot(processor.finalPoints[i]);
    }
}

void CameraCapture::run()
{
    qDebug() << "Capture thread: " << QThread::currentThreadId();
    processor.printThread();

    while(1)
    {
        if (overlay.showing)
            overlay.resetOverlayAdd();

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
            QImage binaryImage = convertMatToQImage(processedImageMat[0]);
            QImage processedImage;

            // add points to overlay
            if (overlay.showing)
            {
                addPointsToOverlay();

                // add overlay
                Mat processedOverlay = overlay.applyOverlay(processedImageMat[1]);
                processedImage = convertMatToQImage(processedOverlay);
            }
            else
                // create display image
               processedImage = convertMatToQImage(processedImageMat[1]);



            emit(capturedNewFrame(binaryImage, processedImage));
        }
    }
}

QImage CameraCapture::convertMatToQImage(Mat mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type() == CV_8UC4)
    {
        cvtColor(mat, mat, CV_BGRA2BGR);
    }

    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
