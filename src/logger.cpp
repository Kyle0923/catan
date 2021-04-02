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
#ifdef RELEASE
    log(true, "Copyright (c) 2021, Zonghao Huang <kyle0923@qq.com>\n"   \
                "All rights reserved.\n"    \
                "cantan.exe - Release version ", VER_MAJOR, '.', VER_MINOR, '.', VER_PATCH,  \
                ", Built on " __DATE__ " " __TIME__ "\n");
#endif /* RELEASE */
}

template<>
int Logger::print(std::stringstream& ostr, char aArg)
{
    if (isprint(aArg))
    {
        ostr << aArg;
    }
    else if (aArg == 13)
    {
        ostr << "\\n"; // print out "\n" instead of newline
    }
    else
    {
        ostr << aArg << " ASCII[" << (int)aArg << ']';
    }
    return 0;
}

Logger::Logger()
{
}

Logger::~Logger()
{
    mLogFile.close();
}