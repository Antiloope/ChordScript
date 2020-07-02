#include <QApplication>
#include "UI/uimanager.h"
#include "executor/executor.h"
#include "utils/log.h"

#include "interpteter/interpreter.h"

using namespace CS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log::getInstance().write("Program started",Log::info_t);

    Interpreter interpreter;

    string testCode = "numeric a = 5.4+pepe; group myGroup = [a,b,snare,kick];";
    testCode.push_back(0x0A);
    testCode += "myGroup.bpm(110);";
    testCode.push_back(0x0A);
    testCode += "myGroup.reverb(hall);";

    try {
        interpreter.interpret(testCode);
    } catch (exception& e) {
        Log::getInstance().write(e.what(),Log::info_t);
    }


    uiManager ui;
    Executor* audio = Executor::getInstance();

    ui.init();
    audio->init();

    int ret = a.exec();
    Log::getInstance().write("Program closed",Log::info_t);
    return ret;
    return 0;
}
