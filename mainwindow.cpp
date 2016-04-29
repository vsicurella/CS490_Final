#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioThread = new QThread;
    audioHandler = new Audio(48000);

    qDebug() << "From main thread: " << QThread::currentThreadId();

    // Initialize GUI elements
    ui->freqSld->setValue((int) (audioHandler->synth.frequency * 10000));
    ui->freqDisplay->setValue(audioHandler->synth.frequency);


    QTimer *qTimer = new QTimer(this);
    connect(qTimer, SIGNAL(timeout()), audioHandler, SLOT(run()));
    qTimer->start(1);

    audioHandler->moveToThread(audioThread);
    audioThread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked)
    {
        audioHandler->resume();
    }
    else
    {
        audioHandler->pause();
    }

}

void MainWindow::on_freqSld_sliderMoved(int position)
{
    audioHandler->sendFreq(position / 10e3f);
//    tone.frequency = position / 10e3f;
    ui->freqDisplay->setValue(position / 10e3f);
}

void MainWindow::on_freqDisplay_valueChanged(double arg1)
{
    audioHandler->sendFreq(arg1);
//    tone.frequency = arg1;
    ui->freqSld->setValue((int) arg1 * 10e3f);
}

void MainWindow::on_comboBox_activated(int index)
{
    audioHandler->setTone(index);
//    qDebug("tone: %d", tone.waveShape);
}
