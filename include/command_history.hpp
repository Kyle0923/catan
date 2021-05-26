/**
 * Project: catan
 * @file common.hpp
 * @brief common data types
 *
 * @author Bowei Li <cesterty024@gmail.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_HISTORY_HPP
#define INCLUDE_COMMAND_HISTORY_HPP

#include<deque>
#include<string>

class CommandHistory
{
private:
    std::deque<std::string>mHistory;
    size_t mCurrentIndex = 0;
    std::string mCurrentCommand;
public:
    CommandHistory();
    void recordCmd(std::string);
    std::string getCommand();
    size_t getHistorySize();
    std::string nextCmd();
    std::string lastCmd();
};
#endif
