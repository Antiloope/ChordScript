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
    bool closeFile(int index = -1,bool force = false);
    bool saveFile(int index = -1);
    void saveFile(QString file, int index = -1);
    void saveAsFile(QString file, int index = -1);
    ~CodeEditorTabs();
    void setError(int);
private:
    std::map<QString,std::tuple<std::unique_ptr<CodeEditor>,bool,QString>> _openFiles;
    size_t _newFileIndex = 0;
private slots:
    void tabModified();
public slots:
    void find(bool,bool,bool,QString);
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void zoomIn();
    void zoomOut();
    void comment();
    void openNewFile(QString code);
};

}
}

#endif // CODEEDITORTABS_H
