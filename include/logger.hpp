#ifndef INCLUDE_LOGGER_HPP
#define INCLUDE_LOGGER_HPP

#include <iostream>

class Logger
{
private:
    template<typename T, typename... Targs>
    static void log(std::ostream& ostr, T aValue, Targs... aArgs)
    {
        ostr << aValue;
        log(ostr, aArgs...);
    };
    template<typename T>
    static void log(std::ostream& ostr, T aValue)
    {
        ostr << aValue << std::endl;
    };
public:
    template<typename... Targs>
    static void debug(Targs... aArgs)
    {
        log(std::cout, "[Debug] ", aArgs...);
    };
    template<typename... Targs>
    static void warn(Targs... aArgs)
    {
        log(std::cerr, "[Warn] ", aArgs...);
    };
    Logger() = default;
    ~Logger() = default;
};

#endif /* INCLUDE_LOGGER_HPP */
