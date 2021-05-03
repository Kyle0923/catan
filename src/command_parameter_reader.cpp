/**
 * Project: catan
 * @file command_parameter_reader.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_parameter_reader.hpp"

CommandParameterReader::CommandParameterReader(ParameterizedCommand* const aParamCmd):
    mParamCmd(aParamCmd)
{
    // empty
}

ActionStatus CommandParameterReader::act(GameMap& aMap, UserInterface& aUi, std::string aInput, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (mParamCmd->processParameter(aInput, aPoint, aReturnMsg) != ActionStatus::SUCCESS)
    {
        DEBUG_LOG_L3("read param failed for ", dynamic_cast<CommandHandler*>(mParamCmd)->command());
        return ActionStatus::FAILED;
    }
    DEBUG_LOG_L3("read param succeeded for ", dynamic_cast<CommandHandler*>(mParamCmd)->command());
    if (mParamCmd->parameterComplete())
    {
            DEBUG_LOG_L3("read param completed ", dynamic_cast<CommandHandler*>(mParamCmd)->command());
            CommandHandler* const pCmdHandler = dynamic_cast<CommandHandler*>(mParamCmd);
            if (!pCmdHandler)
            {
                ERROR_LOG("ParameterizedCommand is not a CommandHandler");
                return ActionStatus::FAILED;
            }
            aReturnMsg.clear();
            pCmdHandler->act(aMap, aUi, {}, aReturnMsg);
            return ActionStatus::EXIT;
    }
    DEBUG_LOG_L3("read param continue... ", dynamic_cast<CommandHandler*>(mParamCmd)->command());
    // read-in succeeded, but more parameters are required
    return ActionStatus::SUCCESS;
}

std::vector<std::string> CommandParameterReader::inputMatcher(std::string aInput, std::string* const aLongestCommonStr) const
{
    // TODO: to be implemented, (get a ptr *matchPool from cmdHandler?)
    // return stringMatcher(aInput, *matchPool, aLongestCommonStr);
    return {};
}
