#include "uidefinitions.h"

using namespace CS::UI;

#define iCast static_cast<int>

UiDefinitions* UiDefinitions::instance = new UiDefinitions();

UiDefinitions* UiDefinitions::getInstance() {
    return instance;
}

namespace {
const QString DEFAULT_GLOBAL_FONT = QString::fromUtf8("Sans Serif");
const int DEFAULT_GLOBAL_FONT_SIZE = 9;
const int DEFAULT_GLOBAL_FONT_WIDTH = 50;
const std::string APP_ICON_PATH = ":/icons/resources/circulo.svg";
}

UiDefinitions::UiDefinitions() {
    // Colors set up
    _colors[iCast(ColorId::Primary)] = QColor(179,2,217,255);
    _colors[iCast(ColorId::Secondary)] = QColor(77,253,39,255);
    _colors[iCast(ColorId::Background)] = QColor(255,255,255,255);
    _colors[iCast(ColorId::TextPrimary)] = QColor(26,26,26,255);
    _colors[iCast(ColorId::TextSecondary)] = QColor(102,102,102,255);
    _colors[iCast(ColorId::Darkest)] = QColor(0,0,0,255);
    _colors[iCast(ColorId::Lightest)] = QColor(255,255,255,255);
    _colors[iCast(ColorId::Light)] = QColor(239,168,254,255);
    _colors[iCast(ColorId::Dark)] = QColor(92,1,111,255);
    _colors[iCast(ColorId::Shadow)] = QColor(153,153,153,153);
    _colors[iCast(ColorId::H_DataType)] = QColor(93,1,111,255);
    _colors[iCast(ColorId::H_ReservedKeyword)] = QColor(132,2,161,255);
    _colors[iCast(ColorId::H_Comment)] = QColor(102,102,102,255);
    _colors[iCast(ColorId::H_Function)] = QColor(179,2,217,255);

    // Creating brushes to paint palette
    QBrush primary(_colors[iCast(ColorId::Primary)]);
    primary.setStyle(Qt::SolidPattern);
    QBrush secondary(_colors[iCast(ColorId::Secondary)]);
    secondary.setStyle(Qt::SolidPattern);
    QBrush background(_colors[iCast(ColorId::Background)]);
    background.setStyle(Qt::SolidPattern);
    QBrush textPrimary(_colors[iCast(ColorId::TextPrimary)]);
    textPrimary.setStyle(Qt::SolidPattern);
    QBrush textSecondary(_colors[iCast(ColorId::TextSecondary)]);
    textSecondary.setStyle(Qt::SolidPattern);
    QBrush dark(_colors[iCast(ColorId::Dark)]);
    dark.setStyle(Qt::SolidPattern);
    QBrush light(_colors[iCast(ColorId::Light)]);
    light.setStyle(Qt::SolidPattern);
    QBrush lightest(_colors[iCast(ColorId::Lightest)]);
    lightest.setStyle(Qt::SolidPattern);
    QBrush darkest(_colors[iCast(ColorId::Darkest)]);
    darkest.setStyle(Qt::SolidPattern);
    QBrush shadow(_colors[iCast(ColorId::Shadow)]);
    shadow.setStyle(Qt::SolidPattern);
    QBrush dataType(_colors[iCast(ColorId::H_DataType)]);
    dataType.setStyle(Qt::SolidPattern);

    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::WindowText, textPrimary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Button, light);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Dark, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Text, textPrimary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::ButtonText, textSecondary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Base, background);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Window, background);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Shadow, shadow);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Highlight, primary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::Link, secondary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::LinkVisited, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::AlternateBase, light);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::ToolTipText, light);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Active, QPalette::PlaceholderText, background);

    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::WindowText, textPrimary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Button, light);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Dark, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Text, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::ButtonText, textSecondary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Base, background);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Window, background);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Shadow, shadow);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Highlight, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::Link, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::LinkVisited, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::AlternateBase, lightest);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::ToolTipText, light);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Inactive, QPalette::PlaceholderText, background);

    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::WindowText, textPrimary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Button, lightest);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Dark, darkest);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Text, textPrimary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::ButtonText, textSecondary);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Base, background);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Window, background);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Shadow, shadow);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Highlight, darkest);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::Link, darkest);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::LinkVisited, dark);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::AlternateBase, lightest);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::ToolTipText, light);
    _palettes[iCast(PaletteId::Global)].setBrush(QPalette::Disabled, QPalette::PlaceholderText, background);

    _fonts[iCast(FontId::Global)].setFamily(QString::fromUtf8("Sans Serif"));
    _fonts[iCast(FontId::Global)].setPointSize(DEFAULT_GLOBAL_FONT_SIZE);
    _fonts[iCast(FontId::Global)].setBold(false);
    _fonts[iCast(FontId::Global)].setWeight(DEFAULT_GLOBAL_FONT_WIDTH);
    _fonts[iCast(FontId::Global)].setStrikeOut(false);
    _fonts[iCast(FontId::Global)].setKerning(true);

    _fonts[iCast(FontId::Monospace)].setFamily(QString::fromUtf8("Courier"));
    _fonts[iCast(FontId::Monospace)].setPointSize(DEFAULT_GLOBAL_FONT_SIZE);
    _fonts[iCast(FontId::Monospace)].setBold(false);
    _fonts[iCast(FontId::Monospace)].setWeight(DEFAULT_GLOBAL_FONT_WIDTH);
    _fonts[iCast(FontId::Monospace)].setStrikeOut(false);
    _fonts[iCast(FontId::Monospace)].setKerning(true);

    _icons[iCast(IconId::App)] = QIcon(QString::fromUtf8(APP_ICON_PATH.c_str()));
}

QPalette UiDefinitions::getPalette(PaletteId paletteId) const {
    return _palettes[iCast(paletteId)];
}

QFont UiDefinitions::getFont(FontId fontId) const {
    return _fonts[iCast(fontId)];
}

QIcon UiDefinitions::getIcon(IconId iconId) const{
    return _icons[iCast(iconId)];
}

QColor UiDefinitions::getColor(ColorId colorId) const{
    return _colors[iCast(colorId)];
}
