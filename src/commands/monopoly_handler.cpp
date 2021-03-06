/**
 * Project: catan
 * @file monopoly_handler.cpp
 *
 * @brief handle monopoly development card
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_handlers.hpp"

MonopolyHandler::MonopolyHandler() :
    mResource(ResourceTypes::NONE)
{
    // empty
}

std::string MonopolyHandler::command() const
{
    return "monopoly_handler";
}

const std::vector<std::string>& MonopolyHandler::paramAutoFillPool(size_t aParamIndex) const
{
    if (!parameterComplete())
    {
        return consumableResourceStringValue;
    }
    return EMPTY_STRING_VECTOR;
}

ActionStatus MonopolyHandler::statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg)
{
    const size_t amountGot = aMap.currentPlayerPlayMonopoly(mResource);
    aReturnMsg.emplace_back(Logger::formatString( \
        "You got ", amountGot, " ", consumableResourceStringValue.at(static_cast<size_t>(mResource))));
    return ActionStatus::SUCCESS;
}

ActionStatus MonopolyHandler::onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (parameterComplete())
    {
        aReturnMsg.emplace_back("discarded parameter: " + aParam);
        return ActionStatus::SUCCESS;
    }

    int index = indexInVector(aParam, consumableResourceStringValue);
    if (index == -1)
    {
        aReturnMsg.emplace_back("unknown parameter: " + aParam);
        return ActionStatus::FAILED;
    }
    mResource = static_cast<ResourceTypes>(index);
    return ActionStatus::SUCCESS;
}

bool MonopolyHandler::parameterComplete() const
{
    return (mResource != ResourceTypes::NONE);
}

void MonopolyHandler::resetParameters()
{
    mResource = ResourceTypes::NONE;
}

void MonopolyHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (!parameterComplete())
    {
        aReturnMsg.emplace_back("Which resource you want to steal from all other players?");
        aReturnMsg.emplace_back(stringVectorJoin(consumableResourceStringValue));
        return;
    }
}
