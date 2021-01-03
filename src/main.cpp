#include "interpreter/interpreters/interpreter.h"
#include "utils/globalconfig.h"
#include <string.h>
#include <iostream>

using namespace CS;
using namespace std;

const int NO_ADDITIONAL_ARGUMENTS = 1;

int main(int argc, char *argv[])
{
    if( argc == NO_ADDITIONAL_ARGUMENTS )
    {
        InteractiveInterpreter interpreter;
        return interpreter.run();
    }

    const char* firstArgument = argv[1];

    if( strcmp(firstArgument,"--help") == 0 ||
        strcmp(firstArgument,"-h") == 0 )
    {
        cout << "Usage: chordscript [option | FILE]" << endl;
        cout << "With no options: executes the interpreter in the interactive mode" << endl;
        cout << "Available options:" << endl;
        cout << " -h --help    : print this help message and exit" << endl;
        cout << " -v --version : print ChordScript version and exit" << endl;
        cout << " -p --pipe    : open a named pipe for receiveing code to execute" << endl;
        cout << " -s --stop    : stop the pipe service" << endl;
        cout << " FILE         : path to a .csf file" << endl;
        return ReturnCodes::SUCCESS;
    }
    if( strcmp(firstArgument,"--version") == 0 ||
        strcmp(firstArgument,"-v") == 0 )
    {
        const char* version;
        try
        {
            version = GlobalConfig::getInstance()->getParameter(ConfigDefinitions::Version);
        }
        catch( ... )
        {
            cout << "Configuration file is corrupted. Please reinstall ChordScript." << endl;
            return ReturnCodes::CONFIG_ERROR;
        }

        if( version )
            cout << "ChordScript version: " << version << endl;
        else
            cout << "Config file is not found. Please reinstall ChordScript." << endl;

        return ReturnCodes::SUCCESS;
    }

    if( strcmp(firstArgument,"--stop") == 0 ||
        strcmp(firstArgument,"-s") == 0 )
        return PipeInterpreter::stopServer();

    if( strcmp(firstArgument,"--pipe") == 0 ||
        strcmp(firstArgument,"-p") == 0 )
    {
        PipeInterpreter interpreter;
        return interpreter.run();
    }

    FileInterpreter interpreter(firstArgument);
    return interpreter.run();
}
