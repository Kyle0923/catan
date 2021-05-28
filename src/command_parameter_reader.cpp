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
    StatefulCommandHandler* const pStatefulCmd = dynamic_cast<StatefulCommandHandler*>(mCmd);

    if (aInput.find("exit") == 0)
    {
        if (pStatefulCmd)
        {
            pStatefulCmd->resetParameters();
        }
        return ActionStatus::EXIT;
    }

    if (aInput.find("help") == 0)
    {
        if (pStatefulCmd)
        {
            pStatefulCmd->instruction(aReturnMsg);
        }
        else
        {
            aReturnMsg.emplace_back(mCmd->description());
        }
        aReturnMsg.emplace_back("");
        aReturnMsg.emplace_back("exit - exit current command");
        return ActionStatus::SUCCESS;
    }

    std::vector<std::string> params = splitString(aInput);
    if (!pStatefulCmd)
    {
        // not inherit from StatefulCommandHandler
        ActionStatus rc = mCmd->act(aMap, aUi, params, aReturnMsg);
        INFO_LOG(mCmd->command() + "::act() returned " + actionStatusToStr(rc));
        if (rc != ActionStatus::PARAM_REQUIRED)
        {
            return ActionStatus::EXIT;
        }
        return ActionStatus::PARAM_REQUIRED;
    }

    if (aInput == "" && aPoint == Point_t{0, 0})
    {
        // no param provided
        pStatefulCmd->instruction(aReturnMsg);
        return ActionStatus::FAILED;
    }

    bool readSuccessful = true;
    for (std::string param : params)
    {
        ActionStatus rc = pStatefulCmd->onParameterReceive(aMap, param, aPoint, aReturnMsg);
        readSuccessful &= (rc == ActionStatus::SUCCESS);

        INFO_LOG(mCmd->command() + "::onParameterReceive(), param: " + param + ", ", \
                    aPoint, "; returned " + actionStatusToStr(rc));
    }

    if (!pStatefulCmd->parameterComplete())
    {
        if (!readSuccessful)
        {
            DEBUG_LOG_L3("read param failed for ", mCmd->command());
        }
        else
        {
            DEBUG_LOG_L3("parameter required for ", mCmd->command());
        }
        pStatefulCmd->instruction(aReturnMsg);
        return ActionStatus::PARAM_REQUIRED;
    }

    INFO_LOG("read param completed for " + mCmd->command() + ", calling act()");
    aReturnMsg.clear();
    ActionStatus rc = mCmd->act(aMap, aUi, {}, aReturnMsg);
    if (rc == ActionStatus::PARAM_REQUIRED)
    {
        return ActionStatus::PARAM_REQUIRED;
    }
    else
    {
        pStatefulCmd->resetParameters();
        return ActionStatus::EXIT;
    }
}

std::vector<std::string> CommandParameterReader::getPossibleInputs(const std::string& aInput, std::string* const aAutoFillString) const
{
    std::vector<std::string> params = splitString(aInput);
    std::vector<std::string> matchPool = mCmd->paramAutoFillPool(mCmd->currentParamIndex() + params.size() - 1);
    if (params.size() == 1)
    {
        matchPool.emplace_back("help");
        matchPool.emplace_back("exit");
    }
    return stringMatcher(params.back(), matchPool, aAutoFillString);
}
