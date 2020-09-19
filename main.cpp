#include <QApplication>
#include "UI/uimanager.h"
#include "executor/executor.h"
#include "utils/log.h"
#include "utils/Exceptions/exception.h"
#include <stdlib.h>
#include "interpteter/interpreter.h"
#include "interpteter/context.h"

#include <iostream>

using namespace CS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log::getInstance().title("Program started");

    Context::getInstance()->load();

    Executor* audio = Executor::getInstance();
    audio->init();

    UI::UiManager ui;
    ui.init(a);

    int ret = a.exec();

    audio->closeAll();
    Log::getInstance().close("Program closed");
    return ret;
    return 0;
}
