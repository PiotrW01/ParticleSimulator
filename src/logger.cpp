#include "logger.h"

Logger::Logger(Level level, const char* file, int line) 
    : logLevel(level), file(file), line(line) {}

Logger::~Logger() 
{
    std::ostream& out = (logLevel == ERROR) ? std::cerr : std::cout;
    out << logLabel(logLevel) << "|" << getFilename(file) << ":" << line << "| " << stream.str() << std::endl;
}

//Logger LOG_INFO { return Logger(INFO); }
//Logger LOG_WARN { return Logger(WARNING); }
//Logger LOG_ERROR { return Logger(ERROR); }

std::string Logger::logLabel(Level lvl) const {
    switch (lvl) 
    {
    case INFO: return "\033[37m[INFO]\033[0m ";
    case WARNING: return "\033[33m[WARNING]\033[0m ";
    case ERROR: return "\033[31m[ERROR]\033[0m ";
    default: return "[UNKNOWN] ";
    }
}