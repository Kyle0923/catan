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

/**
 * @brief
 * an helper class to compose the offer,
 * does not inherit from CommandHandler,
 * therefore, it is not expected to be used by itself
 */
class OfferComposer
{
private:
    bool mOfferComplete;
    Offer_t mOffer;

public:
    ActionStatus composeOffer(const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg);
    const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const;
    void instruction(std::vector<std::string>& aReturnMsg) const;

    void reset();

    bool isOfferComplete() const;
    const Offer_t& getOffer() const;

    OfferComposer(const size_t aPlayerId);
};

class TradeHandler: public StatelessCommandHandler
{
private:
    static const std::vector<std::string> mTradingTargets;
    void instruction(std::vector<std::string>& aReturnMsg) const;

protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;

public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
};

class OfferInitiator: public StatefulCommandHandler
{
private:
    OfferComposer mOfferComposer;

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

    OfferInitiator(const size_t aPlayerId);
};

class OfferBroker: public StatefulCommandHandler
{
private:
    bool mOfferComplete;
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