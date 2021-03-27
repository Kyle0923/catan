/**
 * Project: catan
 * @file logger.hpp
 * @brief logging utility, print message to file and console
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_LOGGER_HPP
#define INCLUDE_LOGGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define INFO_LOG(...) \
    Logger::info(__FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define WARN_LOG(...) \
    Logger::warn(__FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define ERROR_LOG(...) \
    Logger::error(__FILE__ ":", __LINE__, ": ", __VA_ARGS__)

#define DEBUG_LOG_L0(...) \
    Logger::debug(0, __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define DEBUG_LOG_L1(...) \
    Logger::debug(1, __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define DEBUG_LOG_L2(...) \
    Logger::debug(2, __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define DEBUG_LOG_L3(...) \
    Logger::debug(3, __FILE__ ":", __LINE__, ": ", __VA_ARGS__)

class Logger
{
private:
    template<typename... Targs>
    static std::string log(std::ostream& ostr, Targs... aArgs)
    {
        std::stringstream strstream;
        _formatString(strstream, aArgs...);
        std::string logMessage = strstream.str();
        if (mLogger)
        {
            mLogger->mLogFile << logMessage << std::endl;
        }
        else
        {
            std::cout << "[Info] Log file not initialized" << std::endl;
        }
        ostr << logMessage << std::endl;
        return logMessage;
    };
#ifndef RECURSION
    template<typename... Targs>
    static void _formatString(std::stringstream& ostr, Targs... aArgs)
    {
        // pack expansion using brace-enclosed initializer
        __attribute__((unused)) int expander[sizeof...(Targs)] = { (ostr << aArgs, 0)... };
    };
#else
    template<typename T, typename... Targs>
    static void _formatString(std::stringstream& ostr, T aValue, Targs... aArgs)
    {
        ostr << aValue;
        _formatString(ostr, aArgs...);
    };
    template<typename T>
    static void _formatString(std::stringstream& ostr, T aValue)
    {
        ostr << aValue;
    };
#endif // RECURSION
    static int mDebugLevel;
    static Logger* mLogger;
    std::ofstream mLogFile;
    Logger();
    ~Logger();
public:

    /*
     * higher debug_level => the message is more important
     * lower debug_level => the message is more generic
     * set higher level to filter out generic message
     */
    static void setDebugLevel(int aDebugLevel);
    static void initLogfile(std::string aLogFilename = "log.txt");
    template<typename... Targs>
    static std::string formatString(Targs... aArgs)
    {
        std::stringstream strstream;
        _formatString(strstream, aArgs...);
        return strstream.str();
    };
    template<typename... Targs>
    static void debug(int aLevel, Targs... aArgs)
    {
#ifndef RELEASE
        if (aLevel >= mDebugLevel)
        {
            log(std::cout, "[Debug", aLevel, "] " , aArgs...);
        }
#endif /* ifndef RELEASE */
    };
    template<typename... Targs>
    static void info(Targs... aArgs)
    {
        log(std::cout, "[Info] ", aArgs...);
    };
    template<typename... Targs>
    static void warn(Targs... aArgs)
    {
        log(std::cerr, "[Warn] ", aArgs...);
    };

    // Error log will terminate the program
    template<typename... Targs>
    static void error(Targs... aArgs)
    {
        std::string ErrMsg = log(std::cerr, "[Error] ", aArgs...);
        throw std::runtime_error(ErrMsg);
    };
};

#endif /* INCLUDE_LOGGER_HPP */
