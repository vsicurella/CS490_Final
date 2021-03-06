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
#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QMainWindow>
#include <cmath>

namespace Ui {
class ConfigurationWindow;
}

class ConfigurationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigurationWindow(QWidget *parent = 0);
    ~ConfigurationWindow();

    void disableStuff();


private slots:
    void on_in_image_size_valueChanged(int value);
    void on_in_y_min_valueChanged(int value);
    void on_in_y_max_valueChanged(int value);
    void on_in_cb_min_valueChanged(int value);
    void on_in_cb_max_valueChanged(int value);
    void on_in_cr_min_valueChanged(int value);
    void on_in_cr_max_valueChanged(int value);
    void on_in_open_kernel_valueChanged(int value);
    void on_in_close_kernel_valueChanged(int value);
    void on_in_blur_kernel_valueChanged(int value);
    void on_in_blur_sigma_valueChanged(int value);
    void on_in_image_thresh_valueChanged(int value);
    void on_in_area_thresh_valueChanged(int value);
    void on_in_dp_epsilon_valueChanged(int value);
    void on_in_dt_min_valueChanged(int value);
    void on_in_dt_max_valueChanged(int value);
    void on_in_at_min_valueChanged(int value);
    void on_in_at_max_valueChanged(int value);
    void on_in_rh_thresh_valueChanged(int value);
    void on_in_rpd_thresh_valueChanged(int value);
    void on_saveBtn_clicked();

    void on_volumeSld_sliderMoved(int position);

    void on_baseHzSld_sliderMoved(int position);

    void on_baseHzBox_valueChanged(double arg1);

    void on_isMuted_clicked(bool checked);

    void on_rangeBox_valueChanged(int arg1);

    void on_toneBox_activated(int index);

    void on_isQuantized_clicked(bool checked);

    void on_harmBox_valueChanged(int arg1);

    void on_divBox_valueChanged(int arg1);

    void on_overlayOn_clicked(bool checked);

    void on_overlayHeightSld_sliderMoved(int position);

    void on_numTones_valueChanged(int arg1);

signals:
    void sr(int);
    void bs(int);
    void volume(float);
    void pitch(float);
    void playing(bool);
    void numTones(int);
    void tn(int);
    void rng(int);
    void quantized(bool);
    void harmonic(int);
    void divisions(int);
    void overlayOn(bool);
    void overlayHeight(float);

private:
    Ui::ConfigurationWindow *ui;
};

#endif // CONFIGURATIONWINDOW_H
