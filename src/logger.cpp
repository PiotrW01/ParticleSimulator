#include "logger.h"
#include <iostream>

LogLevel Logger::logLevel = LogLevel::All;

void Logger::info(std::string str)
{
    std::cout << "[INFO] " << str << std::endl;
}

void Logger::info(const char *msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::info(int msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::info(float msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::warn(const char *msg)
{
    std::cout << "[WARN] " << msg << std::endl;
}

void Logger::error(const char *msg)
{
    std::cerr << "[ERROR] " << msg << std::endl;
}