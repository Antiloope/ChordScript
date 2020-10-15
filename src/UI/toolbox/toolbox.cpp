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

    QTextBrowser* documentation = new QTextBrowser(this);
    documentation->setSearchPaths({"./docs/"});
    documentation->setSource(QUrl("./docs/Language-Documentation.html"));
    documentation->setOpenExternalLinks(true);
    documentation->setStyleSheet(
        "QTextBrowser {"
            "border:none;"
        "}");
    this->addItem(documentation,"Language Documentation");

    QFrame* learnButtons = new QFrame(this);
    learnButtons->setStyleSheet(
        "QFrame {"
            "border:none;"
            "background-color: " + def->getColorRGB(ColorId::Background) + ";"
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
        "}");
    QGridLayout* learnButtonsLayout = new QGridLayout(learnButtons);
    learnButtons->setLayout(learnButtonsLayout);
    learnButtonsLayout->addWidget(new QPushButton("Start Tutorial",learnButtons),0,0,1,1);
    learnButtonsLayout->addWidget(new QPushButton("Go To ChordScript Page",learnButtons),1,0,1,1,Qt::AlignTop);
    this->addItem(learnButtons,"Learn more");
}
