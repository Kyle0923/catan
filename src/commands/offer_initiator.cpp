/**
 * Project: catan
 * @file offer_initiator.cpp
 *
 * @brief handling trade activity
 *        exchange resources between players or bank/harbour
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "trading_system.hpp"

OfferInitiator::OfferInitiator() :
    mOfferComposed(false),
    mOutgoingOffer{ static_cast<size_t>(-1), {} }
{
    // empty
}

std::string OfferInitiator::command() const
{
    return "offer_initiator";
}

const std::vector<std::string>& OfferInitiator::paramAutoFillPool(size_t aParamIndex) const
{
    if (aParamIndex % 2 == 0)
    {
        return consumableResourceStringValue;
    }
    else
    {
        return mEmptyVector;
    }
}

ActionStatus OfferInitiator::statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg)
{
    aReturnMsg.emplace_back("Stub, OfferInitiator::statefulRun()");
    //TODO:
    // with player: pushCmdHelperStack(OfferBroker)
    return ActionStatus::SUCCESS;
}

// not used
ActionStatus OfferInitiator::onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    return ActionStatus::SUCCESS;
}

ActionStatus OfferInitiator::onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    mOutgoingOffer.offeror = aMap.currentPlayer();
    for (size_t index = 0; index < aArgs.size(); ++index)
    {
        const std::string& param = aArgs[index];

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
            aReturnMsg.emplace_back("Unknown parameter: " + param + ", discarded");
            continue;
        }
    }

    return ActionStatus::SUCCESS;
}

bool OfferInitiator::parameterComplete() const
{
    return mOfferComposed;
}

void OfferInitiator::resetParameters()
{
    mOfferComposed = false;
    mOutgoingOffer.offeror = static_cast<size_t>(-1);
    mOutgoingOffer.resources.clear();
}

void OfferInitiator::instruction(std::vector<std::string>& aReturnMsg) const
{
    aReturnMsg.emplace_back("Trading with player");

    aReturnMsg.emplace_back("Please compose the offer by entering the resource name followed by the amount of that resource");
    aReturnMsg.emplace_back("Enter positive value for the resources you are looking for");
    aReturnMsg.emplace_back("and negative value for the resources you are offering");
    aReturnMsg.emplace_back("Enter 'done' when you finish");

    if (mOutgoingOffer.resources.size() > 0)
    {
        aReturnMsg.emplace_back("");
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
