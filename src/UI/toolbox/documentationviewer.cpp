#include "documentationviewer.h"
#include "UI/uidefinitions.h"
#include "documentationcontent.h"
#include <QGridLayout>
#include <QPushButton>

using namespace CS::UI;

DocumentationViewer::DocumentationViewer(QWidget *parent) :
    QFrame(parent) {

    UiDefinitions* def = UiDefinitions::getInstance();

    setStyleSheet(
        "QFrame {"
            "border:none;"
            "background-color:" + def->getColorRGB(ColorId::Background) + ";"
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

    QGridLayout* grid = new QGridLayout(this);
    grid->setVerticalSpacing(3);
    grid->setHorizontalSpacing(3);
    setLayout(grid);

    QPushButton* introButton = new QPushButton("1. Intro",this);
    grid->addWidget(introButton,0,0,1,1);
    QPushButton* basicsButton = new QPushButton("2. Basics",this);
    grid->addWidget(basicsButton,0,1,1,1);
    QPushButton* dataTypesButton = new QPushButton("3. Data Types",this);
    grid->addWidget(dataTypesButton,0,2,1,1);
    QPushButton* flowControlButton = new QPushButton("4. Flow Control",this);
    grid->addWidget(flowControlButton,1,0,1,1);
    QPushButton* functionsButton = new QPushButton("5. Functions",this);
    grid->addWidget(functionsButton,1,1,1,1);
    QPushButton* preDefinitionsButton = new QPushButton("6. Pre-definitions",this);
    grid->addWidget(preDefinitionsButton,1,2,1,1);

    DocumentationContent* content = new DocumentationContent(this);
    grid->addWidget(content,2,0,1,3);

    connect(introButton,SIGNAL(clicked()),content,SLOT(display()));
    connect(basicsButton,SIGNAL(clicked()),content,SLOT(display()));
    connect(dataTypesButton,SIGNAL(clicked()),content,SLOT(display()));
    connect(flowControlButton,SIGNAL(clicked()),content,SLOT(display()));
    connect(functionsButton,SIGNAL(clicked()),content,SLOT(display()));
    connect(preDefinitionsButton,SIGNAL(clicked()),content,SLOT(display()));
    connect(content,&DocumentationContent::demo, [=](QString code) {emit this->demo(code);} );
}
