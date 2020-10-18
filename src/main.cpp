#include <QApplication>
#include "executor/executor.h"
#include "executor/executorinterface.h"
#include "utils/log.h"
#include "interpreter/context.h"
#include "UI/maininterface.h"

using namespace CS;
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(UI::UiDefinitions::getInstance()->getIcon(UI::IconId::App));

    Log::getInstance().write("Program started",Log::info_t);

    // Init an interpreter context with start values.
    Context::getInstance()->load();

    // Init global executor.
    ExecutorInterface::init();

    // Init main window interface
    UI::MainInterface* mainWindow = new UI::MainInterface();
    mainWindow->show();

    int ret;
    try
    {
        ret = app.exec();
    }
    catch( const exception& e )
    {
        Log::getInstance().write(e.what(),Log::error_t);
    }

    ExecutorInterface::closeAll();

    Log::getInstance().write("Program closed",Log::info_t);

    delete mainWindow;
    return ret;
}
