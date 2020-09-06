#ifndef UIDEFINITIONS_H
#define UIDEFINITIONS_H

#include <QPalette>
#include <QFont>
#include <QColor>
#include <QIcon>

namespace CS {
namespace UI {

enum struct PaletteId {
    Global = 0,
};

enum struct FontId {
    Global = 0,
    Monospace,
};

enum struct ColorId {
    Primary = 0,
    Secondary,
    Background,
    TextPrimary,
    TextSecondary,
    Lightest,
    Light,
    Dark,
    Darkest,
    Shadow,
};

enum struct IconId {
    App = 0,
};

class UiDefinitions {
public:
    static UiDefinitions* getInstance();
    ~UiDefinitions();
    QPalette getPalette(PaletteId) const;
    QFont getFont(FontId) const;
    QIcon getIcon(IconId);
private:
    QPalette _palettes[1];
    QFont _fonts[2];
    QColor _colors[10];
    QIcon _icons[1];
    static UiDefinitions* instance;
    UiDefinitions();
};

}
}

#endif // UIDEFINITIONS_H