#include "toolbox.h"

#include <QPushButton>

using namespace CS::UI;

ToolBox::ToolBox(QWidget *parent)
    : QToolBox(parent) {
    this->setStyleSheet("QToolBox::tab {background: #ed9bff ;padding:0px;border-radius: 5px; color: white;}QToolBox::tab:selected { /* italicize selected tabs */    font: italic;    color: white;}");
    this->setMaximumWidth(200);
    this->setMinimumWidth(150);
    this->addItem(new QPushButton(this),"Sound analizer");
    this->addItem(new QPushButton(this),"Help");
}
