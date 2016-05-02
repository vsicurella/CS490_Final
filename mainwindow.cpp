#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    audioThread = new QThread;
    audioHandler = new Audio(48000, 256);

    qDebug() << "From main thread: " << QThread::currentThreadId();


    QTimer *qTimer = new QTimer(this);
    connect(qTimer, SIGNAL(timeout()), audioHandler, SLOT(run()));
    connect(audioHandler, SIGNAL(initDone()), this, SLOT(setGUI()));
    qTimer->start(1);


    audioHandler->moveToThread(audioThread);
    audioThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setGUI()
{
    // Initialize GUI elements
    ui->freqSld->setValue((int) (audioHandler->synth.frequency * 10000));
    ui->freqDisplay->setValue(audioHandler->synth.frequency);

//    connect(audioHandler->synth, SIGNAL(oscNumChanged()), this, SLOT(updateOscNum()));
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
    ui->freqDisplay->setValue(position / 10e3f);
}

void MainWindow::on_freqDisplay_valueChanged(double arg1)
{
    audioHandler->sendFreq(arg1);
    ui->freqSld->setValue((int) arg1 * 10e3f);
}

void MainWindow::on_comboBox_activated(int index)
{
    audioHandler->setTone(index);
//    qDebug("tone: %d", tone.waveShape);
}

void MainWindow::on_addBtn_clicked()
{
    Oscillator* tempOsc = audioHandler->synth.addOsc(1);
    tempOsc->frequency = audioHandler->synth.frequency * audioHandler->synth.oscillators.size();
    ui->lcdNumber->display(ui->lcdNumber->value() + 1);
}

void MainWindow::on_removeBtn_clicked()
{
    audioHandler->synth.removeOsc(1);
    ui->lcdNumber->display(ui->lcdNumber->value() - 1);

}

void MainWindow::updateOscNum()
{
//    ui->lcdNumber->display(audioHandler->synth.oscillators.size());
}
