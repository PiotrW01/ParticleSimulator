#pragma once
#include <string>
#include <sstream>

enum class LogLevel
{
    All,
    Info,
    Warning,
    Error,
    Off
};

class Logger
{
public:
    static void setLogLevel(LogLevel lvl);
    static void info(std::string str);
    static void info(const char *msg);
    static void info(int msg);
    static void info(float msg);
    static void warn(const char *msg);
    static void error(const char *msg);

private:
    static LogLevel logLevel;
};