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

#include "trading_system.hpp"

const std::vector<std::string> TradeHandler::mTradingTargets = {"bank", "player"};

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
        return mTradingTargets;
    }
    else
    {
        return mEmptyVector;
    }
}

ActionStatus TradeHandler::statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    if (aArgs.size() == 0)
    {
        instruction(aReturnMsg);
        return ActionStatus::PARAM_REQUIRED;
    }
    const std::string& arg = aArgs.front();
    int tradingTarget = indexInVector(arg, mTradingTargets);
    if (tradingTarget < 0)
    {
        aReturnMsg.emplace_back("Unknown parameter: " + arg);
        aReturnMsg.emplace_back("");
        instruction(aReturnMsg);
        return ActionStatus::PARAM_REQUIRED;
    }

    if (tradingTarget == 0)
    {
        // trading with bank
        // TODO:
        // aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(xxx));
        aReturnMsg.emplace_back("trading with bank");
    }
    else
    {
        // trading with player
        aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(&mOfferInitiator));
        mOfferInitiator.instruction(aReturnMsg);
    }

    return ActionStatus::SUCCESS;
}

void TradeHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    aReturnMsg.emplace_back("Please indicate who you want to trade with?");
    aReturnMsg.emplace_back(stringVectorJoin(mTradingTargets));
}
