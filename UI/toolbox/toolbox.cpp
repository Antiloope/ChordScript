#include "toolbox.h"
#include "UI/uidefinitions.h"
#include <QPushButton>
#include "soundvisualizer.h"

using namespace CS::UI;

ToolBox::ToolBox(QWidget *parent)
    : QToolBox(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    this->setStyleSheet(
        "QToolBox::tab {"
            "border: 3px solid " + def->getColorRGB(ColorId::Dark) + ";"
        "}"
        "QToolBox::tab:selected {"
            "font-weight: bold;"
            "background: "+ def->getColorRGB(ColorId::Dark) + ";"
            "color: " + def->getColorRGB(ColorId::Lightest) + ";"
        "}"
        "QToolBox::tab:!selected {"
            "background: "+ def->getColorRGB(ColorId::Light) + ";"
            "color: " + def->getColorRGB(ColorId::TextPrimary) + ";"
        "}"
        );

    _soundVisualizer = new SoundVisualizer(this);
    this->addItem(_soundVisualizer,"Sound Visualizer");
    this->addItem(new QPushButton(this),"Language Documentation");
    this->addItem(new QPushButton(this),"Help");
}
