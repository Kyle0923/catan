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
    mCurrentIndex = mHistory.size();
    mCurrentCommand = "";
}

std::string CommandHistory::getCommand()
{
    return mCurrentCommand;
}

size_t CommandHistory::getCommandIndex()
{
    return mCurrentIndex;
}

size_t CommandHistory::getHistorySize()
{
    return mHistory.size();
}

std::string CommandHistory::nextCmd()
{
    if (mCurrentIndex == mHistory.size() - 1) mCurrentCommand = "";
    else mCurrentCommand = mHistory.at(++mCurrentIndex);
    return mCurrentCommand;
}

std::string CommandHistory::lastCmd()
{
    if (mCurrentIndex == 0) mCurrentCommand = "";
    else mCurrentCommand = mHistory.at(--mCurrentIndex);
    return mCurrentCommand;
}
