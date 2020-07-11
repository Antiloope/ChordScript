#include <QApplication>
#include "UI/uimanager.h"
#include "executor/executor.h"
#include "utils/log.h"
#include "utils/Exceptions/exception.h"
#include <stdlib.h>
#include "interpteter/interpreter.h"

#include <iostream>

using namespace CS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log::getInstance().write("Program started",Log::info_t);

    Interpreter interpreter;

    string testCode;
    ifstream source("source.txt",ifstream::in);
    char c;
    while(source.get(c))
        testCode.push_back(c);
    source.close();

    try {
        interpreter.interpret(testCode);
    } catch (SyntaxException& e) {
        Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()) + testCode[e.getCharacterRefference()-1] + testCode[e.getCharacterRefference()] + testCode[e.getCharacterRefference()+1],Log::info_t);
    } catch (SemanticException& e) {
        Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()) + testCode[e.getCharacterRefference()-1] + testCode[e.getCharacterRefference()] + testCode[e.getCharacterRefference()+1],Log::info_t);
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
