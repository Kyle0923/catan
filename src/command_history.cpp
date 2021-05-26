/**
 * Project: catan
 * @file command_history.cpp
 *
 * @author Bowei Li <cesterty024@gmail.com>
 *
 * All right reserved.
 */

#include "command_history.hpp"


void CommandHistory::recordCmd(std::string aCmd)
{
    // record the command in command history
    if (aCmd.size() == 0) 
    {
        return;
    }

    if (mHistory.size() == 10) 
    {
        mHistory.pop_front();
    } 
    mHistory.push_back(aCmd);
    mCurrentIndex = mHistory.size();
}


std::string CommandHistory::nextCmd()
{
    // fetch and return the next command
    if (mCurrentIndex < mHistory.size()) 
    {
        ++mCurrentIndex;
    } 

    if (mCurrentIndex >= 0 && mCurrentIndex < mHistory.size())
    {
        return mHistory.at(mCurrentIndex);
    }
    else 
    {
        return "";
    }
}


std::string CommandHistory::lastCmd()
{
    // fetch and return the previous command
    if (mCurrentIndex > 0) 
    {
        --mCurrentIndex;
    } 

    if (mCurrentIndex >= 0 && mCurrentIndex < mHistory.size())
    {
        return mHistory.at(mCurrentIndex);
    }
}
