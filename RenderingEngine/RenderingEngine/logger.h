#pragma once

// STD
#include <iostream>
#include <string>

// Windows
#include <windows.h>

namespace RenderingEngine
{
    using std::string;
    using std::ostream;
    using std::cout;
    using std::endl;

    /* Console colors, 0xAB A -> Background color, B -> Foreground color
    0 = Black       8 = Gray
    1 = Blue        9 = Light Blue
    2 = Green       A = Light Green
    3 = Aqua        B = Light Aqua
    4 = Red         C = Light Red
    5 = Purple      D = Light Purple
    6 = Yellow      E = Light Yellow
    7 = White       F = Bright White */
    inline void setTextColor(WORD color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }

    enum Level
    {
        Disabled = 0,
        Min,            // Only Info and Error messages
        All             // Info, Debug and Error messages
    };

    enum LogType
    {
        Info = 0,       // Normal messages of the application
        Debug,          // Debug messages used to narrow down an issue
        Func,           // Func messages display the function name
        Error,          // Error messages when something fails
        End
    };

    class Logger
    {
    private:
        // Log level
        Level logLevel;
        // Type of line being logged
        LogType logType;
        // Prefix for the line being logged
        string prefix[End]{ "<Info>: " , "<Debug>: ", "<Function>: ", "<Error>: " };
        // Output stream to send the log
        ostream &_out_stream;
        // Private constructor used for the singleton pattern, 
        // sets the default output to the screen and the logType to Info
        Logger() : logType{ Info }, _out_stream{ cout } {};
        // Delete the copy constructor and the assign operator of the logger class
        Logger(Logger const&) = delete;
        Logger &operator=(Logger const&) = delete;
        // Check if the log message is going to be shown
        bool showLog()
        {                                               // Levels
            if (logLevel != Disabled &&                 // Disabled, do not display any logging messages
                (logLevel == All ||                      // All, display all the messages
                    (logLevel == Min && logType != Debug)))  // Min, do not display debug messages
                return true;
            return false;
        }
    public:
        // Singleton method, return the instance when called
        static Logger &instance() { static Logger inst; return inst; }
        // Overload to change the logging level
        Logger &operator<<(const Level &level)
        {
            logLevel = level;
            return *this;
        }
        // Overload to set the type of the logging.
        Logger &operator<<(const LogType &type)
        {
            logType = type;
            switch (logType)
            {
            case Info:  setTextColor(0x02); break;
            case Debug: setTextColor(0x06); break;
            case Func:  setTextColor(0x03); break;
            case Error: setTextColor(0x04); break;
            }
            _out_stream << prefix[logType];
            return *this;
        }
        // Overload needed to use endl
        Logger &operator<<(ostream& (*)(ostream&))
        {
            if (showLog())
                _out_stream << endl;
            setTextColor(0x07);
            return *this;
        }
        // Overload with templates to allow content of any type
        template<typename T>
        Logger &operator<<(const T &data)
        {
            if (showLog())
                _out_stream << data;
            return *this;
        }
    };

    // Alias for the log class to call it with "Log" instead of "Logger::instance()"
    static auto &Log = Logger::instance();

#define Function makeTitle(__FUNCTION__)
    inline string makeTitle(string func) { Log << Func; return string{ "---------------" + func + "---------------" }; }
}
