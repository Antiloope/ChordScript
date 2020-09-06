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

//    Interpreter interpreter;

//    string testCode;
//    ifstream source("/home/antiloope/Documents/tesis/build-ChordScript-Desktop_Qt_5_14_2_GCC_64bit-Debug/source.txt",ifstream::in);
//    char c;
//    while(source.get(c))
//        testCode.push_back(c);
//    source.close();

    Executor* audio = Executor::getInstance();
    audio->init();

//    try {
//        interpreter.interpret(testCode);
//    } catch (SyntaxException& e) {
//        Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()) + testCode[e.getCharacterRefference()-1] + testCode[e.getCharacterRefference()] + testCode[e.getCharacterRefference()+1],Log::info_t);
//    } catch (SemanticException& e) {
//        Log::getInstance().write(e.what() + to_string(e.getCharacterRefference()) + testCode[e.getCharacterRefference()-1] + testCode[e.getCharacterRefference()] + testCode[e.getCharacterRefference()+1],Log::info_t);
//    } catch (exception& e) {
//        Log::getInstance().write(e.what(),Log::info_t);
//    }

    UI::UiManager ui;
    ui.init(a);

    int ret = a.exec();
    Log::getInstance().close("Program closed");
    return ret;
    return 0;
}
