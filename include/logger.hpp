#ifndef INCLUDE_LOGGER_HPP
#define INCLUDE_LOGGER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define INFO_LOG(...) \
    Logger::info("In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define WARN_LOG(...) \
    Logger::warn("In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define ERROR_LOG(...) \
    Logger::error("In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)

#define DEBUG_LOG_L0(...) \
    Logger::debug(0, "In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define DEBUG_LOG_L1(...) \
    Logger::debug(1, "In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define DEBUG_LOG_L2(...) \
    Logger::debug(2, "In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)
#define DEBUG_LOG_L3(...) \
    Logger::debug(3, "In " __FILE__ ":", __LINE__, ": ", __VA_ARGS__)

class Logger
{
private:
    template<typename... Targs>
    static std::string log(std::ostream& ostr, Targs... aArgs)
    {
        std::stringstream strstream;
        _log(strstream, aArgs...);
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
    static void _log(std::stringstream& ostr, Targs... aArgs)
    {
        // pack expansion using brace-enclosed initializer
        __attribute__((unused)) int expander[sizeof...(Targs)] = { (ostr << aArgs, 0)... };
    };
#else
    template<typename T, typename... Targs>
    static void _log(std::stringstream& ostr, T aValue, Targs... aArgs)
    {
        ostr << aValue;
        _log(ostr, aArgs...);
    };
    template<typename T>
    static void _log(std::stringstream& ostr, T aValue)
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
        _log(strstream, aArgs...);
        return strstream.str();
    };
    template<typename... Targs>
    static void debug(int aLevel, Targs... aArgs)
    {
        if (aLevel >= mDebugLevel)
        {
            log(std::cout, "[Debug", aLevel, "] " , aArgs...);
        }
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
