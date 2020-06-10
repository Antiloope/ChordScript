#include <QApplication>
#include "UI/uihandler.h"
#include "audio/audioserveradapter.h"
#include "utils/log.h"

using namespace CS;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log::getInstance().write("Program started",Log::info_t);

    uiHandler ui;
    AudioServerAdapter* audio = AudioServerAdapter::getInstance();

    ui.init();
    audio->init();

    int ret = a.exec();
    Log::getInstance().write("Program closed",Log::info_t);
    return ret;
    return 0;
}
