#pragma once
#include <string>
#include <sstream>
#include <iostream>

#define LOG_INFO Logger(Logger::Level::INFO,    __FILE__, __LINE__)
#define LOG_WARN Logger(Logger::Level::WARNING, __FILE__, __LINE__)
#define LOG_ERROR Logger(Logger::Level::ERROR,  __FILE__, __LINE__)

class Logger
{
public:
    enum Level
    {
        INFO,
        WARNING,
        ERROR
    };

    explicit Logger(Level level, const char* file, int line);
    ~Logger();

    //static Logger info();
    //static Logger warn();
    //static Logger error();

    template<typename T>
    Logger& operator<<(const T& val)
    {
        stream << val;
        return *this;
    }

private:
    Level logLevel;
    const char* file;
    int line;
    std::stringstream stream;
    std::string logLabel(Level lvl) const;
private:
    constexpr const char* getFilename(const char* path) {
        const char* file = path;
        while (*path) {
            if (*path == '/' || *path == '\\')
                file = path + 1;
            ++path;
        }
        return file;
    }
};