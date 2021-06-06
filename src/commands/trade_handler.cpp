/**
 * Project: catan
 * @file trade_handler.cpp
 *
 * @brief handling trade activity
 *        exchange resources between players or bank/harbour
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_handlers.hpp"

const std::vector<std::string> TradeHandler::mOffereeMatchingPool = {"bank", "player"};

TradeHandler::TradeHandler() :
    mOfferee(""),
    mOfferComposed(false),
    mOutgoingOffer{ static_cast<size_t>(-1), {} }
{
    // empty
}

std::string TradeHandler::command() const
{
    return "trade";
}

std::string TradeHandler::description() const
{
    return "exchange resources with other players or with bank/harbour";
}

const std::vector<std::string>& TradeHandler::paramAutoFillPool(size_t aParamIndex) const
{
    if (aParamIndex == 0)
    {
        return mOffereeMatchingPool;
    }
    else if (aParamIndex % 2 == 1)
    {
        return consumableResourceStringValue;
    }
    else
    {
        return mEmptyVector;
    }
}

size_t TradeHandler::currentParamIndex() const
{
    if (mOfferee == "")
    {
        return 0;
    }
    return 1;
}

ActionStatus TradeHandler::statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg)
{
    aReturnMsg.emplace_back("Stub, TradeHandler::statefulRun()");
    //TODO:
    // with bank: validate offer
    // with player: pushCmdHelperStack(TradeBroker?)
    return ActionStatus::SUCCESS;
}

// not used
ActionStatus TradeHandler::onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    return ActionStatus::SUCCESS;
}

ActionStatus TradeHandler::onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    for (size_t index = 0; index < aArgs.size(); ++index)
    {
        const std::string& param = aArgs[index];
        if (param == "")
        {
            return ActionStatus::FAILED;
        }

        if (mOfferee == "")
        {
            if (indexInVector(param, mOffereeMatchingPool) >= 0)
            {
                mOfferee = param;
                mOutgoingOffer.offeror = aMap.currentPlayer();
                continue;
            }
            else
            {
                aReturnMsg.emplace_back("Unkown parameter: " + param + ", abort");
                return ActionStatus::FAILED;
            }
        }

        if (param == "done")
        {
            mOfferComposed = true;
            return ActionStatus::SUCCESS;
        }

        int resource = indexInVector(param, consumableResourceStringValue);
        if (resource >= 0)
        {
            if (index + 1 < aArgs.size())
            {
                int amount = 0;
                if (!stringToInteger(aArgs[index + 1], amount))
                {
                    aReturnMsg.emplace_back("No numerical value for " + param + ", discarded");
                    continue;
                }
                // amount store the amount of the resource
                ++index;
                mOutgoingOffer.resources[static_cast<ResourceTypes>(resource)] = amount;
                continue;
            }
        }
        else
        {
            aReturnMsg.emplace_back("Unkown parameter: " + param + ", discarded");
            continue;
        }
    }

    return ActionStatus::SUCCESS;
}

bool TradeHandler::parameterComplete() const
{
    return mOfferComposed;
}

void TradeHandler::resetParameters()
{
    mOfferee = "";
    mOfferComposed = false;
    mOutgoingOffer.offeror = static_cast<size_t>(-1);
    mOutgoingOffer.resources.clear();
    mIncomingOffer.clear();
}

void TradeHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mOfferee == "")
    {
        aReturnMsg.emplace_back("Who do you want to trade with?");
        aReturnMsg.emplace_back(stringVectorJoin(mOffereeMatchingPool));
        return;
    }
    if (mOfferee == "bank")
    {
        aReturnMsg.emplace_back("Trading with bank");
    }
    else if (mOfferee == "player")
    {
        aReturnMsg.emplace_back("Trading with player");
    }
    aReturnMsg.emplace_back("Please compose the offer by entering the resource name followed by the amount of that resource");
    aReturnMsg.emplace_back("Enter positive value for the resources you are looking for");
    aReturnMsg.emplace_back("and negative value for the resources you are offering");
    aReturnMsg.emplace_back("Enter 'done' when you finish");
    aReturnMsg.emplace_back("");

    if (mOutgoingOffer.resources.size() > 0)
    {
        std::string lookingFor = "You are looking for ";
        std::string offering = "You are offering ";
        for (const auto& item : mOutgoingOffer.resources)
        {
            if (item.second < 0)
            {
                offering += std::to_string(-item.second) + " " + \
                            consumableResourceStringValue.at(static_cast<size_t>(item.first)) + " ";
            }
            else
            {
                lookingFor += std::to_string(item.second) + " " + \
                            consumableResourceStringValue.at(static_cast<size_t>(item.first)) + " ";
            }
        }
        aReturnMsg.emplace_back(lookingFor);
        aReturnMsg.emplace_back(offering);
    }

}
