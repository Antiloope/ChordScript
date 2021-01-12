#include "interactiveinterpreter.h"
#include "utils/globalconfig.h"
#include "utils/log.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

using namespace std;
using namespace CS;

const char* CONSOLE_CONFIG_COMMAND = "stty cooked brkint -ignbrk tostop -inlcr -igncr -icrnl -icanon -ctlecho";
const char* CONSOLE_RESET_COMMAND = "stty cooked";

const char COMMAND_CHARACTER = '\033';
const char INTRO_CHARACTER = '\r';
const char OPEN_BRACKET_CHARACTER = '[';
const char UP_ARROW_CHARACTER = 'A';
const char DOWN_ARROW_CHARACTER = 'B';
const char RIGHT_ARROW_CHARACTER = 'C';
const char LEFT_ARROW_CHARACTER = 'D';
const char BACKSPACE_CHARACTER = 8;
const char DELETE_CHARACTER = 127;
const char NULL_CHARACTER = '\0';
const char SUPR_CHARACTER = '3';
const char END_COMMAND_CHARACTER = '~';

enum struct Key {
    None,
    ArrowUp,
    ArrowDown,
    ArrowRight,
    ArrowLeft,
    Intro,
    Backspace,
    Any,
    Supr,
};

void Console::init() {
    // Print first line with ChordScript version
    const char* version;
    try
    {
        version = GlobalConfig::getInstance()->getParameter(ConfigDefinitions::Version);
    }
    catch( ... )
    {
        version = "";
    }

    cout << "ChordScript version: ";
    if( version )
        cout << version << endl;
    else
        cout << endl;

    // Second line
    cout << "Type \"EXIT();\" to quit." << endl;

    // Init atributes
    oldLines = vector<string>(0);

    // Change console mode
    system(CONSOLE_CONFIG_COMMAND);
}

string Console::getSourceCode() {
    string sourceCode = "";

    system(CONSOLE_CONFIG_COMMAND);

    while( true )
    {
        if( sourceCode == "" )
            cout << ">> " << flush; // New line
        else
            cout << ".. " << flush; // Continue line

        Key pressedKey = Key::None;
        bool isCommand = false;

        string line = "";
        auto cursor = line.end();
        size_t currentLineIndex = oldLines.size();

        char currentChar = getchar();
        while( pressedKey != Key::Intro )
        {
            switch( currentChar )
            {
            case COMMAND_CHARACTER:
                isCommand = true;
                break;
            case INTRO_CHARACTER:
                pressedKey = Key::Intro;
                break;
            case OPEN_BRACKET_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar)+1;
                break;
            case END_COMMAND_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar)+1;
                else
                    pressedKey = Key::Supr;
                isCommand = false;
                break;
            case UP_ARROW_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar)+1;
                else
                    pressedKey = Key::ArrowUp;
                isCommand = false;
                break;
            case DOWN_ARROW_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar)+1;
                else
                    pressedKey = Key::ArrowDown;
                isCommand = false;
                break;
            case RIGHT_ARROW_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar)+1;
                else
                    pressedKey = Key::ArrowRight;
                isCommand = false;
                break;
            case LEFT_ARROW_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar)+1;
                else
                    pressedKey = Key::ArrowLeft;
                isCommand = false;
                break;
            case DELETE_CHARACTER:
            case BACKSPACE_CHARACTER:
                pressedKey = Key::Backspace;
                isCommand = false;
                break;
            case SUPR_CHARACTER:
                if( !isCommand )
                    cursor = line.insert(cursor,currentChar);
                break;
            case NULL_CHARACTER:
                isCommand = false;
                if( cursor != line.end() )
                    cursor++;
                break;
            default:
                pressedKey = Key::Any;
                isCommand = false;
                cursor = line.insert(cursor,currentChar)+1;
                break;
            }
            switch( pressedKey )
            {
            case Key::ArrowUp:
                pressedKey = Key::None;

                system(CONSOLE_RESET_COMMAND);
                cout << "\v" << flush;
                system(CONSOLE_CONFIG_COMMAND);

                if( oldLines.size() == 0 )
                    break;

                system(CONSOLE_RESET_COMMAND);
                for( size_t i = 0; i < line.size(); i++)
                    cout << "\b \b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                line = "";

                if( currentLineIndex > 0 )
                    currentLineIndex--;

                cout << oldLines[currentLineIndex] << flush;
                line = oldLines[currentLineIndex];
                cursor = line.end();

                break;
            case Key::ArrowDown:
                pressedKey = Key::None;

                system(CONSOLE_RESET_COMMAND);
                cout << "\033[A";
                for( size_t i = 0; i < line.size(); i++)
                    cout << "\b \b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                line = "";
                cursor = line.end();

                if( oldLines.size() && currentLineIndex < oldLines.size() - 1 )
                {
                    currentLineIndex++;
                    cout << oldLines[currentLineIndex] << flush;
                    line = oldLines[currentLineIndex];
                    cursor = line.end();
                }
                break;
            case Key::ArrowLeft:
                pressedKey = Key::None;

                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.begin() )
                    cursor--;
                else
                    cout << "\033[C";

                for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                    cout << "\b";

                cout << line;
                for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                    cout << "\b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);

                break;
            case Key::ArrowRight:
                pressedKey = Key::None;
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.end() )
                    cursor++;
                else
                    cout << "\033[D";

                for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                    cout << "\b";

                cout << line;
                for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                    cout << "\b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                break;
            case Key::Intro:
                system(CONSOLE_RESET_COMMAND);
                for( auto tmpCursor = cursor; tmpCursor != line.end(); tmpCursor++)
                    cout << "\0";
                cout << endl;
                system(CONSOLE_CONFIG_COMMAND);
                continue;
            case Key::Backspace:
                pressedKey = Key::None;
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.begin() )
                {
                    cursor = line.erase(--cursor);
                    cout << "\b \033[D\033[D";
                    for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                        cout << "\b";

                    cout << line << " \b\0";
                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                    fflush(stdout);
                }
                else
                {
                    cout << "\b \033[D" << line << "\0";
                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                    fflush(stdout);
                }

                system(CONSOLE_CONFIG_COMMAND);

                break;
            case Key::Supr:
                pressedKey = Key::None;
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.end() )
                {
                    cursor = line.erase(cursor);
                    for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                        cout << "\b";

                    cout << line << " \b\0";

                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                    fflush(stdout);
                }
                system(CONSOLE_CONFIG_COMMAND);

                break;
            case Key::Any:
                pressedKey = Key::None;
                system(CONSOLE_RESET_COMMAND);
                for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                    cout << "\b";

                cout << line;
                for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                    cout << "\b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                break;
            case Key::None:
                break;
            }
            currentChar = getchar();
        }

        system(CONSOLE_RESET_COMMAND);

        if( line.size() )
            oldLines.push_back(line);

        sourceCode += line;

        if( !validSource(sourceCode) )
            continue;

        return sourceCode;
    }
}

