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

void CommandHistory::recordCmd(std::string aCmd)
{
    // record the executed command in command history
    if (aCmd.size() == 0)
    {
        return;
    }

    if (mHistory.size() == MAX_HISTORY_SIZE)
    {
        mHistory.pop_front();
    }
    mHistory.push_back(aCmd);
    mCurrentIndex = mHistory.size();
    mInput = "";
    DEBUG_LOG_L3("cmdHistory: ", mHistory);
}

void CommandHistory::recordInput(std::string aInput)
{
    // record the current (not yet executed) command in a string
    if (mCurrentIndex == mHistory.size())
    {
        mInput = aInput;
    }
}

const std::string& CommandHistory::nextCmd()
{
    // fetch and return the next command

    if (mCurrentIndex + 1 >= mHistory.size())
    {
        mCurrentIndex = mHistory.size();
        return mInput;
    }
    else
    {
        return mHistory.at(++mCurrentIndex);
    }
}

const std::string& CommandHistory::lastCmd()
{
    // fetch and return the previous command
    if (mHistory.size() == 0)
    {
        return mInput;
    }

    if (mCurrentIndex == 0)
    {
        return mHistory.at(0);
    }
    else
    {
        return mHistory.at(--mCurrentIndex);
    }
}
