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
    size_t mHistoryIndex = 0; // index to indicate which history to display
    std::string mCache; // denotes the cached input
public:
    void pushToHistory(const std::string&); // push the executed command to command history
    void cacheInput(const std::string&); // record the current (not yet executed) User Input
    const std::string& nextHistory();
    const std::string& prevHistory();
};
#endif
