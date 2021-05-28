/**
 * Project: catan
 * @file command_history.hpp
 * @brief command history class
 *        handling up/down keyboard event
 *        stores the command line history and return them when required
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
    static constexpr int MAX_HISTORY_SIZE = 10; // only holds the most recent ten history records
    size_t mCurrentIndex = 0;
    std::string mInput;
public:
    void recordCmd(std::string); // record the executed command in command history
    void recordInput(std::string); // record the current (not yet executed) command
    const std::string& nextCmd();
    const std::string& lastCmd();
};
#endif
