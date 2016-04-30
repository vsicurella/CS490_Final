#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <iostream>

#include "audio.h"
#include "synth.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_checkBox_toggled(bool checked);

    void on_freqSld_sliderMoved(int poaudioThreadsition);

    void on_freqDisplay_valueChanged(double arg1);

    void on_comboBox_activated(int index);

    void on_addBtn_clicked();

    void on_removeBtn_clicked();

private:
    Ui::MainWindow *ui;

//    AudioHandler* audioHandler = new AudioHandler(48000);
    Audio* audioHandler;

    QThread* audioThread;

    bool playing = false;
};

#endif // MAINWINDOW_H
