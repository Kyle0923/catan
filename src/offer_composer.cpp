/**
 * Project: catan
 * @file offer_composer.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "trading_system.hpp"

OfferComposer::OfferComposer(const size_t aPlayerId) :
    mOfferComplete(false),
    mOffer{ aPlayerId, {} }
{
    // empty
}

const std::vector<std::string>& OfferComposer::paramAutoFillPool(size_t aParamIndex) const
{
    if (aParamIndex % 2 == 0)
    {
        return consumableResourceStringValue;
    }
    else
    {
        return CommandHandler::EMPTY_STRING_VECTOR;
    }
}

ActionStatus OfferComposer::composeOffer(const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    for (size_t index = 0; index < aArgs.size(); ++index)
    {
        const std::string& param = aArgs[index];

        if (param == "done")
        {
            mOfferComplete = true;
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
                mOffer.resources[static_cast<ResourceTypes>(resource)] = amount;
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

void OfferComposer::instruction(std::vector<std::string>& aReturnMsg) const
{
    aReturnMsg.emplace_back("Please compose the offer by entering the resource name followed by the amount of that resource");
    aReturnMsg.emplace_back("Enter positive value for the resources you are looking for");
    aReturnMsg.emplace_back("and negative value for the resources you are offering");
    aReturnMsg.emplace_back("Enter 'done' when you finish");

    if (mOffer.resources.size() > 0)
    {
        aReturnMsg.emplace_back("");
        std::string lookingFor = "";
        std::string offering = "";
        for (const auto& item : mOffer.resources)
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
        if (lookingFor.length() == 0)
        {
            lookingFor = "nothing";
        }
        if (offering.length() == 0)
        {
            offering = "nothing";
        }

        aReturnMsg.emplace_back("You are looking for " + lookingFor);
        aReturnMsg.emplace_back("You are offering " + offering);
    }
}

void OfferComposer::reset()
{
    mOfferComplete = false;
    mOffer.offeror = static_cast<size_t>(-1);
    mOffer.resources.clear();
}

bool OfferComposer::isOfferComplete() const
{
    return mOfferComplete;
}

const Offer_t& OfferComposer::getOffer() const
{
    return mOffer;
}
