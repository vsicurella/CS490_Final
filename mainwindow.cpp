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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include "systemconfiguration.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // create a new system configuration object
    SystemConfiguration config;
    // load the settings to system configuration
    config.loadSettings();

    ui->setupUi(this);
    configWindow = NULL;

    audioThread = new QThread;
    audioHandler = new Audio(48000, 256);

    qDebug() << "From main thread: " << QThread::currentThreadId();

    qTimer = new QTimer(this);
    timerHand = new QTimer(this);
}

void MainWindow::startAutoMode(int devId)
{
    ui->inputCameraId->setDisabled(true);
    cameraId = devId;
    // initialize the capture device
    captureDevice = VideoCapture(cameraId);

    // check if the device is accessable
    if(!captureDevice.isOpened())
    {
        // create a new message box
        QMessageBox messageBox(this);
        // show warning message
        messageBox.warning(this,"Warning","Can not open Camera Device",QMessageBox::Ok);
        // return from this function as capture device is not correct
        return;
    }

    // every thing is ok, start capturing
    startProcessing();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startBtn_clicked()
{
    // read the camera id
    cameraId = ui->inputCameraId->value();

    // initialize the capture device
    captureDevice = VideoCapture(cameraId);

    // check if the device is accessable
    if(!captureDevice.isOpened())
    {
        // create a new message box
        QMessageBox messageBox(this);
        // show warning message
        messageBox.warning(this,"Warning","Can not open Camera Device",QMessageBox::Ok);
        // return from this function as capture device is not correct
        return;
    }

    // every thing is ok, start capturing
    startProcessing();
}

void MainWindow::updateCameraFeedDisplay(QImage img1, QImage img2)
{
    // display received images
    ui->displayProcessedFrame->setPixmap(QPixmap::fromImage(img1).scaled(ui->displayProcessedFrame->width(), ui->displayProcessedFrame->height()));
    ui->displayDetectedPoints->setPixmap(QPixmap::fromImage(img2).scaled(ui->displayDetectedPoints->width(), ui->displayDetectedPoints->height()));
}

void MainWindow::startProcessing()
{
    // disable start brn
    ui->startBtn->setEnabled(false);
    // create a new camera capture object
    cameraCapture = new CameraCapture(captureDevice);

    // set up audio
    audioHandler->moveToThread(audioThread);
    connect(qTimer, SIGNAL(timeout()), audioHandler, SLOT(run()));
    connect(audioHandler, SIGNAL(initDone()), this, SLOT(connectFinalPoints()));

    // connect the signal to receive images
    connect(cameraCapture,SIGNAL(capturedNewFrame(QImage,QImage)),this,SLOT(updateCameraFeedDisplay(QImage,QImage)));

    audioHandler->translator->finalPoints = &(cameraCapture->processor.finalPoints);
    audioHandler->finalPoints = &(cameraCapture->processor.finalPoints);

//    connect(&cameraCapture->processor, SIGNAL(pointsReady()), audioHandler->translator, SLOT(makeReady()));
//    connect(qTimer, SIGNAL(timeout()), audioHandler->translator, SLOT(translate()));
    connect(timerHand, SIGNAL(timeout()), audioHandler, SLOT(checkPlaying()));
    connect(audioHandler->translator, SIGNAL(sendNumTones(int)), audioHandler->synth, SLOT(setOscNum(int)));
    timerHand->start(50);

    // start capturing process
    cameraCapture->start();
    audioThread->start();
    qTimer->start(1);
}

void MainWindow::on_btn_configure_clicked()
{
    // check if configuration window is already not initilized
    if(!configWindow)
    {
        // create a new config window
        configWindow = new ConfigurationWindow();
    }
    // show the c onfig wondow
    configWindow->show();
    // activate the window
    configWindow->raise();
    configWindow->activateWindow();
}

void MainWindow::connectFinalPoints()
{
    // point the data translator to the final finger points

}

//void MainWindow::sendFreq()
//{
////    finalPoints = cameraCapture->getFinalPoints();

//    int numOsc = finalPoints->size() - audioHandler->synth->oscillators.size();

//    if (finalPoints->size() > 0)
//    {
//        audioHandler->synth->playing = true;
////        audioHandler->sendAmp(
////                    (SystemConfiguration::image_size - finalPoints->at(0).y) / (float) SystemConfiguration::image_size);
//        audioHandler->synth->setOscNum(numOsc);

//        vector<Point>::iterator it;
//        int osc = 0; // number of oscillators
//        Point temp;

//        for (it = finalPoints->begin(); it != finalPoints->end(); it++, osc++)
//        {
//            temp = *it;
//            audioHandler->sendFreq(osc, temp.x*2);
////            qDebug() << "Finger #" << osc+1 << "x: " << temp.x << "\t y: " << temp.y;
////            qDebug() << "number of osc: " << numOsc;
//        }
//    }

//    else
//    {
//        audioHandler->synth->setOscNum(0);
//        audioHandler->synth->clearBuffer();
//        audioHandler->synth->playing = false;
//    }
//}
