#ifndef CODEEDITORTABS_H
#define CODEEDITORTABS_H

#include <QTabWidget>
#include <map>
#include <memory>

namespace CS {
namespace UI {

class CodeEditor;

class CodeEditorTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit CodeEditorTabs(QWidget *parent = nullptr);
    void newFile();
    void openFile(QString file);
    bool closeFile(int index = -1);
    void saveFile(int index = -1);
    ~CodeEditorTabs();
private:
    std::map<QString,std::tuple<std::unique_ptr<CodeEditor>,bool,QString>> _openFiles;
    size_t _newFileIndex = 0;
};

}
}

#endif // CODEEDITORTABS_H
