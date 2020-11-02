#include "finder.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include "UI/uidefinitions.h"

using namespace CS::UI;

Finder::Finder(QWidget *parent) : QFrame(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    QGridLayout *gridLayout = new QGridLayout;
    setLayout(gridLayout);

    QPushButton *findNextButton = new QPushButton("Find Next",this);
    QPushButton *findPreviousButton = new QPushButton("Find Previous",this);
    _textToFind = new QLineEdit(this);
    _caseSensitiveCheck = new QCheckBox("Case sensitive",this);
    _wholeWordsCheck = new QCheckBox("Whole words",this);
    QLabel *searchLabel = new QLabel("Search for:");

    setFixedHeight(100);
    setStyleSheet(
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
        "QLineEdit {"
            "selection-background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Primary) + ";"
            "border: 1px solid " + def->getColorRGB(ColorId::LightGray) + ";"
            "padding: 3px;"
        "}"
        );

    setTabOrder(_textToFind,findNextButton);
    setTabOrder(findNextButton,findPreviousButton);
    setTabOrder(findPreviousButton,_caseSensitiveCheck);
    setTabOrder(_caseSensitiveCheck,_wholeWordsCheck);

    gridLayout->addWidget(_textToFind,0,1,1,3);
    gridLayout->addWidget(findNextButton,1,2,1,2);
    gridLayout->addWidget(findPreviousButton,1,0,1,2);
    gridLayout->addWidget(searchLabel,0,0,1,1,Qt::AlignCenter);
    gridLayout->addWidget(_caseSensitiveCheck,2,0,1,2,Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(_wholeWordsCheck,2,2,1,2,Qt::AlignHCenter | Qt::AlignTop);

    update();

    connect(findNextButton,SIGNAL(clicked()),this,SLOT(findNext()));
    connect(findPreviousButton,SIGNAL(clicked()),this,SLOT(findPrevious()));
}

void Finder::findNext() {
    emit find(
        false,
        _caseSensitiveCheck->isChecked(),
        _wholeWordsCheck->isChecked(),
        _textToFind->text());
}

void Finder::findPrevious() {
    emit find(
        true,
        _caseSensitiveCheck->isChecked(),
        _wholeWordsCheck->isChecked(),
        _textToFind->text());
}

void Finder::setFocusFind() {
    _textToFind->setFocus();
}
