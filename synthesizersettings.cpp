#include "synthesizersettings.h"
#include "ui_synthesizersettings.h"

SynthesizerSettings::SynthesizerSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SynthesizerSettings)
{
    ui->setupUi(this);
}

SynthesizerSettings::~SynthesizerSettings()
{
    delete ui;
}
