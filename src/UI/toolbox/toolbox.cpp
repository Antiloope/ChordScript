#include "toolbox.h"
#include "UI/uidefinitions.h"
#include <QPushButton>
#include "soundvisualizer.h"
#include "documentationviewer.h"
#include "UI/tutorial/maintutorial.h"
#include <QDesktopServices>

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

    _documentation = new DocumentationViewer(this);
    this->addItem(_documentation,"Language Documentation");

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

    QPushButton* tutorialButton = new QPushButton("Start Tutorial",learnButtons);
    learnButtonsLayout->addWidget(tutorialButton,0,0,1,1);
    QPushButton* pageButton = new QPushButton("Go To ChordScript Page",learnButtons);
    learnButtonsLayout->addWidget(pageButton,1,0,1,1,Qt::AlignTop);
    this->addItem(learnButtons,"Learn more");

    connect(tutorialButton,SIGNAL(clicked()),this, SLOT(showTutorial()));
    connect(pageButton,SIGNAL(clicked()),this, SLOT(openCSPage()));
}

void ToolBox::showTutorial() {
    auto tutorial = MainTutorial();
    tutorial.exec();
}

void ToolBox::openCSPage() {
    QDesktopServices::openUrl(QUrl("https://antiloope.github.io/ChordScriptPage/"));
}
