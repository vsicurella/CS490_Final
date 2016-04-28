#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize GUI elemnts
    ui->freqSld->setValue((int) (tone.frequency * 10000));
    ui->freqDisplay->setValue(tone.frequency);

    tone.setTone(Wavetable::SINE);
    tone.start();

    QTimer *qTimer = new QTimer(this);
    connect(qTimer,SIGNAL(timeout()),this,SLOT(playNote()));
    qTimer->start(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playNote()
{
    if (ui->checkBox->isChecked())
    {
        tone.genChunk();
        tone.playBuffer();
        tone.clearBuffer();
    }

    if (state != snd_pcm_state(tone.handle))
    {
        state = snd_pcm_state(tone.handle);
        qDebug("state: %d", snd_pcm_state(tone.handle));
    }

    if (state == 4)
    {
        snd_pcm_reset(tone.handle);
    }
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked)
    {
        tone.resume();
    }
    else
    {
        tone.pause();
    }

}

void MainWindow::on_freqSld_sliderMoved(int position)
{
    tone.frequency = position / 10e3f;
    ui->freqDisplay->setValue(tone.frequency);
}

void MainWindow::on_freqDisplay_valueChanged(double arg1)
{
    tone.frequency = arg1;
    ui->freqSld->setValue((int) arg1 * 10e3f);
}

void MainWindow::on_comboBox_activated(int index)
{
    tone.setTone(index);
    qDebug("tone: %d", tone.waveShape);
}
