#ifndef SYNTHESIZERSETTINGS_H
#define SYNTHESIZERSETTINGS_H

#include <QDialog>

namespace Ui {
class SynthesizerSettings;
}

class SynthesizerSettings : public QDialog
{
    Q_OBJECT

public:
    explicit SynthesizerSettings(QWidget *parent = 0);
    ~SynthesizerSettings();

private:
    Ui::SynthesizerSettings *ui;
};

#endif // SYNTHESIZERSETTINGS_H
