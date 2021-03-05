#include "logger.hpp"

int Logger::mDebugLevel = 0;
Logger* Logger::mLogger = nullptr;

void Logger::setDebugLevel(int aDebugLevel)
{
    mDebugLevel = aDebugLevel;
}

void Logger::initLogfile(std::string aLogFilename)
{
    static Logger logger;
    if (mLogger != nullptr)
    {
        return;
    }
    mLogger = &logger;
    logger.mLogFile.open(aLogFilename);
}

Logger::Logger()
{
}

Logger::~Logger()
{
    mLogFile.close();
}