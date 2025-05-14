#include "logger.h"

Logger::Logger(Level level) : logLevel(level) {}

Logger::~Logger() 
{
    std::ostream& out = (logLevel == ERROR) ? std::cerr : std::cout;
    out << logLabel(logLevel) << stream.str() << std::endl;
}

Logger Logger::info() { return Logger(INFO); }
Logger Logger::warn() { return Logger(WARNING); }
Logger Logger::error() { return Logger(ERROR); }

std::string Logger::logLabel(Level lvl) const {
    switch (lvl) 
    {
    case INFO: return "\033[37m[INFO]\033[0m ";
    case WARNING: return "\033[33m[WARNING]\033[0m ";
    case ERROR: return "\033[31m[ERROR]\033[0m ";
    default: return "[UNKNOWN] ";
    }
}