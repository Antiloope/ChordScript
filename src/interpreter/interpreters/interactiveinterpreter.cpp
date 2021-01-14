#include "interactiveinterpreter.h"
#include "utils/globalconfig.h"
#include "utils/log.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <stack>

using namespace std;
using namespace CS;

const char* CONSOLE_CONFIG_COMMAND = "stty cooked brkint -ignbrk tostop -icrnl -inlcr -igncr -imaxbel -icanon -ctlecho -isig erase ^?";
const char* CONSOLE_RESET_COMMAND = "stty cooked";

const char ESCAPE_CHARACTER = '\033';
const char CARRIAGE_RETURN_CHARACTER = '\r';
const char NEW_LINE_CHARACTER = '\n';
const char OPEN_BRACKET_CHARACTER = '[';
const char UP_ARROW_CHARACTER = 'A';
const char DOWN_ARROW_CHARACTER = 'B';
const char RIGHT_ARROW_CHARACTER = 'C';
const char LEFT_ARROW_CHARACTER = 'D';
const char END_COMMAND_CHARACTER = '~';
const char SUPR_CHARACTER = '3';
const char HOME_CHARACTER = '7';
const char END_CHARACTER = '8';
const char CTRL_C_CHARACTER = 3;
const char CTRL_V_CHARACTER = 22;
const char CTRL_X_CHARACTER = 24;
const char CTRL_Z_CHARACTER = 26;
const char CTRL_L_CHARACTER = 12;
const char CTRL_P_CHARACTER = 16;
const char BACKSPACE_CHARACTER = 8;
const char DELETE_CHARACTER = 127;

