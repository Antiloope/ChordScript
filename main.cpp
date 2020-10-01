#include <QApplication>
#include "UI/uimanager.h"
#include "executor/executor.h"
#include "utils/log.h"
#include "utils/Exceptions/exception.h"
#include <stdlib.h>
#include "interpreter/interpreter.h"
#include "interpreter/context.h"

#include <iostream>

using namespace CS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log::getInstance().write("Program started",Log::info_t);

    Context::getInstance()->load();

    Executor* audio = Executor::getInstance();
    audio->init();

    UI::UiManager ui;
    ui.init(a);

    int ret;
    try
    {
        ret = a.exec();
    }
    catch( const exception& e)
    {
        Log::getInstance().write(e.what(),Log::error_t);
    }

    audio->closeAll();
    Log::getInstance().write("Program closed",Log::info_t);
    return ret;
}
