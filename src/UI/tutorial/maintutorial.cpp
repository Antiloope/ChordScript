#include "maintutorial.h"
#include "UI/uidefinitions.h"
#include <QGridLayout>
#include <QLabel>

using namespace CS::UI;

MainTutorial::MainTutorial(QWidget *parent) : QDialog(parent) {
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
        );
    setWindowTitle("Tutorial");

    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);

    QLabel* label = new QLabel(this);
    label->setText("Wellcome to ChordScript!! Tutorial will be available in future versions...");

    layout->addWidget(label);
}
