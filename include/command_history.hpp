/**
 * Project: catan
 * @file command_history.hpp
 * @brief command history class
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
    std::deque<std::string> mHistory; // container to hold command history
    size_t mCurrentIndex = 0;
    std::string mInput;
public:
    void recordCmd(std::string);
    void recordInput(std::string);
    std::string nextCmd();
    std::string lastCmd();
};
#endif
