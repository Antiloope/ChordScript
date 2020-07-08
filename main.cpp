#include <QApplication>
#include "UI/uimanager.h"
#include "executor/executor.h"
#include "utils/log.h"
#include "utils/Exceptions/exception.h"
#include <stdlib.h>
#include "interpteter/interpreter.h"

using namespace CS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log::getInstance().write("Program started",Log::info_t);

    Interpreter interpreter;

    string testCode = "string pepe(sound carlos, numeric d){numeric a = 4; return ";
    testCode += '"';
    testCode += "ds";
    testCode += '"';
    testCode += ";} numeric a = 5.4+(34*3-43); group myGroup = a.d(3,54.5);";
    testCode.push_back(0x0A);
    testCode += "myGroup.bpm(110);";
    testCode.push_back(0x0A);
    testCode += "myGroup.reverb(hall);";

    try {
        interpreter.interpret(testCode);
    } catch (SyntaxException& e) {
        Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()),Log::info_t);
    } catch (SemanticException& e) {
        Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()),Log::info_t);
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
