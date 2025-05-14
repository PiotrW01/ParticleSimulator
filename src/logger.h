#pragma once
#include <string>
#include <sstream>
#include <iostream>

class Logger
{
public:
    enum Level
    {
        INFO,
        WARNING,
        ERROR
    };

    static Logger info();
    static Logger warn();
    static Logger error();

    ~Logger();

    template<typename T>
    Logger& operator<<(const T& val)
    {
        stream << val;
        return *this;
    }

private:
    explicit Logger(Level level);
    Level logLevel;
    std::stringstream stream;
    std::string logLabel(Level lvl) const;
};