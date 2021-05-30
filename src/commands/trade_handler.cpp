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
    mOutgoingOffer{ static_cast<size_t>(-1), {}, {} }
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
    return ActionStatus::SUCCESS;
}

// not used
ActionStatus TradeHandler::onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    return ActionStatus::SUCCESS;
}

ActionStatus TradeHandler::onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    // for ()
    // if (aParam == "")
    // {
    //     return ActionStatus::FAILED;
    // }

    // if (mOfferee == "")
    // {
    //     if (indexInVector(aParam, mOffereeMatchingPool) >= 0)
    //     {
    //         mOfferee = aParam;
    //         mOutgoingOffer.offeror = aMap.currentPlayer();
    //         return ActionStatus::SUCCESS;
    //     }
    //     else
    //     {
    //         return ActionStatus::FAILED;
    //     }
    // }

    return ActionStatus::SUCCESS;
}

bool TradeHandler::parameterComplete() const
{
    return true;
}

void TradeHandler::resetParameters()
{
    mOfferee = "";
    mOfferComposed = false;
    mOutgoingOffer.offeror = static_cast<size_t>(-1);
    mOutgoingOffer.lookingFor.clear();
    mOutgoingOffer.offering.clear();
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
}
