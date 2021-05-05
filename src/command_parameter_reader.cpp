/**
 * Project: catan
 * @file command_parameter_reader.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "utility.hpp"
#include "command_parameter_reader.hpp"

CommandParameterReader::CommandParameterReader(CommandHandler* const aCmd):
    mCmd(aCmd)
{
    // empty
}

ActionStatus CommandParameterReader::act(GameMap& aMap, UserInterface& aUi, std::string aInput, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (aInput == "exit" || aInput == "quit")
    {
        return ActionStatus::EXIT;
    }

    ParameterizedCommand* const pParamCmd = dynamic_cast<ParameterizedCommand*>(mCmd);
    if (!pParamCmd)
    {
        // not inherit from ParameterizedCommand
        ActionStatus rc = mCmd->act(aMap, aUi, splitString(aInput), aReturnMsg);
        INFO_LOG(mCmd->command() + "::act() returned " + actionStatusToStr(rc));
        if (rc != ActionStatus::PARAM_REQUIRED)
        {
            return ActionStatus::EXIT;
        }
        return ActionStatus::PARAM_REQUIRED;
    }

    if (pParamCmd->processParameter(aInput, aPoint, aReturnMsg) != ActionStatus::SUCCESS)
    {
        DEBUG_LOG_L3("read param failed for ", mCmd->command());
        return ActionStatus::FAILED;
    }
    DEBUG_LOG_L3("read param succeeded for ", mCmd->command());
    if (pParamCmd->parameterComplete())
    {
            DEBUG_LOG_L3("read param completed ", mCmd->command());
            aReturnMsg.clear();
            mCmd->act(aMap, aUi, {}, aReturnMsg);
            pParamCmd->resetParameters();
            return ActionStatus::EXIT;
    }
    DEBUG_LOG_L3("read param continue... ", mCmd->command());
    // read-in succeeded, but more parameters are required
    return ActionStatus::SUCCESS;
}

std::vector<std::string> CommandParameterReader::inputMatcher(std::string aInput, std::string* const aLongestCommonStr) const
{
    // TODO: to be implemented, (get a ptr *matchPool from cmdHandler?)
    // return stringMatcher(aInput, *matchPool, aLongestCommonStr);
    return {};
}
