/**
 * Project: catan
 * @file command_helper.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_history.hpp"

CommandHistory::CommandHistory() = default;

void CommandHistory::recordCmd(std::string cmd)
{
    if (mHistory.size() == 10) mHistory.erase(mHistory.begin());
    mHistory.push_back(cmd);
    mCurrentIndex = mHistory.size();
    mCurrentCommand = "";
}

std::string CommandHistory::getCommand()
{
    return mCurrentCommand;
}

void CommandHistory::nextCmd()
{
    if (mCurrentIndex == mHistory.size() - 1) 
    {
        mCurrentCommand = "";
    }
    else mCurrentCommand = mHistory.at(++mCurrentIndex);
}

void CommandHistory::lastCmd()
{
    if (mCurrentIndex == 0) 
    {
        mCurrentCommand = "";
    }
    else mCurrentCommand = mHistory.at(--mCurrentIndex);
}
