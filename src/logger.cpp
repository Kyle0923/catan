/**
 * Project: catan
 * @file logger.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "logger.hpp"
#include "constant.hpp"

int Logger::mDebugLevel = constant::DEFAULT_DEBUG_LEVEL;
Logger* Logger::mLogger = nullptr;

void Logger::setDebugLevel(int aDebugLevel)
{
    mDebugLevel = aDebugLevel;
}

void Logger::initLogger(std::string aLogFilename)
{
    static Logger logger;
    if (mLogger != nullptr)
    {
        return;
    }
    mLogger = &logger;
    logger.mLogFile.open(aLogFilename);
    log("Copyright (c) 2021, Zonghao Huang <kyle0923@qq.com>\n"   \
                "All rights reserved.\n"    \
                "cantan.exe - Release version ", VER_MAJOR, '.', VER_MINOR, '.', VER_PATCH,  \
                ", Built on " __DATE__ " " __TIME__ "\n");
}

template<>
int Logger::print(std::stringstream& ostr, char aArg)
{
    if (isprint(aArg))
    {
        ostr << aArg;
        return 0;
    }
    else if (aArg == '\n' || aArg == '\r')
    {
        ostr << "\\n"; // print out "\n" instead of newline
    }
    else if (aArg == '\t')
    {
        ostr << "\\t"; // print out "\t" instead of tab
    }
    else if (aArg == ' ')
    {
        ostr << "'whitespace'"; // print out "\n" instead of newline
    }
    else
    {
        ostr << aArg;
    }
    ostr << " ASCII[" << (int)aArg << ']';
    return 0;
}

Logger::Logger()
{
}

Logger::~Logger()
{
    mLogFile.close();
}