const char CURSOR_DOWN_COMMAND = '\v';
const char* CURSOR_UP_COMMAND = "\033[A";
const char* CURSOR_RIGHT_COMMAND = "\033[C";
const char* CURSOR_LEFT_COMMAND = "\033[D";

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
    Home,
    End,
    Ctrl_C,
    Ctrl_V,
    Ctrl_X,
    Ctrl_Z,
    Ctrl_L,
    Ctrl_P,
    Escape,
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

        string line = "";
        auto cursor = line.end();
        size_t currentLineIndex = oldLines.size();

        char currentChar = getchar();
        while( pressedKey != Key::Intro )
        {
            // This switch detect witch key is pressed
            switch( currentChar )
            {
            case ESCAPE_CHARACTER:
                currentChar = getchar();
                switch( currentChar )
                {
                case OPEN_BRACKET_CHARACTER:
                    currentChar = getchar();
                    switch( currentChar )
                    {
                    case UP_ARROW_CHARACTER:
                        pressedKey = Key::ArrowUp;
                        break;
                    case DOWN_ARROW_CHARACTER:
                        pressedKey = Key::ArrowDown;
                        break;
                    case RIGHT_ARROW_CHARACTER:
                        pressedKey = Key::ArrowRight;
                        break;
                    case LEFT_ARROW_CHARACTER:
                        pressedKey = Key::ArrowLeft;
                        break;
                    case SUPR_CHARACTER:
                        currentChar = getchar();
                        if( currentChar == END_COMMAND_CHARACTER )
                            pressedKey = Key::Supr;
                        break;
                    case HOME_CHARACTER:
                        currentChar = getchar();
                        if( currentChar == END_COMMAND_CHARACTER )
                            pressedKey = Key::Home;
                        break;
                    case END_CHARACTER:
                        currentChar = getchar();
                        if( currentChar == END_COMMAND_CHARACTER )
                            pressedKey = Key::End;
                        break;
                    default:
                        pressedKey = Key::Any;
                        break;
                    }
                    break;
                case ESCAPE_CHARACTER:
                    pressedKey = Key::Escape;
                    break;
                default:
                    pressedKey = Key::Any;
                    break;
                }
                break;
            case NEW_LINE_CHARACTER:
            case CARRIAGE_RETURN_CHARACTER:
                pressedKey = Key::Intro;
                break;
            case DELETE_CHARACTER:
            case BACKSPACE_CHARACTER:
                pressedKey = Key::Backspace;
                break;
            case CTRL_C_CHARACTER:
                pressedKey = Key::Ctrl_C;
                break;
            case CTRL_V_CHARACTER:
                pressedKey = Key::Ctrl_V;
                break;
            case CTRL_X_CHARACTER:
                pressedKey = Key::Ctrl_X;
                break;
            case CTRL_Z_CHARACTER:
                pressedKey = Key::Ctrl_Z;
                break;
            case CTRL_L_CHARACTER:
                pressedKey = Key::Ctrl_L;
                break;
            case CTRL_P_CHARACTER:
                pressedKey = Key::Ctrl_P;
                break;
            default:
                pressedKey = Key::Any;
                break;
            }
            // This switch process the pressed key
            switch( pressedKey )
            {
            case Key::ArrowUp:
                // Undo up event
                system(CONSOLE_RESET_COMMAND);
                cout << CURSOR_DOWN_COMMAND << flush;
                system(CONSOLE_CONFIG_COMMAND);

                // If there are not old lines, do nothing
                if( oldLines.size() == 0 )
                    break;

                // Clean line
                system(CONSOLE_RESET_COMMAND);
                for( size_t i = 0; i < line.size(); i++)
                    cout << "\b \b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                line = "";

                // Set current line using previous old line stored
                if( currentLineIndex > 0 )
                    currentLineIndex--;

                cout << oldLines[currentLineIndex] << flush;
                line = oldLines[currentLineIndex];
                cursor = line.end();

                break;
            case Key::ArrowDown:
                // Undo down event
                system(CONSOLE_RESET_COMMAND);
                cout << CURSOR_UP_COMMAND;

                // Clean line
                for( size_t i = 0; i < line.size(); i++)
                    cout << "\b \b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                line = "";
                cursor = line.end();

                // Change current line index if there are another old line
                if( oldLines.size() && currentLineIndex < oldLines.size() - 1 )
                {
                    currentLineIndex++;
                    cout << oldLines[currentLineIndex] << flush;
                    line = oldLines[currentLineIndex];
                    cursor = line.end();
                }
                break;
            case Key::ArrowLeft:
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.begin() )
                    cursor--; // Move cursor to the left
                else
                    cout << CURSOR_RIGHT_COMMAND; // Undo left move

                // Go to line start
                for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                    cout << "\b";

                // Overwrite line
                cout << line;

                // Go again to cursor position
                for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                    cout << "\b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);

                break;
            case Key::ArrowRight:
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.end() )
                    cursor++; // Move cursor to the right
                else
                    cout << CURSOR_LEFT_COMMAND; // Undo right move

                // Go to line start
                for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                    cout << "\b";

                // Overwrite line
                cout << line;

                // Go again to cursor position
                for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                    cout << "\b";
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                break;
            case Key::Intro:
                system(CONSOLE_RESET_COMMAND);
                // Go to end of the line
                for( auto tmpCursor = cursor; tmpCursor != line.end(); tmpCursor++)
                    cout << "\0";

                // Print an end line
                cout << endl;
                system(CONSOLE_CONFIG_COMMAND);
                continue;
            case Key::Backspace:
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.begin() )
                {
                    // Erase character
                    cursor = line.erase(--cursor);
                    cout << "\b \b\b";

                    // Overwrite line
                    for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                        cout << "\b";

                    cout << line << " \b\0";
                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                    fflush(stdout);
                }
                else // If cursor is in the first character
                {
                    // Clean backspace character and overwrite line
                    cout << "\b \b" << line << "\0";
                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                    fflush(stdout);
                }
                system(CONSOLE_CONFIG_COMMAND);
                break;
            case Key::Supr:
                system(CONSOLE_RESET_COMMAND);
                if( cursor != line.end() )
                {
                    // Erase character
                    cursor = line.erase(cursor);

                    // Overwrite line
                    for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                        cout << "\b";

                    cout << line << " \b\0";

                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                    fflush(stdout);
                }
                system(CONSOLE_CONFIG_COMMAND);
                break;
            case Key::Home:
                system(CONSOLE_RESET_COMMAND);
                while( true )
                {
                    cout << CURSOR_LEFT_COMMAND;
                    if( cursor != line.begin() )
                        cursor--; // Move cursor to the left
                    else
                        break;

                    // Go to line start
                    for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                        cout << "\b";

                    // Overwrite line
                    cout << line;

                    // Go again to cursor position
                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                }
                cout << CURSOR_RIGHT_COMMAND; // Undo left move
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);

                break;
            case Key::End:
                system(CONSOLE_RESET_COMMAND);
                while( true )
                {
                    cout << CURSOR_RIGHT_COMMAND;
                    if( cursor != line.end() )
                        cursor++; // Move cursor to the right
                    else
                        break;

                    // Go to line start
                    for( auto tmpCursor = cursor; tmpCursor != line.begin(); tmpCursor--)
                        cout << "\b";

                    // Overwrite line
                    cout << line;

                    // Go again to cursor position
                    for( auto tmpCursor = line.end(); tmpCursor != cursor; tmpCursor--)
                        cout << "\b";
                }

                cout << CURSOR_LEFT_COMMAND; // Undo right move
                fflush(stdout);
                system(CONSOLE_CONFIG_COMMAND);
                break;
            case Key::Ctrl_C:
            case Key::Ctrl_V:
            case Key::Ctrl_X:
            case Key::Ctrl_Z:
            case Key::Ctrl_L:
            case Key::Ctrl_P:
            case Key::Escape:
                break;
            case Key::Any:
                system(CONSOLE_RESET_COMMAND);
                // Insert new character
                cursor = line.insert(cursor,currentChar) + 1;

                // Overwrite line
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
            pressedKey = Key::None;
            currentChar = getchar();
        }

        // Insert line into oldLines
        if( line.size() )
            oldLines.push_back(line);

        sourceCode += line;

        if( !validSource(sourceCode) )
            continue;

        system(CONSOLE_RESET_COMMAND);

        return sourceCode;
    }
}

bool Console::validSource(string sourceCode) {
    stack<char> detector;
    bool insideText = false;
    // Detect braces, brackets and pharentesis
    for( char c : sourceCode )
        switch( c )
        {
        case '(':
            if( !insideText )
                detector.push('(');
            break;
        case '[':
            if( !insideText )
                detector.push('[');
            break;
        case '{':
            if( !insideText )
                detector.push('{');
            break;
        case ')':
            if( insideText )
                break;

            if( !detector.empty() && detector.top() == '(' )
                detector.pop();
            else
                return false;
            break;
        case ']':
            if( insideText )
                break;

            if( !detector.empty() && detector.top() == '[' )
                detector.pop();
            else
                return false;
            break;
        case '}':
            if( insideText )
                break;

            if( !detector.empty() && detector.top() == '{' )
                detector.pop();
            else
                return false;
            break;
        case '"':
            insideText = !insideText;
            break;
        default:
            break;
        }

    if( !detector.empty() || insideText )
        return false;

    if( sourceCode.back() == '}' || sourceCode.back() == ';' )
        return true;

    return false;
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

    if( lastJump != 0 )
        lastJump++;

    tmp = sourceCode.substr(lastJump, charReference - lastJump + 1);
    cout << "Next to: " << endl << '\t' << tmp << endl;
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
