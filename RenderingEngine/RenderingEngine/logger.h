#pragma once

// STD
#include <iostream>
#include <string>
#if defined(__linux__)
#include <memory>
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#if defined(_WIN32)
// Windows
#include <windows.h>
#endif

#if defined(__linux__)
// Implementation of the make_unique for the unique_ptr for linux/android
namespace std
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

namespace RenderingEngine
{
    using std::string;
    using std::ostream;
    using std::cout;
    using std::endl;

#if defined(_WIN32)
    /* Console colors, 0xAB A -> Background color, B -> Foreground color
    0 = Black       8 = Gray
    1 = Blue        9 = Light Blue
    2 = Green       A = Light Green
    3 = Aqua        B = Light Aqua
    4 = Red         C = Light Red
    5 = Purple      D = Light Purple
    6 = Yellow      E = Light Yellow
    7 = White       F = Bright White */

    // Set the text color to use on console
    inline void setTextColor(WORD color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }
#endif

    // Level of the logging being done in the application
    enum Level
    {
        Disabled = 0,
        Min,            // Only Info and Error messages
        All             // Info, Debug and Error messages
    };

    // Type of the log message
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
        Logger() : logType{ Info }, _out_stream( cout ) {};
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
        // Overload to set the type of the logging and the color of the text
        Logger &operator<<(const LogType &type)
        {
            logType = type;
#if defined(_WIN32)
            switch (logType)
            {
            case Info:  setTextColor(0x02); break;
            case Debug: setTextColor(0x06); break;
            case Func:  setTextColor(0x03); break;
            case Error: setTextColor(0x04); break;
            }
#endif
#ifndef __ANDROID__
            _out_stream << prefix[logType];
#endif
            return *this;
        }
        // Overload needed to use endl to add a new line
        Logger &operator<<(ostream& (*)(ostream&))
        {
            if (showLog())
#ifdef __ANDROID__
                __android_log_print(ANDROID_LOG_VERBOSE, prefix[logType].c_str(), "\n");
#else
                _out_stream << endl;
#endif
#if defined(_WIN32)
            setTextColor(0x07);
#endif
            return *this;
        }
        // Overload with templates to allow content of any type
#if defined (__ANDROID__)
        Logger &operator<<(const string &data)
#else
        template<typename T>
        Logger &operator<<(const T &data)
#endif
        {
            if (showLog())
#ifdef __ANDROID__
                __android_log_print(ANDROID_LOG_VERBOSE, prefix[logType].c_str(), data.c_str());
#else
                _out_stream << data;
#endif
            return *this;
        }
    };

    // Alias for the log class to call it with "Log" instead of "Logger::instance()"
    static auto &Log = Logger::instance();

#define Function makeTitle(__FUNCTION__)
    inline string makeTitle(string func) { Log << Func; return string{ "---------------" + func + "---------------" }; }
}