bool Console::validSource(string sourceCode) {
    return true;
}

void Console::showError(std::string sourceCode,int charReference, string description) {
    string tmp = sourceCode.substr(0, charReference);
    int linesCounter = 1, lastJump = 0;

    for( size_t i = 0; i < tmp.size(); i++ )
        if( tmp[i] == '\n' )
        {
            linesCounter++;
            lastJump = i;
        }

    cout << "Error in line: " << endl << '\t' << linesCounter << endl;
    tmp = sourceCode.substr(lastJump + 1, charReference - lastJump - 1);
    cout << "Close to: " << endl << '\t' << tmp << endl;
    cout << "Description: " << endl << '\t' << description << endl;
}

void Console::close() {
    system(CONSOLE_RESET_COMMAND);
    redirectOutput(Clean);
}

InteractiveInterpreter::InteractiveInterpreter() {}

int InteractiveInterpreter::run()
{
    PipeInterpreter interpreter;

    // Change output to avoid messages in console.
    redirectOutput(SetNull);

    // Start an instance of a pipe interpreter in background
    int status = interpreter.run();

    if( status == ReturnCodes::FORK_ERROR )
    {
        cout << "Can not start ChordScript. Error trying to fork the process." << endl;
        return ReturnCodes::FORK_ERROR;
    }
    if( status == ReturnCodes::CHILD_PROCESS_RETURN ||
        status == ReturnCodes::ANOTHER_INSTANCE_RUNNING ) // True for child process
        return ReturnCodes::SUCCESS;

    // Only parent process executes following lines

    // Reset output.
    redirectOutput(Reset);

    // Init console and show startup messages
    _console.init();

    try
    {
        // Start processing input
        while( true )
        {
            // Get source code
            string sourceCode = _console.getSourceCode();

            // Interpret source code
            string response = PipeInterpreter::interpret(sourceCode);

            // Identify response
            if( response[0] == 'O' && response[1] == 'K' )
                continue;

            if( response[0] == 'E' && response[1] == 'X' && response[2] == 'I' && response[3] == 'T' )
                break;

            // If an error exist in source code, the response has the format:
            //  [CharacterReference] [ErrorDescription]
            // So, following lines separate both parts
            int charReference = stoi(response.substr(0,response.find_first_of(" "))) - 1;

            string description = response.substr(response.find_first_of(" ") + 1);

            // Print formated error message
            _console.showError(sourceCode,charReference,description);
        }
    }
    catch( const exception& e )
    {
        Log::getInstance().write(e.what(),Log::LogType::error_t);
    }

    // Return console to standard mode
    _console.close();

    // Stop server in background
    interpreter.stopServer();

    return ReturnCodes::SUCCESS;
}
