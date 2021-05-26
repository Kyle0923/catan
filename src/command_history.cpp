/**
 * Project: catan
 * @file command_helper.cpp
 *
 * @author Bowei Li <cesterty024@gmail.com>
 *
 * All right reserved.
 */

#include "command_history.hpp"

CommandHistory::CommandHistory() = default;

void CommandHistory::recordCmd(std::string aCmd)
{
    if (mHistory.size() == 10) mHistory.pop_front();
    mHistory.push_back(aCmd);
    mCurrentIndex = mHistory.size() + 1;
    mCurrentCommand = "";
}

std::string CommandHistory::getCommand()
{
    return mCurrentCommand;
}

size_t CommandHistory::getHistorySize()
{
    return mHistory.size();
}

std::string CommandHistory::nextCmd()
{
    if (mCurrentIndex <= mHistory.size()) 
    {
        ++mCurrentIndex;
    } 

    if (mCurrentIndex >= 1 && mCurrentIndex <= mHistory.size())
    {
        mCurrentCommand = mHistory.at(mCurrentIndex - 1);
    }
    else {
        mCurrentCommand = "";
    }

    return mCurrentCommand;
}

std::string CommandHistory::lastCmd()
{
    if (mCurrentIndex > 0) 
    {
        --mCurrentIndex;
    } 

    if (mCurrentIndex >= 1 && mCurrentIndex <= mHistory.size())
    {
        mCurrentCommand = mHistory.at(mCurrentIndex - 1);
    }
    else {
        mCurrentCommand = "";
    }

    return mCurrentCommand;
}
