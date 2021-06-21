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

OfferInitiator::OfferInitiator(const size_t aPlayerId) :
    mOfferComposer(aPlayerId)
{
    // empty
}

std::string OfferInitiator::command() const
{
    return "offer_initiator";
}

const std::vector<std::string>& OfferInitiator::paramAutoFillPool(size_t aParamIndex) const
{
    return mOfferComposer.paramAutoFillPool(aParamIndex);
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
    return mOfferComposer.composeOffer(aArgs, aReturnMsg);
}

bool OfferInitiator::parameterComplete() const
{
    return mOfferComposer.isOfferComplete();
}

void OfferInitiator::resetParameters()
{
    mOfferComposer.reset();
}

void OfferInitiator::instruction(std::vector<std::string>& aReturnMsg) const
{
    mOfferComposer.instruction(aReturnMsg);
}
