#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <iostream>

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
    void playNote();

    void on_checkBox_toggled(bool checked);

    void on_freqSld_sliderMoved(int position);

    void on_freqDisplay_valueChanged(double arg1);

    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;

    Synth tone = Synth(48000);

    bool isPlaying = false;

    int state;
};

#endif // MAINWINDOW_H
