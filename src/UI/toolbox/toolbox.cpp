#include "toolbox.h"
#include "UI/uidefinitions.h"
#include <QPushButton>
#include "documentationviewer.h"
#include "UI/tutorial/maintutorial.h"
#include <QDesktopServices>
#include <QGridLayout>
#include <QUrl>

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
        "QScrollBar:vertical {"
            "border-left: 1px solid " + def->getColorRGB(ColorId::Dark) + ";"
            "background: transparent;"
            "width: 9px;"
        "}"
        "QScrollBar::handle {"
            "background: " + def->getColorRGB(ColorId::Dark) + ";"
        "}"
        "QScrollBar::add-line:vertical {"
            "height: 0px;"
            "subcontrol-position: bottom;"
            "subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:vertical {"
            "height: 0 px;"
            "subcontrol-position: top;"
            "subcontrol-origin: margin;"
        "}"
        );

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
    connect(_documentation,&DocumentationViewer::demo, [=](QString code) {emit this->demo(code);} );
}

void ToolBox::showTutorial() {
    auto tutorial = MainTutorial();
    tutorial.exec();
}

void ToolBox::openCSPage() {
    QDesktopServices::openUrl(QUrl("https://antiloope.github.io/ChordScriptPage/"));
}
