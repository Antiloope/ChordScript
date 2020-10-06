#include "plotsettingsdialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "UI/uidefinitions.h"
#include "UI/toolbox/soundvisualizer.h"

using namespace CS::UI;

PlotSettingsDialog::PlotSettingsDialog(int samples,int stretchFactor,QWidget *parent) :
    QDialog(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    setStyleSheet(
        "QDialog {"
            "background-color: " + def->getColorRGB(ColorId::Lightest) + ";"
        "}"
        "QPushButton {"
            "border: 3px solid " + def->getColorRGB(ColorId::Dark) + ";"
            "background-color: transparent;"
            "padding: 3px;"
        "}"
        "QPushButton:hover {"
            "background-color: " + def->getColorRGB(ColorId::Light) + ";"
        "}"
        "QPushButton:pressed {"
            "background-color: " + def->getColorRGB(ColorId::Dark) + ";"
            "color: " + def->getColorRGB(ColorId::Lightest) + ";"
        "}"
        "QComboBox {"
            "border: 2px solid " + def->getColorRGB(ColorId::Dark) + ";"
            "background-color: transparent;"
            "padding: 3px;"
        "}"
        "QComboBox::item {"
            "background: " + def->getColorRGB(ColorId::Background) + ";"
            "selection-background-color: " + def->getColorRGB(ColorId::Primary) + ";"
        "}"
        );

    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    layout->addWidget(new QLabel(tr("Choose plot quality:")),1,1,1,1);

    _qualityPicker = new QComboBox(this);
    _qualityPicker->addItem("Low",Quality::Low);
    _qualityPicker->addItem("Medium",Quality::Medium);
    _qualityPicker->addItem("High",Quality::High);

    if( samples == 64 )
        _qualityPicker->setCurrentIndex(0);
    if( samples == 128 )
        _qualityPicker->setCurrentIndex(1);
    if( samples == 256 )
        _qualityPicker->setCurrentIndex(2);

    layout->addWidget(_qualityPicker,1,2,1,1);

    layout->addWidget(new QLabel(tr("Stretch factor:")),2,1,1,1);

    _stretchFactor = new QComboBox(this);
    _stretchFactor->addItem("x1",1);
    _stretchFactor->addItem("x2",2);
    _stretchFactor->addItem("x4",4);

    if( stretchFactor == 1 )
        _stretchFactor->setCurrentIndex(0);
    if( stretchFactor == 2 )
        _stretchFactor->setCurrentIndex(1);
    if( stretchFactor == 4 )
        _stretchFactor->setCurrentIndex(2);

    layout->addWidget(_stretchFactor,2,2,1,1);

    QPushButton* saveButton = new QPushButton("&Save",this);
    layout->addWidget(saveButton,3,1,1,1);

    QPushButton* closeButton = new QPushButton("&Close",this);
    layout->addWidget(closeButton,3,2,1,1);

    setWindowTitle(tr("Plot Settings"));

    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
}

void PlotSettingsDialog::save() {
    ((SoundVisualizer*)parent())->loadSettings((Quality)_qualityPicker->currentData().toInt(),_stretchFactor->currentData().toInt());
    close();
}
