/**
 * Project: catan
 * @file command_history.cpp
 *
 * @author Bowei Li <cesterty024@gmail.com>
 *
 * All right reserved.
 */

#include "command_history.hpp"
#include "logger.hpp"
#include "constant.hpp"

void CommandHistory::pushToHistory(const std::string& aExecutedCmd)
{
    // push the executed command to command history
    if (aExecutedCmd.size() == 0)
    {
        return;
    }

    if (mHistory.size() == constant::MAX_HISTORY_SIZE)
    {
        mHistory.pop_front();
    }
    mHistory.push_back(aExecutedCmd);
    mHistoryIndex = mHistory.size();
    mCache = "";
    DEBUG_LOG_L3("cmdHistory: ", mHistory);
}

void CommandHistory::cacheInput(const std::string& aCurrentInput)
{
    // record the current (not yet executed) command in a string named mCache
    if (mHistoryIndex == mHistory.size())
    {
        mCache = aCurrentInput;
    }
}

const std::string& CommandHistory::nextHistory()
{
    // fetch and return the next command

    if (mHistoryIndex + 1 >= mHistory.size())
    {
        mHistoryIndex = mHistory.size();
        return mCache;
    }
    else
    {
        return mHistory.at(++mHistoryIndex);
    }
}

const std::string& CommandHistory::prevHistory()
{
    // fetch and return the previous command
    if (mHistory.size() == 0)
    {
        return mCache;
    }

    if (mHistoryIndex == 0)
    {
        return mHistory.at(0);
    }
    else
    {
        return mHistory.at(--mHistoryIndex);
    }
}
