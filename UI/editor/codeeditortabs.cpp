#include "codeeditortabs.h"
#include "codeeditor.h"
#include "UI/uidefinitions.h"
#include "fstream"

using namespace CS::UI;

const QString newFileName = "new_file_";

CodeEditorTabs::CodeEditorTabs(QWidget* parent) :
    QTabWidget(parent) {
    UiDefinitions* def = UiDefinitions::getInstance();

    setStyleSheet(
        "QTabBar::tab:!selected {"
            "background: "+ def->getColorRGB(ColorId::Light) + ";"
            "color: " + def->getColorRGB(ColorId::TextPrimary) + ";"
            "padding: 5px;"
            "border: 3px solid " + def->getColorRGB(ColorId::Dark) + ";"
        "}"
        "QTabWidget::tab-bar {"
            "alignment: center;"
        "}"
        "QTabWidget::pane {"
            "border-top: 2px solid " + def->getColorRGB(ColorId::Dark) + ";"
            "position: absolute;"
            "top: -0.8em;"
            "padding-top: 0.8em;"
        "}"
        "QTabBar::tab:selected {"
            "padding: 8px;"
            "background: "+ def->getColorRGB(ColorId::Dark) + ";"
            "color: " + def->getColorRGB(ColorId::Lightest) + ";"
        "}"
        );

    setMovable(true);
    setTabBarAutoHide(true);
    setTabsClosable(true);
    QString fileName = newFileName + QString(std::to_string(_newFileIndex++).c_str());
    auto file = _openFiles.insert({fileName,std::tuple<std::unique_ptr<CodeEditor>,bool,QString>(std::unique_ptr<CodeEditor>(new CodeEditor(this)), false,"")});
    addTab(std::get<0>(file.first->second).get(),fileName);
}

CodeEditorTabs::~CodeEditorTabs() {}

void CodeEditorTabs::newFile() {
    QString fileName = newFileName + QString(std::to_string(_newFileIndex++).c_str());
    auto file = _openFiles.insert({fileName,std::tuple<std::unique_ptr<CodeEditor>,bool,QString>(std::unique_ptr<CodeEditor>(new CodeEditor(this)), false, "")});
    addTab(std::get<0>(file.first->second).get(),fileName);
    setCurrentIndex(count()-1);
}

void CodeEditorTabs::openFile(QString file) {
    std::ifstream source(file.toStdString(), std::ifstream::in);
    CodeEditor* editor = new CodeEditor(this);
    QString fileName = file.replace('\\','/').remove(QRegExp(".*\\/"));
    _openFiles.insert({fileName,std::tuple<std::unique_ptr<CodeEditor>,bool,QString>(std::unique_ptr<CodeEditor>(editor), false, file)});
    addTab(editor,fileName);
    std::string sourceCode;
    editor->clear();
    while( !getline(source,sourceCode).eof() )
    {
        editor->appendPlainText(sourceCode.c_str());
    }
    source.close();
    setCurrentIndex(count()-1);
}

bool CodeEditorTabs::closeFile(int index) {
    if( index < 0 )
    {
        auto file = _openFiles.find(tabText(currentIndex()));
        if( std::get<1>(file->second) )
        {
            _openFiles.erase(tabText(currentIndex()));
            removeTab(currentIndex());
            return true;
        }
        return false;
    }
    if( index < count() && std::get<1>(_openFiles.find(tabText(index))->second) )
    {
        _openFiles.erase(tabText(currentIndex()));
        removeTab(currentIndex());
        return true;
    }
    return false;
}

void CodeEditorTabs::saveFile(int index) {
    if( index < 0 )
    {
        auto file = _openFiles.find(tabText(currentIndex()));
        if( !std::get<1>(file->second) )
        {
            auto editor = std::get<0>(file->second).get();

            std::ofstream source(std::get<2>(file->second).toStdString(),std::ofstream::trunc);
            source << editor->getText().toStdString();
            source.close();

            std::get<1>(file->second) = true;
        }
        return;
    }
    auto file = _openFiles.find(tabText(currentIndex()));
    if( index < count() && std::get<1>(file->second) )
    {
        auto editor = std::get<0>(file->second).get();

        std::ofstream source(std::get<2>(file->second).toStdString(),std::ofstream::trunc);
        source << editor->getText().toStdString();
        source.close();

        std::get<1>(file->second) = true;
    }
}
