#include "interpreter/interpreters/interpreter.h"
#include "utils/globalconfig.h"
#include <string.h>
#include <iostream>

using namespace CS;
using namespace std;

int main(int argc, char *argv[])
{
    if( argc == 1 )
    {
        InteractiveInterpreter interpreter;
        return interpreter.run();
    }
    if( strcmp(argv[1],"--help") == 0 || strcmp(argv[1],"-h") == 0 )
    {
        cout << "Usage: chordscript [option | FILE]" << endl;
        cout << "With no options: executes the interpreter in the interactive mode" << endl;
        cout << "Available options:" << endl;
        cout << " -h --help    : print this help message and exit" << endl;
        cout << " -v --version : print ChordScript version and exit" << endl;
        cout << " -p --pipe    : open a named pipe for receiveing code to execute" << endl;
        cout << " -s --stop    : stop the pipe service" << endl;
        return ReturnCodes::SUCCESS;
    }
    if( strcmp(argv[1],"--version") == 0 || strcmp(argv[1],"-v") == 0 )
    {
        const char* version;
        try
        {
            version = GlobalConfig::getInstance()->getParameter(ConfigDefinitions::Section::StartupSettings,ConfigDefinitions::Parameter::Version);
        }
        catch( ... )
        {
            cout << "Configuration file is corrupted. Please reinstall ChordScript." << endl;
            return ReturnCodes::CONFIG_ERROR;
        }

        cout << "ChordScript version: " << version << endl;
        return ReturnCodes::SUCCESS;
    }
    if( strcmp(argv[1],"--stop") == 0 || strcmp(argv[1],"-s") == 0 )
    {
        return PipeInterpreter::stopServer();
    }
    if( strcmp(argv[1],"--pipe") == 0 || strcmp(argv[1],"-p") == 0 )
    {
        PipeInterpreter interpreter;
        return interpreter.run();
    }

    const char* fileName = argv[1];

    FileInterpreter interpreter(fileName);
    return interpreter.run();
}
