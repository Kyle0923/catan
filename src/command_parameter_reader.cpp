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
    if (aInput.find("exit") == 0)
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

    if (aInput.find("help") == 0)
    {
        pParamCmd->instruction(aReturnMsg);
        return ActionStatus::SUCCESS;
    }

    if (aInput == "" && aPoint == Point_t{0, 0})
    {
        // no param provided
        aReturnMsg.push_back("Parameter needed");
        pParamCmd->instruction(aReturnMsg);
        return ActionStatus::FAILED;
    }

    if (pParamCmd->processParameter(aMap, aInput, aPoint, aReturnMsg) != ActionStatus::SUCCESS)
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

std::vector<std::string> CommandParameterReader::getPossibleInputs(const std::string& aInput, std::string* const aAutoFillString) const
{
    std::vector<std::string> matchPool = mCmd->paramAutoFillPool();
    matchPool.emplace_back("help");
    matchPool.emplace_back("exit");
    return stringMatcher(aInput, matchPool, aAutoFillString);
}
