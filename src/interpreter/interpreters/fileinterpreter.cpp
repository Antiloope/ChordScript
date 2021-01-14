#include "fileinterpreter.h"
#include "utils/Exceptions/exception.h"
#include "executor/executorinterface.h"
#include "interpreter/context.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using namespace CS;

FileInterpreter::FileInterpreter(const char* fileName) : _fileName(fileName) {}

int FileInterpreter::run()
{
    // Open file
    ifstream sourceFile;
    try
    {
        sourceFile = ifstream(_fileName);

        if( !sourceFile )
        {
            cout << "File not found or invalid file name." << endl;
            return ReturnCodes::INVALID_FILE_NAME;
        }
    }
    catch(const exception& e)
    {
        cout << "Error interpreting source file: " << endl;
        cout << e.what() << endl;
        return ReturnCodes::INVALID_FILE_NAME;
    }

    string aux, sourceCode = "";

    // Read file
    while( getline(sourceFile,aux) )
        sourceCode += aux + '\n';

    sourceFile.close();

    // Create a pipe interpreter and run it
    PipeInterpreter interpreter;

    redirectOutput(SetNull);

    int status = interpreter.run();

    if( status != ReturnCodes::SUCCESS &&
        status != ReturnCodes::ANOTHER_INSTANCE_RUNNING &&
        status != CHILD_PROCESS_RETURN )
    {
        cout << "Can not start ChordScript. Error trying to fork the process." << endl;
        return ReturnCodes::FORK_ERROR;
    }
    if( status == ReturnCodes::CHILD_PROCESS_RETURN || status == ReturnCodes::ANOTHER_INSTANCE_RUNNING )
        return ReturnCodes::SUCCESS;

    redirectOutput(Reset);

    // Interpret file
    string response = PipeInterpreter::interpret(sourceCode);

    if( response[0] == 'O' && response[1] == 'K' )
        return ReturnCodes::SUCCESS;
    if( response[0] == 'E' && response[1] == 'X' && response[2] == 'I' && response[3] == 'T' )
        return ReturnCodes::SUCCESS;

    int charReference = stoi(response.substr(0,response.find_first_of(" "))) - 1;
    string tmp = sourceCode.substr(0, charReference);
    int linesCounter = 1, lastJump = 0;

    for( size_t i = 0; i < tmp.size(); i++ )
        if( tmp[i] == '\n' )
        {
            linesCounter++;
            lastJump = i;
        }

    cout << "Error in line: " << endl << '\t' << linesCounter << endl;

    if( lastJump != 0 )
        lastJump++;

    tmp = sourceCode.substr(lastJump, charReference - lastJump + 1);
    cout << "Next to: " << endl << '\t' << tmp << endl;
    cout << "Description: " << endl << '\t' << response.substr(response.find_first_of(" ") + 1) << endl;

    return ReturnCodes::CODE_ERROR;
}
