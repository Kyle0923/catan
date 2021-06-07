/**
 * Project: catan
 * @file trading_system.hpp
 * @brief handlers that related to trading
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_TRADING_SYSTEM_HPP
#define INCLUDE_TRADING_SYSTEM_HPP

#include <map>
#include "command_dispatcher.hpp"
#include "command_parameter_reader.hpp"
#include "command_common.hpp"
#include "game_map.hpp"
#include "user_interface.hpp"
#include "utility.hpp"
#include "logger.hpp"

#if 0
Offer_composer

Trade |- With_bank
      |- With_player -> offer_initiator -> offer_broker -> offer_finalizer

#endif

struct Offer_t {
    size_t offeror;
    std::map<ResourceTypes, int> resources;
};

class OfferInitiator: public StatefulCommandHandler
{
private:
    bool mOfferComposed;
    Offer_t mOutgoingOffer;

protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;

    // not used
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;

public:
    virtual std::string command() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    OfferInitiator();
};

class TradeHandler: public StatelessCommandHandler
{
private:
    static const std::vector<std::string> mTradingTargets;
    void instruction(std::vector<std::string>& aReturnMsg) const;
    OfferInitiator mOfferInitiator;

protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;

public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
};

class OfferBroker: public StatefulCommandHandler
{
private:
    bool mOfferComposed;
    Offer_t mIncomingOffer;

protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;

    // not used
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;

public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
    virtual size_t currentParamIndex() const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    OfferBroker();
};

#endif /* INCLUDE_TRADING_SYSTEM_HPP */