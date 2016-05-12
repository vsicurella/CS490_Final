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
    cameraCapture->overlay.setOverlay(cameraCapture->overlay.generate12Tone(range));

    // set up audio
    configWindow->disableStuff();
//    connectParameters();
    audioHandler = new Audio(sample_rate, buffer_size);
    audioHandler->moveToThread(audioThread);
    connect(qTimer, SIGNAL(timeout()), audioHandler, SLOT(run()));
    connect(audioHandler, SIGNAL(initDone()), this, SLOT(connectFinalPoints()));

    // connect the signal to receive images
    connect(cameraCapture,SIGNAL(capturedNewFrame(QImage,QImage)),this,SLOT(updateCameraFeedDisplay(QImage,QImage)));

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
    qDebug() << "got signal!";
    // point the data translator to the final finger points
    audioHandler->translator->finalPoints = cameraCapture->processor.getFinalPoints();
    audioHandler->finalPoints = cameraCapture->processor.getFinalPoints();

    connect(timerHand, SIGNAL(timeout()), audioHandler, SLOT(checkPlaying()));
    connect(audioHandler->translator, SIGNAL(sendNumTones(int)), audioHandler->synth, SLOT(setOscNum(int)));
    connect(qTimer, SIGNAL(timeout()), audioHandler, SLOT(run()));

    timerHand->start(5);
}

void MainWindow::connectParameters()
{
    connect(configWindow, SIGNAL(sr(int)), this, SLOT(set_sr(int)));
    connect(configWindow, SIGNAL(bs(int)), this, SLOT(set_bs(int)));
    connect(configWindow, SIGNAL(volume(float)), this, SLOT(set_volume(float)));
    connect(configWindow, SIGNAL(pitch(float)), this, SLOT(set_pitch(float)));
    connect(configWindow, SIGNAL(playing(bool)), this, SLOT(set_playing(bool)));
    connect(configWindow, SIGNAL(tn(int)), this, SLOT(set_tone(int)));
    connect(configWindow, SIGNAL(rng(int)), this, SLOT(set_range(int)));
    connect(configWindow, SIGNAL(quantized(bool)), this, SLOT(set_quantized(bool)));
    connect(configWindow, SIGNAL(harmonic(int)), this, SLOT(set_harmonic(int)));
    connect(configWindow, SIGNAL(divisions(int)), this, SLOT(set_divisions(int)));
    connect(configWindow, SIGNAL(overlayOn(bool)), this, SLOT(set_overlayOn(bool)));
    connect(configWindow, SIGNAL(overlayHeight(float)), this, SLOT(set_overlayHeight(float)));
}

void MainWindow::set_sr(int sr)
{
    sample_rate = sr;
}

void MainWindow::set_bs(int bs)
{
    buffer_size = bs;
}

void MainWindow::set_volume(float vol)
{
    audioHandler->synth->masterVolume = vol;
}

void MainWindow::set_pitch(float pitch)
{
    audioHandler->translator->startingFreq = pitch;
}

void MainWindow::set_playing(bool playing)
{
    audioHandler->synth->playing = playing;
}

void MainWindow::set_tone(int tn)
{
    audioHandler->synth->setTone(tn);
}

void MainWindow::set_range(int rng)
{
    audioHandler->translator->range = rng;
}

void MainWindow::set_quantized(bool quant)
{
    audioHandler->translator->quantizing = quant;
}

void MainWindow::set_harmonic(int harm)
{
    audioHandler->translator->harmonic = 2;
}

void MainWindow::set_divisions(int div)
{
    audioHandler->translator->divisions = div;
}

void MainWindow::set_overlayOn(bool overOn)
{
    cameraCapture->overlay.showing = overOn;
}

void MainWindow::set_overlayHeight(float overHeight)
{
    cameraCapture->overlay.height = (SystemConfiguration::image_size * overHeight);
    cameraCapture->overlay.update();
    cameraCapture->overlay.setOverlay(cameraCapture->overlay.generate12Tone(range));
}
