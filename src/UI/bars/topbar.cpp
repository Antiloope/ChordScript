#include "topbar.h"
#include "UI/uidefinitions.h"

using namespace CS::UI;

const int DEFAULT_WINDOW_WIDTH = 500;
const int DEFAULT_MENU_BAR_HEIGHT = 20;

TopBar::TopBar(QWidget *parent) : QMenuBar(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    setGeometry(QRect(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_MENU_BAR_HEIGHT));
    setAutoFillBackground(false);
    setPalette(UiDefinitions::getInstance()->getPalette(PaletteId::Global));
    setStyleSheet(
        "QMenuBar{"
            "background-color:" + UiDefinitions::getInstance()->getColorRGB(ColorId::Background) + ";"
        "}"
            "QMenu::item:selected { "
            "background: " + def->getColorRGB(ColorId::Primary) + ";"
        "}"
        );
}
