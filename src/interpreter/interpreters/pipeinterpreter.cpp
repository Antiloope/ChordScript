#include "pipeinterpreter.h"

#include "utils/log.h"
#include "interpreter/context.h"
#include "executor/executorinterface.h"
#include "utils/Exceptions/exception.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace CS;

const char* PIPE_FILE_SOURCE = "/tmp/cssource";
const char* PIPE_FILE_RESPONSE = "/tmp/csresponse";
const string STOP_SERVER_COMMAND = "STOP_SERVER();";
const int IS_RUNNING_CODE = 0;
const string SUCCESS_RESPONSE = "OK";
const char* CHILD_PROCESS_NAME = "ChordScriptPipe";

const pid_t FORK_ERROR_ID = -1;
const pid_t CHILD_PROCESS = 0;

PipeInterpreter::PipeInterpreter() {}

int PipeInterpreter::run()
{
    switch( fork() )
    {
    case FORK_ERROR_ID:
        cout << "Can not start ChordScript in pipe mode. Error trying to fork the process." << endl;
        return ReturnCodes::FORK_ERROR;
    case CHILD_PROCESS:
    {
        setsid();
        if( !isRunning() )
        {
            prctl(PR_SET_NAME, (unsigned long) CHILD_PROCESS_NAME);
            startServer();
            return ReturnCodes::CHILD_PROCESS_RETURN;
        }
        else
        {
            cout << "There was another instance of ChordScript running. Stop it in order to run anorher one.";
            return ReturnCodes::ANOTHER_INSTANCE_RUNNING;
        }
    }
        break;
    default:
        cout << "ChordScript running in pipe mode..." << endl;
        cout << "To stop the server send the command 'STOP_SERVER()' to the pipe or type 'chordscript --stop'" << endl;
        return ReturnCodes::SUCCESS;
    }
}

bool PipeInterpreter::isRunning()
{
    return ((system(
                ("pgrep " + string(CHILD_PROCESS_NAME) + " > /dev/null").c_str()
            ) == IS_RUNNING_CODE));
}

int PipeInterpreter::startServer()
{
    Log::getInstance().write("Program started in pipe mode",Log::info_t);

    // Init an interpreter context with start values.
    Context::getInstance()->load();

    int savedOut = dup(1);
    int savedEOut = dup(2);
    dup2(open("/dev/null",O_WRONLY), 1);
    dup2(open("/dev/null",O_WRONLY), 2);

    // Init global executor.
    ExecutorInterface::init();

    dup2(savedOut, 1);
    dup2(savedEOut, 2);
    close(savedOut);
    close(savedEOut);

    // Create pipe
    mkfifo(PIPE_FILE_SOURCE, 0666);
    mkfifo(PIPE_FILE_RESPONSE, 0666);
    bool running = true;
    while( running )
    {
        ifstream codeFile(PIPE_FILE_SOURCE);
        string tmp, sourceCode = "";
        // Read all received code
        while(getline(codeFile,tmp))
            sourceCode += tmp + '\n';

        if( sourceCode == "" )
            continue;

        codeFile.close();

        ofstream responseFile(PIPE_FILE_RESPONSE);

        if( sourceCode.substr(0,STOP_SERVER_COMMAND.size()) == STOP_SERVER_COMMAND )
        {
            running = false;
            continue;
        }

        string response = SUCCESS_RESPONSE;

        try
        {
            Interpreter::interpret(sourceCode);
        }
        catch( CS::SyntaxException& e )
        {
            response = to_string(e.getCharacterRefference()) + " ";
            response += e.what();
        }
        catch( CS::SemanticException& e )
        {
            response = to_string(e.getCharacterRefference()) + " ";
            response += e.what();
        }
        catch( std::exception& e )
        {
            response = "0 ";
            response += e.what();
        }

        responseFile << response;
        responseFile.close();
    }

    system(("rm " + string(PIPE_FILE_SOURCE)).c_str());
    system(("rm " + string(PIPE_FILE_RESPONSE)).c_str());

    ExecutorInterface::closeAll();

    Log::getInstance().write("Program closed",Log::info_t);

    return ReturnCodes::SUCCESS;
}

int PipeInterpreter::stopServer()
{
    if( !isRunning() )
    {
        cout << "There are not a running insntance of ChordScript. Nothing to do." << endl;
        return ReturnCodes::NOTHING_TO_DO;
    }

    // Send command to pipe
    ofstream codeFile(PIPE_FILE_SOURCE);

    codeFile << STOP_SERVER_COMMAND << endl;
    codeFile.close();

    ifstream responseFile(PIPE_FILE_RESPONSE);

    string response;
    responseFile >> response;
    responseFile.close();

    cout << "Successfully stopped." << endl;
    return ReturnCodes::SUCCESS;
}

string PipeInterpreter::interpret(string source)
{
    mkfifo(PIPE_FILE_SOURCE, 0666);
    mkfifo(PIPE_FILE_RESPONSE, 0666);
    ofstream codeFile(PIPE_FILE_SOURCE);

    codeFile << source << endl;

    codeFile.close();

    ifstream response(PIPE_FILE_RESPONSE);

    string aux, ret = "";
    while(getline(response,aux))
        ret += aux + '\n';

    response.close();

    return ret;
}
